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

#include <Poco/NumberFormatter.h>
#include <Poco/Util/ServerApplication.h>

#include "ApplicationImpl.h"
#include "Gui/Application.h"
#include "Gui/GuiLogger.h"


namespace Omm {
namespace Gui {


class PocoApplication : public Poco::Util::ServerApplication
{
public:
    int main(const std::vector<std::string>& args)
    {
        waitForTerminationRequest();
        return Application::EXIT_OK;
    }
};


ApplicationImpl::ApplicationImpl(Application* pApplication) :
_pApplication(pApplication)
{
    LOG(gui, debug, "application impl ctor");
}


ApplicationImpl::~ApplicationImpl()
{
}


void
ApplicationImpl::show(bool show)
{
}


void
ApplicationImpl::resize(int width, int height)
{
}


int
ApplicationImpl::width()
{
    return 0;
}


int
ApplicationImpl::height()
{
    return 0;
}


void
ApplicationImpl::setFullscreen(bool fullscreen)
{
}


void
ApplicationImpl::addControlPanel(View* pView)
{
}


void
ApplicationImpl::showControlPanels(bool show)
{
}


int
ApplicationImpl::run(int argc, char** argv)
{
    LOG(gui, debug, "event loop exec ...");
    _pApplication->_pMainView = _pApplication->createMainView();
    _pApplication->createdMainView();
    try {
        _pPocoApplication = &Poco::Util::ServerApplication::instance();
    }
    catch(Poco:: NullPointerException) {
        _pPocoApplication = new PocoApplication();
    }
//    if (_visible) {
//        _pMainWindow->show();
//    }
//    _pMainWindow->resize(_width, _height);
    _pApplication->presentedMainView();
    _pApplication->start();

    int ret = _pPocoApplication->run();

    LOG(gui, debug, "event loop exec finished.");
    _pApplication->finishedEventLoop();
    _pApplication->stop();
    return ret;
}


void
ApplicationImpl::quit()
{
}


}  // namespace Omm
}  // namespace Gui
