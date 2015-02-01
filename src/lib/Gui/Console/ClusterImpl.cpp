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

#include "ClusterImpl.h"
#include "Gui/Cluster.h"
#include "Gui/GuiLogger.h"
#include "Log.h"

namespace Omm {
namespace Gui {


ClusterViewImpl::ClusterViewImpl(View* pView)
{
}


ClusterViewImpl::~ClusterViewImpl()
{
}


void
ClusterViewImpl::insertView(View* pView, const std::string& label, int index)
{
}


void
ClusterViewImpl::removeView(View* pView)
{
}


void
ClusterViewImpl::showViewAtIndex(View* pView, int index)
{

}


void
ClusterViewImpl::hideView(View* pView)
{

}


std::string
ClusterViewImpl::getConfiguration()
{
    return "";
}


void
ClusterViewImpl::setConfiguration(const std::string& configuration)
{
}


int
ClusterViewImpl::getViewCount()
{
    return 0;
}


int
ClusterViewImpl::getCurrentViewIndex()
{
    return 0;
}


void
ClusterViewImpl::setCurrentViewIndex(int index)
{
}


int
ClusterViewImpl::getIndexFromView(View* pView)
{
    return 0;
}


View*
ClusterViewImpl::getViewFromIndex(int index)
{
    return 0;
}


void
ClusterViewImpl::setHandlesHidden(bool hidden)
{
}


const int
ClusterViewImpl::getHandleHeight()
{
    return 0;
}


}  // namespace Omm
}  // namespace Gui
