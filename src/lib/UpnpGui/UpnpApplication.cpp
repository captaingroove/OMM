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

#include "UpnpGui/UpnpApplication.h"
#include "UpnpGui/WebSetup.h"
#include "Sys/Path.h"
#include "Sys/System.h"
#include "Util.h"
#include "UpnpAv.h"
#include "UpnpAvCtlServer.h"


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

void
ConfigRequestHandler::handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response)
{
    LOG(upnp, debug, "omm application http request \"" + request.getURI() + "\" from " + request.getHost());

    Poco::URI requestUri(request.getURI());

    if (requestUri.getPath() == UpnpApplication::PLAYLIST_URI) {
        std::stringstream* pPlaylistResource = _pApp->getPlaylistResource();
        if (pPlaylistResource) {
            std::ostream& outStream = response.send();
            Poco::StreamCopier::copyStream(*pPlaylistResource, outStream);
            delete pPlaylistResource;
        }
    }
    else if (requestUri.getPath() == UpnpApplication::CONFIG_URI) {
//        response.setChunkedTransferEncoding(true);
        response.setContentType("text/html");

        Poco::Net::HTMLForm form(request, request.stream());
        if (requestUri.getQuery() == UpnpApplication::CONFIG_APP_QUERY) {
            _pApp->_pWebSetup->handleAppConfigRequest(form);
        }
        else if (requestUri.getQuery() == UpnpApplication::CONFIG_DEV_QUERY) {
            _pApp->_pWebSetup->handleDevConfigRequest(form);
        }

        std::ostream& outStream = response.send();
        std::istream* pConfigPage = _pApp->_pWebSetup->generateConfigPage();
        Poco::StreamCopier::copyStream(*pConfigPage, outStream);
        delete pConfigPage;
    }
    else {
        response.send();
    }
}


Poco::Net::HTTPRequestHandler*
ConfigRequestHandlerFactory::createRequestHandler(const Poco::Net::HTTPServerRequest& request)
{
    return new ConfigRequestHandler(_pApp);
}


class GlobalErrorHandler : public Poco::ErrorHandler
{
public:
    void exception(const Poco::Exception& exc)
    {
        LOG(upnp, error, "global error handler: " + exc.displayText());
    }


    void exception(const std::exception& exc)
    {
        LOG(upnp, error, "global error handler: " + std::string(exc.what()));
    }


    void exception()
    {
        LOG(upnp, error, "global error handler: unknown exception");
    }
};


const std::string UpnpApplication::PLAYLIST_URI = "/Playlist";
const std::string UpnpApplication::CONFIG_URI = "/Config";
const std::string UpnpApplication::CONFIG_APP_QUERY = "saveForm=app";
const std::string UpnpApplication::CONFIG_DEV_QUERY = "saveForm=dev";

UpnpApplication::UpnpApplication(int argc, char** argv) :
//Poco::Util::Application(argc, argv),
_argc(argc),
_argv(argv),
_helpRequested(false),
_lockInstance(true),
_ignoreConfig(false),
//#ifdef __IPHONE__
//    _rendererName("iPhone Renderer"),
//#else
//    _rendererName("OMM Renderer"),
//#endif
_enableServer(false),
_enableLocalDeviceServer(false),
_pLocalDeviceServer(new DeviceServer),
_pLocalDeviceContainer(new DeviceContainer),
_pConf(0),
_instanceMutexName("OmmApplicationMutex"),
_appStandardPort(4009),
_pHttpServer(0)
{
    setUnixOptions(true);
}


UpnpApplication::~UpnpApplication()
{
    delete _pLocalDeviceServer;
    if (_pHttpServer) {
        delete _pHttpServer;
    }
}


int
UpnpApplication::runEventLoop(int argc, char** argv)
{
    initLocalDevices();
    startApp();
//    run(argc, argv);
    // FIXME: on windows Ctrl-C terminates immediately without shutting down the application
    waitForTerminationRequest();
    stopApp();
    return Poco::Util::ServerApplication::EXIT_OK;
}


