/***************************************************************************|
|  OMM - Open Multimedia                                                    |
|                                                                           |
|  Copyright (C) 2009, 2010                                                 |
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
#include <fstream>
#include <sstream>

#include <Poco/ClassLibrary.h>
#include <Poco/Net/HTTPIOStream.h>
#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/DOM/DOMParser.h>
#include <Poco/DOM/Document.h>
#include <Poco/File.h>

#include <Omm/UpnpAvObject.h>
#include <Omm/Web/WebRadio.h>

#include "WebradioServer.h"


WebradioModel::WebradioModel()
{
    _pWebRadio = new Omm::Web::DirbleWebRadio;
//    _pWebRadio = new Omm::Web::WebRadio;
}


void
WebradioModel::init()
{
    std::ifstream stationList(getBasePath().c_str());
    _pWebRadio->readXml(stationList);
}


std::string
WebradioModel::getModelClass()
{
    return "WebradioModel";
}


Poco::UInt64
WebradioModel::getSystemUpdateId(bool checkMod)
{
    Poco::File stationListFile(getBasePath());
    if (stationListFile.exists()) {
        return stationListFile.getLastModified().epochTime();
    }
    else {
        return 0;
    }
}


void
WebradioModel::scan()
{
    std::ifstream stationList(getBasePath().c_str());
    _pWebRadio->readXml(stationList);

    for (Omm::Web::WebRadio::StationIterator it = _pWebRadio->stationBegin(); it != _pWebRadio->stationEnd(); ++it) {
        addPath(it->second->getName());
    }
}


void
WebradioModel::scanDeep()
{
    LOGNS(Omm::Av, upnpav, debug, "webradio model deep scan ...");

    _pWebRadio->scanStationList();
    std::ofstream stationList(getBasePath().c_str());
    _pWebRadio->writeXml(stationList);

    LOGNS(Omm::Av, upnpav, debug, "webradio model deep scan finished.");
}


std::string
WebradioModel::getClass(const std::string& path)
{
    return Omm::Av::AvClass::className(Omm::Av::AvClass::ITEM, Omm::Av::AvClass::AUDIO_BROADCAST);
}


std::string
WebradioModel::getTitle(const std::string& path)
{
    return path;
}


bool
WebradioModel::isSeekable(const std::string& path, const std::string& resourcePath)
{
    return false;
}


std::istream*
WebradioModel::getStream(const std::string& path, const std::string& resourcePath)
{
    Omm::Web::Station* pStation = _pWebRadio->getStation(path);
    if (pStation) {
        return _pWebRadio->getStream(pStation->getUri());
    }
    else {
        return 0;
    }
}


void
WebradioModel::freeStream(std::istream* pIstream)
{
    LOGNS(Omm::Av, upnpav, debug, "deleting webradio stream");

    _pWebRadio->freeStream(pIstream);
}


std::string
WebradioModel::getMime(const std::string& path)
{
    return "audio/mpeg";
}


std::string
WebradioModel::getDlna(const std::string& path)
{
    return "DLNA.ORG_PN=MP3;DLNA.ORG_OP=01";
}


#ifdef OMMPLUGIN
POCO_BEGIN_MANIFEST(Omm::Av::AbstractDataModel)
POCO_EXPORT_CLASS(WebradioModel)
POCO_END_MANIFEST
#endif
