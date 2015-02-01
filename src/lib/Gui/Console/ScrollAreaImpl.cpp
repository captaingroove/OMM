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

#include "ColorImpl.h"
#include "ScrollAreaImpl.h"
#include "Gui/ScrollArea.h"
#include "Gui/GuiLogger.h"

namespace Omm {
namespace Gui {


ScrollAreaViewImpl::ScrollAreaViewImpl(View* pView)
{
    _pAreaView = new View;
}


ScrollAreaViewImpl::~ScrollAreaViewImpl()
{
}


View*
ScrollAreaViewImpl::getAreaView()
{
    return _pAreaView;
}


int
ScrollAreaViewImpl::getViewportWidth()
{
    return 0;
}


int
ScrollAreaViewImpl::getViewportHeight()
{
    return 0;
}


int
ScrollAreaViewImpl::getXOffset()
{
    return 0;
}


int
ScrollAreaViewImpl::getYOffset()
{
    return 0;
}


void
ScrollAreaViewImpl::scrollContentsTo(int x, int y)
{
}


void
ScrollAreaViewImpl::showScrollBars(bool show)
{
}


void
ScrollAreaViewImpl::setAreaResizable(bool resize)
{
}


void
ScrollAreaViewImpl::setBackgroundColor(const Color& color)
{
}


void
ScrollAreaViewImpl::addView(View* pView)
{
}


}  // namespace Omm
}  // namespace Gui