void
UpnpApplication::defineOptions(Poco::Util::OptionSet& options)
{
    Poco::Util::ServerApplication::defineOptions(options);

    options.addOption(Poco::Util::Option("help", "", "display help information on command line arguments")
                      .required(false)
                      .repeatable(false));
    options.addOption(Poco::Util::Option("server", "s", "add server \"name:uuid:datamodel:basepath\"")
                      .required(false)
                      .repeatable(true)
                      .argument("serverSpec", true));
}


void
UpnpApplication::handleOption(const std::string& name, const std::string& value)
{
    Poco::Util::ServerApplication::handleOption(name, value);

    if (name == "help") {
        _helpRequested = true;
    }
    else if (name == "server") {
        std::map<std::string,std::string> params;
        std::set<std::string> allowed;
        allowed.insert("name");
        allowed.insert("uuid");
        allowed.insert("model");
        allowed.insert("path");

        if (parseParameters(params, allowed, value)) {
            std::string uuid = params["uuid"];
            if (uuid == "") {
                uuid = Poco::UUIDGenerator().createRandom().toString();
            }
            params["uuid"] = uuid;
            params["enable"] = "true";
            setParameters(params, "server." + uuid);
        }
        else {
            LOGNS(Av, upnpav, error, "wrong server spec in command line, ignoring");
        }
    }
}


bool
UpnpApplication::parseParameters(std::map<std::string,std::string>& parameters, const std::set<std::string>& allowedNames, const std::string& values)
{
    Poco::StringTokenizer serverSpec(values, ",");
    bool correctSyntax = true;
    for (Poco::StringTokenizer::Iterator it = serverSpec.begin(); it != serverSpec.end(); ++it) {
        Poco::StringTokenizer paramSpec(*it, "=");
        if (paramSpec.count() < 2) {
            LOGNS(Av, upnpav, error, "parameter of server spec must be a comma seperated list of \"key = value\" pairs, but is: " + *it + ", ignoring");
            correctSyntax = false;
        }
        else {
            std::string key = Poco::trim(paramSpec[0]);
            std::string value = Poco::trim(paramSpec[1]);
            if (allowedNames.find(key) != allowedNames.end()) {
                parameters[key] = value;
            }
            else {
                correctSyntax = false;
            }
        }
    }
    return correctSyntax;
}


void
UpnpApplication::setParameters(const std::map<std::string,std::string>& parameters, const std::string& baseKey)
{
    for (std::map<std::string,std::string>::const_iterator it = parameters.begin(); it != parameters.end(); ++it) {
        config().setString(baseKey + "." + it->first, it->second);
    }
}


int
UpnpApplication::main(const std::vector<std::string>& args)
{
    int ret = Poco::Util::ServerApplication::EXIT_OK;

    if (_helpRequested)
    {
        displayHelp();
    }
    else
    {
        installGlobalErrorHandler();

//        Poco::Util::ServerApplication::init(_argc, _argv);
    // TODO: reenable _feature instance checking (segfaults with mutex)
//        if (instanceAlreadyRunning()) {
//            LOG(upnp, information, "omm application instance running, starting in controller mode");
//            setLockInstance(false);
//            setIgnoreConfig(true);
//        }
        loadConfig();
        initConfig();

        ret = runEventLoop(_argc, _argv);
    }
    return ret;
}


void
UpnpApplication::startApp()
{
    LOGNS(Av, upnpav, debug, "omm application starting ...");
    startAppHttpServer();
    LOGNS(Av, upnpav, debug, "omm application starting local device server ...");
    if (_enableLocalDeviceServer) {
        _pLocalDeviceServer->setState(config().getString("application.devices", DeviceManager::Public));
    }
    LOGNS(Av, upnpav, debug, "omm application started.");
}


