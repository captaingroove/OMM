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

#ifndef GuiUpnpApplication_INCLUDED
#define GuiUpnpApplication_INCLUDED

#include "UpnpApplication.h"
#include "../Gui/Application.h"


namespace Omm {

class ControllerWidget;
class GuiVisual;

class GuiUpnpApplication :  public UpnpApplication, public Gui::Application
{
    friend class ConfigRequestHandler;

public:
    static const std::string ModeFull;
    static const std::string ModeRendererOnly;

    GuiUpnpApplication(int argc, char** argv);
    ~GuiUpnpApplication();

    // Omm::Gui::Application interface
    virtual void start();
    virtual void stop();

    void enableController(bool enable = true);
    void showRendererVisualOnly(bool show = true);
    Av::MediaRenderer* getLocalRenderer();

    void setFullscreen(bool fullscreen = true);

protected:
    // Poco::Util::Application interface
    virtual void defineOptions(Poco::Util::OptionSet& options);
    virtual void handleOption(const std::string& name, const std::string& value);

    virtual int runEventLoop(int argc, char** argv);
    virtual void defaultConfig();
    virtual void initConfig();
    virtual void saveConfig();

    // local devices
    virtual void initLocalDevices();

    virtual std::stringstream* getPlaylistResource();

private:
    // Omm::Gui::Application interface
    virtual Omm::Gui::View* createMainView();
    virtual void presentedMainView();

    // local devices
    void setLocalRenderer(const std::string& name, const std::string& uuid, const std::string& pluginName);
    void setLocalRenderer();

    int                                         _argc;
    char**                                      _argv;

    ControllerWidget*                           _pControllerWidget;
    bool                                        _enableController;

    Av::MediaRenderer*                          _pLocalMediaRenderer;
    bool                                        _enableRenderer;
    std::string                                 _rendererName;
    std::string                                 _rendererUuid;
    std::string                                 _rendererPlugin;
    ui2                                         _defaultRendererVolume;
    bool                                        _showRendererVisualOnly;
    GuiVisual*                                  _pRendererVisual;
};


}  // namespace Omm


#endif
