/***************************************************************************|
|  OMM - Open Multimedia                                                    |
|                                                                           |
|  Copyright (C) 2019                                           |
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

#include <Poco/String.h>
#include <Poco/LineEndingConverter.h>
#include <Poco/Notification.h>
#include <Poco/Util/ServerApplication.h>
#include <Poco/Util/Option.h>
#include <Poco/Util/OptionSet.h>
#include <Poco/Util/HelpFormatter.h>

#include "Omm/Upnp.h"


class OmmBusApplication :  public Poco::Util::ServerApplication
{
public:
    OmmBusApplication(int argc, char** argv);
    ~OmmBusApplication();

private:
    virtual void defineOptions(Poco::Util::OptionSet& options);
    virtual void handleOption(const std::string& name, const std::string& value);
        
    virtual int runEventLoop(int argc, char** argv);
    virtual int main(const std::vector<std::string>& args);
    void displayHelp();
    
    bool                                        _helpRequested;
    int                                         _argc;
    char**                                      _argv;

    Omm::SsdpBus*                               _pSsdpBus;
};


OmmBusApplication::OmmBusApplication(int argc, char** argv) :
_argc(argc),
_argv(argv),
_helpRequested(false)
{
    setUnixOptions(true);
    _pSsdpBus = new Omm::SsdpBus;
}


OmmBusApplication::~OmmBusApplication()
{
}


void
OmmBusApplication::defineOptions(Poco::Util::OptionSet& options)
{
    Poco::Util::ServerApplication::defineOptions(options);

    options.addOption(Poco::Util::Option("help", "", "display help information on command line arguments")
                      .required(false)
                      .repeatable(false));
//    options.addOption(Poco::Util::Option("server", "s", "add server \"name=<name>, uuid=<uuid>, model=<model>, path=<path>\"")
//                      .required(false)
//                      .repeatable(true)
//                      .argument("serverSpec", true));
//    options.addOption(Poco::Util::Option("application", "a", "application config parameters")
//                      .required(false)
//                      .repeatable(false)
//                      .argument("applicationSpec", true));
}


void
OmmBusApplication::handleOption(const std::string& name, const std::string& value)
{
    Poco::Util::ServerApplication::handleOption(name, value);

    if (name == "help") {
        _helpRequested = true;
    }
//    else if (name == "server") {
//        std::map<std::string,std::string> params;
//        std::set<std::string> allowed;
//        allowed.insert("name");
//        allowed.insert("uuid");
//        allowed.insert("model");
//        allowed.insert("path");
//
//        if (parseParameters(params, allowed, value)) {
//            std::string uuid = params["uuid"];
//            if (uuid == "") {
//                uuid = Poco::UUIDGenerator().createRandom().toString();
//            }
//            params["uuid"] = uuid;
//            params["enable"] = "true";
//            setParameters(params, "server." + uuid);
//        }
//        else {
//            LOGNS(Av, upnpav, error, "wrong server spec in command line, ignoring");
//        }
//    }
//    else if (name == "application") {
//        std::map<std::string,std::string> params;
//        std::set<std::string> allowed;
//        allowed.insert("checkInstance");
//
//        if (parseParameters(params, allowed, value)) {
//            setParameters(params, "application");
//        }
//        else {
//            LOGNS(Av, upnpav, error, "wrong application spec in command line, ignoring");
//        }
//    }
}


void
OmmBusApplication::displayHelp()
{
    Poco::Util::HelpFormatter helpFormatter(options());
    helpFormatter.setCommand(commandName());
    helpFormatter.setUsage("OPTIONS");
    helpFormatter.setHeader("OMM SSDP bus server.");
    helpFormatter.format(std::cout);
}


int
OmmBusApplication::runEventLoop(int argc, char** argv)
{
//    initLocalDevices();
    _pSsdpBus->start();
//    run(argc, argv);
    // FIXME: on windows Ctrl-C terminates immediately without shutting down the application
    waitForTerminationRequest();
    _pSsdpBus->stop();
    return Poco::Util::ServerApplication::EXIT_OK;
}


int
OmmBusApplication::main(const std::vector<std::string>& args)
{
    int ret = Poco::Util::ServerApplication::EXIT_OK;

    if (_helpRequested)
    {
        displayHelp();
    }
    else
    {
//        installGlobalErrorHandler();
//
////        Poco::Util::ServerApplication::init(_argc, _argv);
//
//        _featureCheckInstance = config().getBool("application.checkInstance", false);
//        if (_featureCheckInstance && instanceAlreadyRunning()) {
//            LOG(upnp, information, "omm application instance running, starting in controller mode");
//            setLockInstance(false);
//            setIgnoreConfigFile(true);
//        }
////        loadConfig();
//        initConfig();
//
        ret = runEventLoop(_argc, _argv);
    }
    return ret;
}


int main(int argc, char** argv)
{
    OmmBusApplication app(argc, argv);
    
    return app.run(argc, argv);
}
