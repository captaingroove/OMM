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

#include "ImageImpl.h"
#include "Gui/Image.h"
#include "Gui/GuiLogger.h"

namespace Omm {
namespace Gui {


ImageModelImpl::ImageModelImpl(Model* pModel) :
ModelImpl(pModel)
{
}


ImageModelImpl::~ImageModelImpl()
{
}


void
ImageModelImpl::setData(const std::string& data)
{
    _pData = &data;
}


ImageViewImpl::ImageViewImpl(View* pView)
{
}


ImageViewImpl::~ImageViewImpl()
{
}


void
ImageViewImpl::syncViewImpl()
{
}


void
ImageViewImpl::setAlignment(View::Alignment alignment)
{
}


void
ImageViewImpl::scaleBestFit(int width, int height)
{
}


}  // namespace Omm
}  // namespace Gui
