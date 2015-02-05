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

#ifndef UpnpApplication_INCLUDED
#define UpnpApplication_INCLUDED

#include <Poco/Notification.h>
#include <Poco/Util/ServerApplication.h>
#include <Poco/Util/Option.h>
#include <Poco/Util/OptionSet.h>
#include <Poco/Util/HelpFormatter.h>
#include <Poco/Util/XMLConfiguration.h>
#include <Poco/Util/PropertyFileConfiguration.h>
#include <Poco/Net/SocketAddress.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

#include "../Upnp.h"
#include "../UpnpAvCtlRenderer.h"
#include "../UpnpAvRenderer.h"
#include "../UpnpAvCtlServer.h"
#include "../UpnpAvCtlObject.h"
#include "../UpnpAvRenderer.h"
#include "../UpnpAvServer.h"


namespace Omm {

class WebSetup;

class UpnpApplication :  public Poco::Util::ServerApplication
{
    friend class ConfigRequestHandler;

public:
    static const std::string PLAYLIST_URI;
    static const std::string CONFIG_URI;
    static const std::string CONFIG_APP_QUERY;
    static const std::string CONFIG_DEV_QUERY;

    UpnpApplication(int argc, char** argv);
    ~UpnpApplication();

    void startApp();
    void stopApp();

    void setLockInstance(bool lock = true);
    void setIgnoreConfig(bool ignore = true);
    bool getIgnoreConfig();
    std::string getAppHttpUri();
    std::string getConfigHttpUri();
    Poco::Util::PropertyFileConfiguration* getFileConfiguration();
    void restartLocalDeviceContainer();
    void setLocalDeviceContainerState(const std::string& state);
    Av::MediaServer* getLocalMediaServer(const std::string& uuid);

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

    bool                                        _helpRequested;
    bool                                        _ignoreConfig;
    Poco::Util::PropertyFileConfiguration*      _pConf;
    DeviceContainer*                            _pLocalDeviceContainer;
    bool                                        _enableLocalDeviceServer;

private:
    // Poco::Util::Application interface
    virtual int main(const std::vector<std::string>& args);

    // application configuration
    void displayHelp();
    void printConfig();
    void printForm(const Poco::Net::HTMLForm& form);
    void loadConfig();

    // local devices
    void addLocalServer(const std::string& id);

    // other stuff
    void startAppHttpServer();
    void stopAppHttpServer();
    bool instanceAlreadyRunning();
    void installGlobalErrorHandler();

    int                                         _argc;
    char**                                      _argv;
    bool                                        _lockInstance;

    std::string                                 _confFilePath;
    WebSetup*                                   _pWebSetup;

    DeviceServer*                               _pLocalDeviceServer;
    bool                                        _enableServer;
    std::string                                 _instanceMutexName;

    // _socket is needed for
    // 1. playlist editor resource imports
    // 2. configuration of app
    int                                         _appStandardPort;
    Poco::Net::ServerSocket                     _socket;
    Poco::Net::HTTPServer*                      _pHttpServer;
};


class ConfigRequestHandler: public Poco::Net::HTTPRequestHandler
{
public:
    ConfigRequestHandler(UpnpApplication* pApp) : _pApp(pApp) {}

    void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);

private:
    UpnpApplication*        _pApp;
};


class ConfigRequestHandlerFactory : public Poco::Net::HTTPRequestHandlerFactory
{
public:
    ConfigRequestHandlerFactory(UpnpApplication* pApp) : _pApp(pApp) {}

    Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest& request);

private:
    UpnpApplication*        _pApp;
};


}  // namespace Omm


#endif
