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

#include "NavigatorImpl.h"
#include "Gui/Navigator.h"
#include "Gui/GuiLogger.h"
#include "Log.h"


namespace Omm {
namespace Gui {

NavigatorViewImpl::NavigatorViewImpl(View* pView)
{
}


NavigatorViewImpl::~NavigatorViewImpl()
{
}


void
NavigatorViewImpl::pushView(View* pView, const std::string label)
{
}


void
NavigatorViewImpl::popView(bool keepRootView)
{
}


void
NavigatorViewImpl::popToRootView()
{
}


View*
NavigatorViewImpl::getVisibleView()
{
    return 0;
}


int
NavigatorViewImpl::viewCount()
{
    return 0;
}


void
NavigatorViewImpl::showNavigatorBar(bool show)
{
}


void
NavigatorViewImpl::showSearchBox(bool show)
{
}


void
NavigatorViewImpl::clearSearchText()
{
}


void
NavigatorViewImpl::showRightButton(bool show)
{
}


void
NavigatorViewImpl::setRightButtonLabel(const std::string& label)
{
}


void
NavigatorViewImpl::setRightButtonColor(const Color& color)
{
}


void
NavigatorViewImpl::removeView(View* pView)
{
}


void
NavigatorViewImpl::exposeView(View* pView)
{
}


void
NavigatorViewImpl::changedSearchText(const std::string& searchText)
{
}


void
NavigatorViewImpl::poppedToRoot()
{
}


void
NavigatorViewImpl::poppedToView(View* pView)
{
}


void
NavigatorViewImpl::rightButtonPushed()
{
}

} // namespace Gui
} // namespace Omm