void
UpnpApplication::stopApp()
{
    LOGNS(Av, upnpav, debug, "omm application stopping ...");
    if (_enableLocalDeviceServer) {
        _pLocalDeviceServer->setState(DeviceManager::Stopped);
    }
    stopAppHttpServer();
    saveConfig();
    Poco::Util::Application::uninitialize();
    LOGNS(Av, upnpav, debug, "omm application stopped.");
}


void
UpnpApplication::displayHelp()
{
    Poco::Util::HelpFormatter helpFormatter(options());
    helpFormatter.setCommand(commandName());
    helpFormatter.setUsage("OPTIONS");
    helpFormatter.setHeader("OMM application running a UPnP controller, renderer and servers.");
    helpFormatter.format(std::cout);
}


void
UpnpApplication::setLockInstance(bool lock)
{
    _lockInstance = lock;
}


void
UpnpApplication::setIgnoreConfig(bool ignore)
{
    _ignoreConfig = ignore;
}


bool
UpnpApplication::getIgnoreConfig()
{
    return _ignoreConfig;
}


void
UpnpApplication::printConfig()
{
//    std::vector<std::string> rootKeys;
//    config().keys(rootKeys);
//    for (std::vector<std::string>::iterator it = rootKeys.begin(); it != rootKeys.end(); ++it) {
//        LOGNS(Av, upnpav, debug, "omm config, root keys: " + *it);
//    }

    std::vector<std::string> rendererKeys;
    config().keys("renderer", rendererKeys);
    for (std::vector<std::string>::iterator it = rendererKeys.begin(); it != rendererKeys.end(); ++it) {
        LOGNS(Av, upnpav, debug, "omm config renderer." + *it + ": " + config().getString("renderer." + *it, ""));
    }

    std::vector<std::string> serverKeys;
    config().keys("server", serverKeys);
    for (std::vector<std::string>::iterator it = serverKeys.begin(); it != serverKeys.end(); ++it) {
        std::vector<std::string> serverConfigKeys;
        config().keys("server." + *it, serverConfigKeys);
        for (std::vector<std::string>::iterator cit = serverConfigKeys.begin(); cit != serverConfigKeys.end(); ++cit) {
            LOGNS(Av, upnpav, debug, "omm config server." + *it + "." + *cit + ": " + config().getString("server." + *it + "." + *cit, ""));
        }
    }

//    if (_pConf) {
//        std::vector<std::string> confKeys;
//        _pConf->keys(confKeys);
//        for (std::vector<std::string>::iterator it = confKeys.begin(); it != confKeys.end(); ++it) {
//            LOGNS(Av, upnpav, debug, "omm config, config file keys: " + *it + ", value: " + _pConf->getString(*it, ""));
//        }
//    }

//    std::vector<std::string> appKeys;
//    config().keys("application", appKeys);
//    for (std::vector<std::string>::iterator it = appKeys.begin(); it != appKeys.end(); ++it) {
//        LOGNS(Av, upnpav, debug, "omm config, application keys: " + *it + ", value: " + config().getString("application." + *it, ""));
//    }
//
//    std::vector<std::string> sysKeys;
//    config().keys("system", sysKeys);
//    for (std::vector<std::string>::iterator it = sysKeys.begin(); it != sysKeys.end(); ++it) {
//        LOGNS(Av, upnpav, debug, "omm config, system keys: " + *it + ", value: " + config().getString("system." + *it, ""));
//    }
}


void
UpnpApplication::printForm(const Poco::Net::HTMLForm& form)
{
    for (Poco::Net::NameValueCollection::ConstIterator it = form.begin(); it != form.end(); ++it)
    {
        LOGNS(Av, upnpav, debug, "form " + it->first + ": " + it->second);
    }
}


