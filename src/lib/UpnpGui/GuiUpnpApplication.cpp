/***************************************************************************|
|  OMM - Open Multimedia                                                    |
|                                                                           |
|  Copyright (C) 2011                                                       |
|  Jörg Bakker (jb'at'open-multimedia.org)                                  |
|                                                                           |
|  This file is part of OMM.                                                |
|                                                                           |
|  OMM is free software: you can redistribute it and/or modify              |
|  it under the terms of the GNU General Public License as published by     |
|  the Free Software Foundation version 3 of the License.                   |
|                                                                           |
|  OMM is distributed in the hope that it will be useful,                   |
|  but WITHOUT ANY WARRANTY; without even the implied warranty of           |
|  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            |
|  GNU General Public License for more details.                             |
|                                                                           |
|  You should have received a copy of the GNU General Public License        |
|  along with this program.  If not, see <http://www.gnu.org/licenses/>.    |
 ***************************************************************************/

#include <iostream>

#include <Poco/NotificationCenter.h>
#include <Poco/Observer.h>
#include <Poco/UUIDGenerator.h>
#include <Poco/StreamCopier.h>
#include <Poco/NamedMutex.h>
#include "Poco/ErrorHandler.h"
#include <Poco/Util/Application.h>

#include "UpnpGui/ControllerWidget.h"
#include "UpnpGui/GuiUpnpApplication.h"
#include "UpnpGui/GuiSetup.h"
#include "UpnpGui/GuiVisual.h"
#include "Sys/Path.h"
#include "Sys/System.h"
#include "Util.h"
#include "Gui/GuiLogger.h"
#include "UpnpAv.h"
#include "UpnpAvCtlServer.h"
#include "MediaImages.h"


// FIXME: make engines also loadable, so that ommcontroller executable
//        doesn't need to link against an engine.
#ifdef __IPHONE__
#include <Omm/X/EngineMPMoviePlayer.h>
#include <Omm/X/EngineAVFoundation.h>
#include <Omm/X/MPMedia.h>
#include <Omm/X/iPhoneCamera.h>
#else
#include <Omm/X/EngineVlc.h>
#include <Omm/X/EnginePhonon.h>
#include <Poco/StringTokenizer.h>
#include <Poco/Net/NameValueCollection.h>
#endif

