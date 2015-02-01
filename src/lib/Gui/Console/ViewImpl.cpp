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

#include "ViewImpl.h"
#include "ColorImpl.h"
#include "Gui/View.h"
#include "Gui/GuiLogger.h"

namespace Omm {
namespace Gui {


ViewImpl::~ViewImpl()
{
}


void
ViewImpl::initViewImpl(View* pView, void* pNative)
{
    _pView = pView;
    _pNativeView = pNative;
}


void
ViewImpl::triggerViewSync()
{
}


View*
ViewImpl::getView()
{
    return _pView;
}


void*
ViewImpl::getNativeView()
{
    return _pNativeView;
}


#ifdef __WINDOWS__
    void*
#else
    uint32_t
#endif
ViewImpl::getNativeWindowId()
{
}


void
ViewImpl::setNativeView(void* pView)
{
    _pNativeView = pView;
}


void
ViewImpl::setParent(View* pView)
{
}


void
ViewImpl::showView(bool async)
{
}


void
ViewImpl::hideView(bool async)
{
}


void
ViewImpl::raise(bool async)
{
}


bool
ViewImpl::isVisible()
{
    return false;
}


int
ViewImpl::posXView()
{
    return 0;
}


int
ViewImpl::posYView()
{
    return 0;
}


int
ViewImpl::widthView()
{
    return 0;
}


int
ViewImpl::heightView()
{
    return 0;
}


void
ViewImpl::resizeView(int width, int height)
{
}


void
ViewImpl::setSizeConstraint(int width, int height, View::SizeConstraint size)
{
}


float
ViewImpl::getFontSize()
{
    return 0.0;
}


void
ViewImpl::setFontSize(float fontSize)
{
}


void
ViewImpl::moveView(int x, int y)
{
}


void
ViewImpl::setHighlighted(bool highlighted)
{
}


void
ViewImpl::setBackgroundColor(const Color& color)
{
}


bool
ViewImpl::getEnableRedraw()
{
    return false;
}


void
ViewImpl::setEnableRedraw(bool enable)
{
}


void
ViewImpl::setAcceptDrops(bool accept)
{
}


int
ViewImpl::getDragMode()
{
    return 0;
}


void
ViewImpl::setEnableHover(bool enable)
{
}


void
ViewImpl::shown()
{
}


void
ViewImpl::hidden()
{
}


void
ViewImpl::resized(int width, int height)
{
}


void
ViewImpl::selected()
{
}


void
ViewImpl::released()
{
}


void
ViewImpl::activated()
{
}


void
ViewImpl::keyPressed(int key)
{
}


bool
ViewImpl::keyPressedNonFullscreen(int key, int modifiers)
{
    return false;
}


Controller::KeyCode
ViewImpl::keyTranslateNativeCode(int key)
{
}


Controller::Modifiers
ViewImpl::keyTranslateNativeModifiers(int modifiers)
{
    int res = Controller::NoModifier;

    return (Controller::Modifiers)res;
}


void
ViewImpl::mouseHovered(const Position& pos)
{
}


void
ViewImpl::mouseMoved(const Position& pos)
{
}


void
ViewImpl::dragStarted()
{
}


void
ViewImpl::dragEntered(const Position& pos, Drag* pDrag, bool& accept)
{
}


void
ViewImpl::dragMoved(const Position& pos, Drag* pDrag, bool& accept)
{
}


void
ViewImpl::dragLeft()
{
}


void
ViewImpl::dropped(const Position& pos, Drag* pDrag, bool& accept)
{
}


PlainViewImpl::PlainViewImpl(View* pView)
{
    initViewImpl(pView, 0);
}


}  // namespace Omm
}  // namespace Gui