void
UpnpApplication::defaultConfig()
{
    LOGNS(Av, upnpav, information, "creating application default config");

    int serverCount = 0;
    std::string serverString;

    _pConf->setString("server.0.path", Sys::SysPath::getPath(Sys::SysPath::Home));
    _pConf->setBool("server.0.checkMod", false);
    _pConf->setBool("server.0.enable", false);
    _pConf->setString("server.0.name", "OMM Media Files");
    _pConf->setString("server.0.layout", Av::ServerContainer::LAYOUT_FLAT);
    _pConf->setString("server.0.model", "file");
    _pConf->setInt("server.0.pollUpdateId", 0);
    serverString += Poco::NumberFormatter::format(serverCount++);

    _pConf->setString("server.1.path", "webradio.xml");
    _pConf->setBool("server.1.checkMod", false);
    _pConf->setBool("server.1.enable", false);
    _pConf->setString("server.1.name", "OMM Webradio");
    _pConf->setString("server.1.layout", Av::ServerContainer::LAYOUT_FLAT);
    _pConf->setString("server.1.model", "webradio");
    _pConf->setInt("server.1.pollUpdateId", 0);
    serverString += (serverCount ? "," : "") + Poco::NumberFormatter::format(serverCount++);

    std::vector<Sys::Device*> dvbDevices;
    Sys::System::instance()->getDevicesForType(dvbDevices, Sys::System::DeviceTypeDvb);
    if(dvbDevices.size() > 0) {
        _pConf->setString("server.2.path", "dvb.xml");
        _pConf->setBool("server.2.checkMod", false);
        _pConf->setBool("server.2.enable", false);
        _pConf->setString("server.2.name", "OMM Digital TV");
        _pConf->setString("server.2.layout", Av::ServerContainer::LAYOUT_FLAT);
        _pConf->setString("server.2.model", "dvb");
        _pConf->setInt("server.2.pollUpdateId", 0);
        serverString += (serverCount ? "," : "") + Poco::NumberFormatter::format(serverCount++);
    }

    _pConf->setString("server.new.path", "");
    _pConf->setBool("server.new.checkMod", false);
    _pConf->setBool("server.new.enable", false);
    _pConf->setString("server.new.name", "");
    _pConf->setString("server.new.layout", Av::ServerContainer::LAYOUT_FLAT);
    _pConf->setString("server.new.model", "");
    _pConf->setInt("server.new.pollUpdateId", 0);

    _pConf->setString("servers", serverString);
}


void
UpnpApplication::loadConfig()
{
    if (!_ignoreConfig) {
        LOGNS(Av, upnpav, information, "reading config file ...");

        _confFilePath = Omm::Util::Home::instance()->getConfigDirPath("/") + "omm.properties";
        _pConf = new Poco::Util::PropertyFileConfiguration;
        try {
            _pConf->load(_confFilePath);
            LOGNS(Av, upnpav, information, "reading config file done.");
        }
        catch (Poco::Exception& e) {
            LOGNS(Av, upnpav, warning, "could not read config file: " + e.displayText());
            defaultConfig();
        }
    //        config().addWriteable(_pConf, -200);
        config().addWriteable(_pConf, 0);
    }
    printConfig();
}


void
UpnpApplication::initConfig()
{
    _pWebSetup = new WebSetup(this);
}


void
UpnpApplication::saveConfig()
{
    if (!_ignoreConfig) {
        try {
            _pConf->save(_confFilePath);
            LOGNS(Av, upnpav, information, "saving config file done.");
        }
        catch (Poco::Exception& e) {
            LOGNS(Av, upnpav, error, "could not write config file present" + e.displayText());
        }
    }
}