namespace Omm {

const std::string GuiUpnpApplication::ModeFull = "ModeFull";
const std::string GuiUpnpApplication::ModeRendererOnly = "ModeRendererOnly";

GuiUpnpApplication::GuiUpnpApplication(int argc, char** argv) :
UpnpApplication(argc, argv),
_argc(argc),
_argv(argv),
//#ifdef __IPHONE__
//    _rendererName("iPhone Renderer"),
//#else
//    _rendererName("OMM Renderer"),
//#endif
_enableRenderer(false),
_enableController(true),
_showRendererVisualOnly(false),
_pControllerWidget(0),
_pLocalMediaRenderer(0),
_defaultRendererVolume(50)
{
}


GuiUpnpApplication::~GuiUpnpApplication()
{
}


void
GuiUpnpApplication::defineOptions(Poco::Util::OptionSet& options)
{
    UpnpApplication::defineOptions(options);

    options.addOption(Poco::Util::Option("width", "w", "width of application window")
                      .binding("application.width")
                      .required(false)
                      .repeatable(false)
                      .argument("width", true));
    options.addOption(Poco::Util::Option("height", "h", "height of application window")
                      .binding("application.height")
                      .required(false)
                      .repeatable(false)
                      .argument("height", true));
    options.addOption(Poco::Util::Option("fullscreen", "f", "option passed to plugin")
                      .binding("application.fullscreen")
                      .required(false)
                      .repeatable(false));
    options.addOption(Poco::Util::Option("zoom", "z", "application window scale factor")
                      .binding("application.scale")
                      .required(false)
                      .repeatable(false)
                      .argument("scale", true));
    options.addOption(Poco::Util::Option("controller", "c", "controller config parameters")
                      .required(false)
                      .repeatable(false)
                      .argument("controllerSpec", true));
    options.addOption(Poco::Util::Option("renderer", "r", "enable renderer  \"name=<name>, uuid=<uuid>, engine=<engine>\"")
                      .required(false)
                      .repeatable(false)
                      .argument("rendererSpec", true));
}


void
GuiUpnpApplication::handleOption(const std::string& name, const std::string& value)
{
    UpnpApplication::handleOption(name, value);

    if (name == "renderer") {
        std::map<std::string,std::string> params;
        std::set<std::string> allowed;
        allowed.insert("name");
        allowed.insert("uuid");
        allowed.insert("engine");

        if (parseParameters(params, allowed, value)) {
            std::string uuid = params["uuid"];
            if (uuid == "") {
                uuid = Poco::UUIDGenerator().createRandom().toString();
            }
            params["uuid"] = uuid;
            params["enable"] = "true";
            setParameters(params, "renderer");
        }
        else {
            LOGNS(Av, upnpav, error, "wrong renderer spec in command line, ignoring");
        }
    }
    else if (name == "fullscreen") {
        config().setString("application.fullscreen", "true");
    }
    else if (name == "controller") {
        std::map<std::string,std::string> params;
        std::set<std::string> allowed;
        allowed.insert("subscribeEventing");
        allowed.insert("showNetworkActivity");
        allowed.insert("pollPosition");
        allowed.insert("trackConnection");
        allowed.insert("handleUpdateId");

        if (parseParameters(params, allowed, value)) {
            setParameters(params, "controller");
        }
        else {
            LOGNS(Av, upnpav, error, "wrong controller spec in command line, ignoring");
        }
    }
}


int
GuiUpnpApplication::runEventLoop(int argc, char** argv)
{
    return Gui::Application::runEventLoop(_argc, _argv);
}


Omm::Gui::View*
GuiUpnpApplication::createMainView()
{
    if (_showRendererVisualOnly) {
        _pRendererVisual = new GuiVisual();
        return _pRendererVisual;
    }
    _pControllerWidget = new Omm::ControllerWidget(this);
    addControlPanel(_pControllerWidget->getControlPanel());
    addControlPanel(_pControllerWidget->getStatusBar());
    return _pControllerWidget;
}


void
GuiUpnpApplication::presentedMainView()
{
    if (_showRendererVisualOnly) {
        if (!config().getBool("renderer.enable", false)) {
            config().setString("renderer.enable", "true");
            config().setString("renderer.name", "OMM Player");
            config().setString("renderer.engine", "vlc");
        }
    }
    else {
        if (config().getBool("application.fullscreen", false)) {
            _pControllerWidget->setHandlesHidden(true);
    //        _pControllerWidget->showOnlyBasicDeviceGroups(true);
        }
        else {
            _pControllerWidget->setRendererVisualVisible(false);
        }
        _pControllerWidget->init();
    }
    initLocalDevices();
}


void
GuiUpnpApplication::start()
{
    UpnpApplication::startApp();

    LOGNS(Av, upnpav, debug, "omm gui application starting ...");
    if (_enableController) {
        LOGNS(Av, upnpav, debug, "omm application starting controller ...");
        _pControllerWidget->setFeatureSubscribeEventing(config().getBool("controller.subscribeEventing", true));
        _pControllerWidget->setFeatureShowNetworkActivity(config().getBool("controller.showNetworkActivity", true));
        _pControllerWidget->setState(DeviceManager::PublicLocal);
    }
    LOGNS(Av, upnpav, debug, "omm gui application started.");
}


void
GuiUpnpApplication::stop()
{
    LOGNS(Av, upnpav, debug, "omm gui application stopping ...");
    if (_enableController) {
        _pControllerWidget->setState(DeviceManager::Stopped);
    }
    LOGNS(Av, upnpav, debug, "omm gui application stopped.");

    UpnpApplication::stopApp();
}


void
GuiUpnpApplication::defaultConfig()
{
    UpnpApplication::defaultConfig();

    LOGNS(Av, upnpav, information, "creating gui application default config");
    _pConf->setBool("renderer.enable", true);
    _pConf->setString("renderer.name", "OMM Media Player");
    _pConf->setString("renderer.engine", "vlc");
}


void
GuiUpnpApplication::initConfig()
{
    UpnpApplication::initConfig();

    setFullscreen(config().getBool("application.fullscreen", false));
    resizeMainView(config().getInt("application.width", 800), config().getInt("application.height", 480));
    scaleMainView(config().getDouble("application.scale", 1.0));
}


void
GuiUpnpApplication::saveConfig()
{
    if (!_ignoreConfig) {
        LOGNS(Av, upnpav, information, "saving config file ...");
        _pConf->setInt("application.width", width());
        _pConf->setInt("application.height", height());
        if (!config().getBool("application.fullscreen", false)) {
            _pConf->setString("application.cluster", _pControllerWidget->getConfiguration());
        }
        _pConf->setInt("renderer.volume", _pLocalMediaRenderer->getEngine()->getVolume(Omm::Av::AvChannel::MASTER));

        UpnpApplication::saveConfig();
    }
}


void
GuiUpnpApplication::enableController(bool enable)
{
//    if (_pControllerWidget) {
//        enable ? _pControllerWidget->start() : _pControllerWidget->stop();
//    }
    _enableController = enable;
}


void
GuiUpnpApplication::showRendererVisualOnly(bool show)
{
    _showRendererVisualOnly = show;
    _enableController = false;
}


void
GuiUpnpApplication::initLocalDevices()
{
    // add local renderer
    LOGNS(Av, upnpav, debug, "omm gui application init local devices ...");
    if (config().getBool("renderer.enable", false)) {
        setLocalRenderer(config().getString("renderer.name", "OMM Renderer"),
                config().getString("renderer.uuid", ""),
                config().getString("renderer.engine", ""));
    }
//#ifndef __IPHONE__
    if (_enableRenderer) {
        _enableLocalDeviceServer = true;
        setLocalRenderer();
    }
//#endif

    UpnpApplication::initLocalDevices();

//#ifndef __IPHONE__
    if (_enableRenderer && _pLocalMediaRenderer && !_showRendererVisualOnly) {
        _pControllerWidget->setDefaultRenderer(_pLocalMediaRenderer);
    }
//#endif
   LOGNS(Av, upnpav, debug, "omm gui application init local devices done.");
}


std::stringstream*
GuiUpnpApplication::getPlaylistResource()
{
    return _pControllerWidget->getPlaylistResource();
}


void
GuiUpnpApplication::setLocalRenderer(const std::string& name, const std::string& uuid, const std::string& pluginName)
{
    _enableRenderer = true;
    _rendererName = name;
    _rendererUuid = uuid;
    _rendererPlugin = "engine-" + pluginName;
}


void
GuiUpnpApplication::setLocalRenderer()
{
    LOGNS(Av, upnpav, debug, "omm application set local renderer ...");

    Omm::Av::Engine* pEngine;
#ifdef __IPHONE__
        pEngine = new MPMoviePlayerEngine;
//        pEngine = new AVFoundationEngine;
#else
    Omm::Util::PluginLoader<Omm::Av::Engine> pluginLoader;
    try {
        pEngine = pluginLoader.load(_rendererPlugin);
    }
    catch(Poco::NotFoundException) {
        LOGNS(Av, upnpav, error, "could not find engine plugin: " + _rendererPlugin);
        pEngine = new VlcEngine;
    }
    LOGNS(Av, upnpav, information, "engine plugin: " + _rendererPlugin + " loaded successfully");
#endif

    pEngine->createPlayer();
    if (_showRendererVisualOnly) {
        pEngine->setVisual(_pRendererVisual);
    }
    else {
        pEngine->setVisual(_pControllerWidget->getLocalRendererVisual());
    }
    // set default soft volume of engine
    ui2 volume = config().getInt("renderer.volume", _defaultRendererVolume);
    pEngine->setVolume(Omm::Av::AvChannel::MASTER, volume);

    Av::MediaRenderer* pMediaRenderer = new Av::MediaRenderer;
    _pLocalMediaRenderer = pMediaRenderer;
    pMediaRenderer->addEngine(pEngine);
    Omm::Icon* pRendererIcon = new Omm::Icon(22, 22, 8, "image/png", "renderer.png");
    pMediaRenderer->addIcon(pRendererIcon);
    pMediaRenderer->setFriendlyName(_rendererName);
    pMediaRenderer->setUuid(_rendererUuid);
    _pLocalDeviceContainer->addDevice(pMediaRenderer);
    _pLocalDeviceContainer->setRootDevice(pMediaRenderer);

    LOGNS(Av, upnpav, debug, "omm application set local renderer finished.");
}


Av::MediaRenderer*
GuiUpnpApplication::getLocalRenderer()
{
    return _pLocalMediaRenderer;
}


} // namespace Omm
