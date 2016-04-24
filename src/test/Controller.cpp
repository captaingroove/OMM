/***************************************************************************|
|  OMM - Open Multimedia                                                    |
|                                                                           |
|  Copyright (C) 2009, 2010                                                 |
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

#include "Poco/Util/ServerApplication.h"

#include "Log.h"
#include "Upnp.h"
//#include "UpnpAv.h"
//#include "UpnpAvCtlServer.h"


class TestController : public Omm::Controller
{
    virtual void addDeviceContainer(Omm::DeviceContainer* pDeviceContainer, int index, bool begin)
    {
        std::cout << "TestController device container added(), root device uuid: " + pDeviceContainer->getRootDevice()->getUuid() << std::endl;
    }

    virtual void removeDeviceContainer(Omm::DeviceContainer* pDeviceContainer, int index, bool begin)
    {
        std::cout << "TestController device container removed(), root device uuid: " + pDeviceContainer->getRootDevice()->getUuid() << std::endl;
    }
};


class ControllerApp: public Poco::Util::ServerApplication
{
protected:
    int main(const std::vector<std::string>& args)
    {
        TestController controller;
//        Omm::Av::CtlMediaServerGroup deviceGroup;
//        controller.registerDeviceGroup(&deviceGroup);
        controller.init();
        controller.setState(Omm::Controller::Public);
        Omm::LOG(upnp, debug, "ControllerApp::main() waiting for termination request");
        waitForTerminationRequest();
        controller.setState(Omm::Controller::Stopped);
        return Application::EXIT_OK;
    }
};


int main(int argc, char** argv)
{
    ControllerApp app;
    return app.run(argc, argv);
}