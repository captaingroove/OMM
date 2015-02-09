/***************************************************************************|
|  OMM - Open Multimedia                                                    |
|                                                                           |
|  Copyright (C) 2015                                                       |
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

#include <Poco/StringTokenizer.h>

#include <Omm/Web/WebRadio.h>


int
main(int argc, char** argv)
{
    Omm::Web::DirbleWebRadio webradio;

    webradio.scanStationList();
    webradio.writeXml(std::cout);

//    for (int i = 1; i < argc; ++i) {
//        Poco::StringTokenizer initialTransponders(argv[i], "/");
//        if (initialTransponders.count() != 2) {
//            std::cerr << "usage: scandvb <frontend-type1>/<transponder-list1> <frontend-type2>/<transponder-list2> ... " << std::endl;
//            return 1;
//        }
//        pDevice->addInitialTransponders(initialTransponders[0], initialTransponders[1]);
//    }

    return 0;
}
