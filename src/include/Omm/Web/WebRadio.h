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

#ifndef WebRadio_INCLUDED
#define WebRadio_INCLUDED

#include <cstring>
#include <string>
#include <vector>
#include <map>
#include <set>

#ifdef POCO_VERSION_HEADER_FOUND
#include <Poco/Version.h>
#endif
#include <Poco/Timestamp.h>
#include <Poco/Thread.h>
#include <Poco/RunnableAdapter.h>
#include <Poco/Mutex.h>
#include <Poco/Logger.h>
#include <Poco/Format.h>
#include <Poco/StringTokenizer.h>
#include <Poco/DOM/Document.h>
#include "Poco/Notification.h"

namespace Omm {
namespace Web {


class Station
{
public:
    virtual void scanStation(Poco::XML::Node* pXmlStation) {}

    void readXml(Poco::XML::Node* pXmlStation);
    void writeXml(Poco::XML::Element* pWebradio);

    std::string getName();
    std::string getUri();

protected:
    std::string     _name;
    std::string     _uri;
    std::string     _webpage;
    std::string     _icon;
};


class WebRadio
{
public:
    WebRadio();
    ~WebRadio();

    typedef std::map<std::string, Station* >::iterator StationIterator;
    StationIterator stationBegin();
    StationIterator stationEnd();

    virtual void scanStationList() {}

    void readXml(std::istream& istream);
    void writeXml(std::ostream& ostream);

    Station* getStation(const std::string& stationName);

    std::istream* getStream(Station* pStation);
    std::istream* getStream(const std::string& uri);
    void freeStream(std::istream* pIstream);

protected:
    bool addStation(Station* pStation);

private:
    std::map<std::string, Station*>     _stationMap;
};


class DirbleStation : public Station
{
public:
    virtual void scanStation(Poco::XML::Node* pXmlStation);

private:
    std::string     _id;
    std::string     _country;
    std::string     _bitrate;
    std::string     _status;
};


class DirbleWebRadio : public WebRadio
{
public:
    DirbleWebRadio();

    virtual void scanStationList();

private:
    void readStationList(std::istream& istream);

    const std::string       _apiKey;
    const std::string       _server;
    const std::string       _baseUri;
    const std::string       _requestUri;
    const std::string       _formatUri;
};


}  // namespace Omm
}  // namespace Web

#endif
