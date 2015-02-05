/***************************************************************************|
|  OMM - Open Multimedia                                                    |
|                                                                           |
|  Copyright (C) 2013                                                       |
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

#ifndef WebSetup_INCLUDED
#define WebSetup_INCLUDED

#include <Poco/Net/HTMLForm.h>

namespace Omm {

class UpnpApplication;


class WebSetup
{
public:
    WebSetup(UpnpApplication* pApp);

    std::stringstream* generateConfigPage();
    void handleAppConfigRequest(const Poco::Net::HTMLForm& form);
    void handleDevConfigRequest(const Poco::Net::HTMLForm& form);

private:
    UpnpApplication*    _pApp;
};

}  // namespace Omm


#endif
