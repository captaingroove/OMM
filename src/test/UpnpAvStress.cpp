/***************************************************************************|
|  OMM - Open Multimedia                                                    |
|                                                                           |
|  Copyright (C) 2009, 2010, 2011, 2015                                     |
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
#include <vector>
#include <string>

#include <Poco/Thread.h>
#include <Poco/NumberFormatter.h>
#include <Poco/Random.h>
#include <Poco/Environment.h>
#include <Poco/DirectoryIterator.h>
#include <Poco/Util/ServerApplication.h>
#include <Poco/Util/Option.h>
#include <Poco/Util/OptionSet.h>
#include <Poco/Util/HelpFormatter.h>

#include <Omm/Util.h>
#include <Omm/Upnp.h>
#include <Omm/UpnpAv.h>
#include <Omm/UpnpAvCtlServer.h>
#include <Omm/UpnpAvCtlRenderer.h>
#include <Omm/UpnpAvCtlObject.h>


class MediaServerGroup : public Omm::DeviceGroup
{
public:
    MediaServerGroup(Omm::DeviceGroupDelegate* pDeviceGroupDelegate) : DeviceGroup(pDeviceGroupDelegate) {}

private:
    virtual Omm::Device* createDevice() { return new Omm::Av::CtlMediaServer; }
};


class MediaRendererGroup : public Omm::DeviceGroup
{
public:
    MediaRendererGroup(Omm::DeviceGroupDelegate* pDeviceGroupDelegate) : DeviceGroup(pDeviceGroupDelegate) {}

private:
    virtual Omm::Device* createDevice() { return new Omm::Av::CtlMediaRenderer; }
};


class AvStressController : public Poco::Util::ServerApplication
{
public:
    AvStressController():
        _helpRequested(false),
        _mediaRendererGroup(new Omm::Av::MediaRendererGroupDelegate),
        _mediaServerGroup(new Omm::Av::MediaServerGroupDelegate),
        _controllerThreadRunnable(*this, &AvStressController::controllerThread),
        _controllerThreadRunning(false)
    {}

    void displayHelp() {}
    void start();
    void stop();


protected:
    int main(const std::vector<std::string>& args);

private:
    void controllerThread();
    bool controllerThreadRunning();

    Omm::Av::CtlMediaObject* chooseRandomItem(Omm::Av::CtlMediaObject* pParentContainer);

    bool                                        _helpRequested;
    MediaRendererGroup                          _mediaRendererGroup;
    MediaServerGroup                            _mediaServerGroup;
    Omm::Controller                             _controller;
    Poco::Thread                                _controllerThread;
    Poco::RunnableAdapter<AvStressController>   _controllerThreadRunnable;
    bool                                        _controllerThreadRunning;
    Poco::FastMutex                             _controllerThreadRunningLock;
};


void
AvStressController::start()
{
    _controller.registerDeviceGroup(&_mediaRendererGroup);
    _controller.registerDeviceGroup(&_mediaServerGroup);
    _controller.init();
    _controller.setState(Omm::DeviceManager::Public);
    _controllerThread.start(_controllerThreadRunnable);
}


void
AvStressController::stop()
{
    _controllerThreadRunningLock.lock();
    _controllerThreadRunning = false;
    _controllerThreadRunningLock.unlock();
    if (controllerThreadRunning() && !_controllerThread.tryJoin(1000)) {
        LOGNS(Omm::Av, upnpav, error, "failed to join controller thread");
    }
    else {
        LOGNS(Omm::Av, upnpav, debug, "controller thread finished.");
    }
    _controller.setState(Omm::DeviceManager::Stopped);
}


void
AvStressController::controllerThread()
{
    _controllerThreadRunning = true;
    LOGNS(Omm::Av, upnpav, debug, "Upnp-AV stress test: controller thread started");

    const int maxPlayTime = 3500;  // msec
    const int waitForDevice = 1000; // msec

    Poco::Random serverNumber;
    serverNumber.seed();
    Poco::Random rendererNumber;
    rendererNumber.seed();
    Poco::Random playTime;
    playTime.seed();

    int test = 1;
    while (controllerThreadRunning()) {
        Omm::DeviceGroup* pServerGroup = _controller.getDeviceGroup(Omm::Av::DeviceType::MEDIA_SERVER_1);
        Omm::DeviceGroup* pRendererGroup = _controller.getDeviceGroup(Omm::Av::DeviceType::MEDIA_RENDERER_1);

        if (pServerGroup == 0 || pRendererGroup == 0 || pServerGroup->getDeviceCount() == 0 || pRendererGroup->getDeviceCount() == 0) {
            LOGNS(Omm::Av, upnpav, debug, "wait for devices ...");
            Poco::Thread::sleep(waitForDevice);
        }
        else {
            LOGNS(Omm::Av, upnpav, debug, "test #" + Poco::NumberFormatter::format(test)\
                + ", servers: " + Poco::NumberFormatter::format(pServerGroup->getDeviceCount())\
                + ", renderers: " + Poco::NumberFormatter::format(pRendererGroup->getDeviceCount()));
            // choose a random server
            int selectedServerNumber = serverNumber.next(pServerGroup->getDeviceCount());

            Omm::Av::CtlMediaServer* pSelectedServer = dynamic_cast<Omm::Av::CtlMediaServer*>(pServerGroup->getDevice(selectedServerNumber));
            if (pSelectedServer) {
                pSelectedServer->browseRootObject();
                Omm::Av::CtlMediaObject* pRootObject = pSelectedServer->getRootObject();
                LOGNS(Omm::Av, upnpav, debug, "server: " + pSelectedServer->getFriendlyName()); // + ", root object: " + pRootObject->getTitle());

                // choose a random item
                Omm::Av::CtlMediaObject* pObject = 0;
                Omm::Av::CtlMediaObject* pParent = 0;
                if (pRootObject && pRootObject->isContainer()) {
                    pObject = chooseRandomItem(pRootObject);
                    pParent = dynamic_cast<Omm::Av::CtlMediaObject*>(pObject->getParent());
                }

                // choose a random renderer
                int selectedRendererNumber = rendererNumber.next(pRendererGroup->getDeviceCount());
                Omm::Av::CtlMediaRenderer* pRenderer = dynamic_cast<Omm::Av::CtlMediaRenderer*>(pRendererGroup->getDevice(selectedRendererNumber));

                // play item on renderer
                if (pRenderer && pObject && pParent) {
                    pRenderer->setObject(pObject, pParent, 0);
                    pRenderer->playPressed();
                    LOGNS(Omm::Av, upnpav, debug, "playing: " + pObject->getTitle() + " on: " + pRenderer->getFriendlyName());
                    Poco::Thread::sleep(maxPlayTime);
                    // we put further stress on the engine without stopping it.
    //                pRenderer->stopPressed();
                }
            }
            test++;
        }
    }
}


bool
AvStressController::controllerThreadRunning()
{
    Poco::ScopedLock<Poco::FastMutex> _lock(_controllerThreadRunningLock);
    return _controllerThreadRunning;
}


int
AvStressController::main(const std::vector<std::string>& args)
{
    if (_helpRequested)
    {
        displayHelp();
    }
    else
    {
        start();
        waitForTerminationRequest();
        stop();
    }
    return Application::EXIT_OK;
}


Omm::Av::CtlMediaObject*
AvStressController::chooseRandomItem(Omm::Av::CtlMediaObject* pParentContainer)
{
    Poco::Random childNumber;
    childNumber.seed();

    if (pParentContainer->getChildCount() == 0) {
        return 0;
    }

    int child = childNumber.next(pParentContainer->getChildCount());
    while (pParentContainer->getChildCount() <= child) {
        pParentContainer->fetchChildren();
    }

    Omm::Av::CtlMediaObject* pObject = dynamic_cast<Omm::Av::CtlMediaObject*>(pParentContainer->getChildForRow(child));
    if (pObject->isContainer()) {
        pObject = chooseRandomItem(pObject);
    }

    return pObject;
}


int
main(int argc, char** argv) {
    AvStressController controller;
    return controller.run(argc, argv);
}