void
UpnpApplication::initLocalDevices()
{
    // add local servers from config file
    std::string serversString;
    if (_pConf) {
        serversString = _pConf->getString("servers", "");
    }
    Poco::StringTokenizer servers(serversString, ",");
    for (Poco::StringTokenizer::Iterator it = servers.begin(); it != servers.end(); ++it) {
        LOGNS(Av, upnpav, debug, "omm config, server: " + *it);
        if (_pConf->getBool("server." + *it + ".enable", false)) {
            addLocalServer(*it);
        }
    }
    // add local servers from command line
    std::vector<std::string> serverKeys;
    config().keys("server", serverKeys);
    for (std::vector<std::string>::iterator it = serverKeys.begin(); it != serverKeys.end(); ++it) {
        if (!(_pConf && _pConf->hasProperty("server." + *it + ".enable"))) {
            LOGNS(Av, upnpav, debug, "omm config, server: " + *it);
            if (config().getBool("server." + *it + ".enable", false)) {
                addLocalServer(*it);
            }
        }
    }
    if (_enableServer) {
        _enableLocalDeviceServer = true;
    }

    if (_enableLocalDeviceServer) {
        _pLocalDeviceServer->addDeviceContainer(_pLocalDeviceContainer);
        _pLocalDeviceServer->init();
    }

   LOGNS(Av, upnpav, debug, "omm application init local devices done.");
}


std::stringstream*
UpnpApplication::getPlaylistResource()
{
    return 0;
}


void
UpnpApplication::restartLocalDeviceContainer()
{
    _pLocalDeviceServer->setState(DeviceManager::Stopped);
    _pLocalDeviceServer = new DeviceServer;
//        _pLocalDeviceServer->removeDeviceContainer(_pLocalDeviceContainer);
    // delete local devices
//        delete _pLocalDeviceContainer;
    _pLocalDeviceContainer = new DeviceContainer;
    initConfig();
    initLocalDevices();
    _pLocalDeviceServer->setState(DeviceManager::Public);
//    _pLocalDeviceServer->setState(DeviceManager::Local);
}


void
UpnpApplication::setLocalDeviceContainerState(const std::string& state)
{
    _pLocalDeviceServer->setState(state);
}


Av::MediaServer*
UpnpApplication::getLocalMediaServer(const std::string& uuid)
{
    return dynamic_cast<Av::MediaServer*>(_pLocalDeviceContainer->getDevice(uuid));
}


void
UpnpApplication::addLocalServer(const std::string& id)
{
    LOGNS(Av, upnpav, debug, "omm application add local server with id: " + id + " ...");

    std::string pluginName = "model-" + config().getString("server." + id + ".model", "webradio");
    Omm::Av::AbstractDataModel* pDataModel;
#ifdef __IPHONE__
    if (pluginName == "model-mpmedia") {
        pDataModel = new MPMediaModel;
    }
    else if (pluginName == "model-iphonecamera") {
        pDataModel = new IPhoneCameraModel;
    }
    else {
        return;
    }
#else
    Omm::Util::PluginLoader<Omm::Av::AbstractDataModel> pluginLoader;
    try {
        pDataModel = pluginLoader.load(pluginName);
    }
    catch(Poco::NotFoundException) {
        LOGNS(Av, upnpav, error, "could not find server model plugin: " + pluginName);
        return;
    }
    LOGNS(Av, upnpav, information, "container model plugin: " + pluginName + " loaded successfully");
#endif

    pDataModel->setCacheDirPath(Util::Home::instance()->getCacheDirPath(), id);
    pDataModel->setMetaDirPath(Util::Home::instance()->getMetaDirPath(), id);
    pDataModel->setTextEncoding(config().getString("server." + id + ".textEncoding", "UTF8"));
    pDataModel->setCheckObjectModifications(config().getBool("server." + id + ".checkMod", false));

    _enableServer = true;
    Omm::Av::MediaServer* pMediaServer = new Av::MediaServer;

    std::string name = config().getString("server." + id + ".name", "OMM Server");
    Omm::Av::ServerContainer* pContainer = new Av::ServerContainer(pMediaServer);
    pContainer->setTitle(name);
    pContainer->setClass(Omm::Av::AvClass::className(Omm::Av::AvClass::CONTAINER));
    pContainer->setDataModel(pDataModel);
    std::string basePath = config().getString("server." + id + ".path", "webradio.xml");
    std::string path = basePath;
    Poco::Path ppath(path);
    // if relative path then search file in config directory
    if (ppath.isRelative()) {
        path = Omm::Util::Home::instance()->getConfigDirPath() + basePath;
    }
    pContainer->setBasePath(path);
    pContainer->setLayout(config().getString("server." + id + ".layout", Av::ServerContainer::LAYOUT_FLAT));

    pMediaServer->setRoot(pContainer);
    pMediaServer->setFriendlyName(name);
    pMediaServer->setUuid(config().getString("server." + id + ".uuid", ""));
    Omm::Icon* pIcon = new Omm::Icon(32, 32, 8, "image/png", "server.png");
    pMediaServer->addIcon(pIcon);
    pMediaServer->setPollSystemUpdateIdTimer(config().getInt("server." + id + ".pollUpdateId", 0));

    _pLocalDeviceContainer->addDevice(pMediaServer);
    _pLocalDeviceContainer->setRootDevice(pMediaServer);

    LOGNS(Av, upnpav, debug, "omm application add local server finished.");
}


void
UpnpApplication::startAppHttpServer()
{
    int port = config().getInt("application.configPort", _appStandardPort);
    bool useRandomPort = true;
// TODO: reenable _feature instance checking (segfaults with mutex)
//    if (!instanceAlreadyRunning()) {
        try {
            _socket = Poco::Net::ServerSocket(port);
            useRandomPort = false;
        }
        catch (Poco::Exception& e) {
            LOG(upnp, error, "failed to start application http server on port " + Poco::NumberFormatter::format(port) + "(" + e.displayText() + ") , using random port.");
        }
//    }
    if (useRandomPort) {
        try {
            _socket = Poco::Net::ServerSocket(0);
        }
        catch (Poco::Exception& e) {
            LOG(upnp, error, "failed to start application http server: " + e.displayText());
        }
    }
    Poco::Net::HTTPServerParams* pParams = new Poco::Net::HTTPServerParams;
    _pHttpServer = new Poco::Net::HTTPServer(new ConfigRequestHandlerFactory(this), _socket, pParams);
    _pHttpServer->start();
    LOG(upnp, information, "omm application http server listening on: " + _socket.address().toString());
}


void
UpnpApplication::stopAppHttpServer()
{
    _pHttpServer->stop();
    LOG(upnp, information, "omm application http server stopped on port: " + Poco::NumberFormatter::format(_socket.address().port()));
}


bool
UpnpApplication::instanceAlreadyRunning()
{
#ifdef __IPHONE__
    return false;
#endif

    if (_lockInstance) {
        _lockInstance = false;
        Poco::NamedMutex mutex(_instanceMutexName);
        return !mutex.tryLock();
    }
    else {
        return false;
    }
}


void
UpnpApplication::installGlobalErrorHandler()
{
    static GlobalErrorHandler errorHandler;
    Poco::ErrorHandler* pOldErrorHandler = Poco::ErrorHandler::set(&errorHandler);
}


std::string
UpnpApplication::getAppHttpUri()
{
    std::string address = Net::NetworkInterfaceManager::instance()->getValidIpAddress().toString();
    return "http://" + address + ":" + Poco::NumberFormatter::format(_socket.address().port());
}


std::string
UpnpApplication::getConfigHttpUri()
{
    // FIXME: on second process instance, configuration is ignored, but config page of first process instance
    // should be displayed. If standard port is used, this works, but not if another port is configured.
    // However, getting the configured port doesn't solve the problem, as it may be in use when first instance was started,
    // and so first instance chose a random port.
    return "http://localhost:" + config().getString("application.configPort", Poco::NumberFormatter::format(_appStandardPort)) + "/Config";
//    return "http://localhost:" + config().getString("application.configPort", Poco::NumberFormatter::format(_socket.address().port())) + "/Config";
//    return "http://localhost:" + Poco::NumberFormatter::format(_socket.address().port()) + "/Config";
}


Poco::Util::PropertyFileConfiguration*
UpnpApplication::getFileConfiguration()
{
    return _pConf;
}


} // namespace Omm
