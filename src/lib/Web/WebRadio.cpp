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

#include <fstream>
#include <sstream>
#include <cstring>
#include <map>

#include <Poco/File.h>
#include <Poco/Glob.h>
#include <Poco/StreamCopier.h>
#include <Poco/NumberParser.h>
#include <Poco/NumberFormatter.h>
#include <Poco/DOM/AbstractContainerNode.h>
#include <Poco/DOM/DOMException.h>
#include <Poco/DOM/DOMParser.h>
#include <Poco/DOM/DOMWriter.h>
#include <Poco/XML/XMLWriter.h>
#include <Poco/DOM/NodeIterator.h>
#include <Poco/DOM/NodeFilter.h>
#include <Poco/DOM/NodeList.h>
#include <Poco/DOM/NamedNodeMap.h>
#include <Poco/DOM/AttrMap.h>
#include <Poco/DOM/Element.h>
#include <Poco/DOM/Attr.h>
#include <Poco/DOM/Text.h>
#include <Poco/DOM/AutoPtr.h>
#include <Poco/DOM/DocumentFragment.h>
#include <Poco/SAX/InputSource.h>
#include <Poco/URI.h>
#include <Poco/Net/HTTPIOStream.h>
#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/StringTokenizer.h>
#include <Poco/Thread.h>

#include "Log.h"
#include "WebLogger.h"
#include "WebRadio.h"


namespace Omm {
namespace Web {


void
Station::readXml(Poco::XML::Node* pXmlStation)
{
    Poco::XML::Node* pProp = pXmlStation->firstChild();
    while (pProp) {
        if (pProp->nodeName() == "name") {
            _name = pProp->innerText();
        }
        else if (pProp->nodeName() == "uri") {
            _uri = pProp->innerText();
        }
        else if (pProp->nodeName() == "webpage") {
            _webpage = pProp->innerText();
        }
        else if (pProp->nodeName() == "icon") {
            _icon = pProp->innerText();
        }
        else {
            LOG(web, warning, "webradio station list entry has unknown property: " + pProp->nodeName());
        }
        pProp = pProp->nextSibling();
    }
    LOG(web, debug, "added web radio station with name: " + _name + ", uri: " + _uri);
}


void
Station::writeXml(Poco::XML::Element* pWebradio)
{
    Poco::XML::Document* pDoc = pWebradio->ownerDocument();
    Poco::AutoPtr<Poco::XML::Element> pStation = pDoc->createElement("station");
    pWebradio->appendChild(pStation);

    Poco::AutoPtr<Poco::XML::Element> pName = pDoc->createElement("name");
    Poco::AutoPtr<Poco::XML::Text> pNameVal = pDoc->createTextNode(_name);
    pName->appendChild(pNameVal);
    pStation->appendChild(pName);

    Poco::AutoPtr<Poco::XML::Element> pUri = pDoc->createElement("uri");
    Poco::AutoPtr<Poco::XML::Text> pUriVal = pDoc->createTextNode(_uri);
    pUri->appendChild(pUriVal);
    pStation->appendChild(pUri);

    if (_webpage != "") {
        Poco::AutoPtr<Poco::XML::Element> pWebpage = pDoc->createElement("webpage");
        Poco::AutoPtr<Poco::XML::Text> pWebpageVal = pDoc->createTextNode(_webpage);
        pWebpage->appendChild(pWebpageVal);
        pStation->appendChild(pWebpage);
    }

    if (_icon != "") {
        Poco::AutoPtr<Poco::XML::Element> pIcon = pDoc->createElement("icon");
        Poco::AutoPtr<Poco::XML::Text> pIconVal = pDoc->createTextNode(_icon);
        pIcon->appendChild(pIconVal);
        pStation->appendChild(pIcon);
    }

    LOG(web, debug, "wrote web radio station with name: " + _name);
}


std::string
Station::getName()
{
    return _name;
}


std::string
Station::getUri()
{
    return _uri;
}


WebRadio::WebRadio()
{
}


WebRadio::~WebRadio()
{
    for(std::map<std::string, Station*>::iterator it = _stationMap.begin(); it != _stationMap.end(); ++it) {
        delete it->second;
    }
}


WebRadio::StationIterator
WebRadio::stationBegin()
{
    return _stationMap.begin();
}


WebRadio::StationIterator
WebRadio::stationEnd()
{
    return _stationMap.end();
}


void
WebRadio::readXml(std::istream& istream)
{
    LOG(web, debug, "web radio, read xml ...");

    Poco::AutoPtr<Poco::XML::Document> pXmlDoc = new Poco::XML::Document;
    Poco::XML::InputSource xmlFile(istream);

    Poco::XML::DOMParser parser;
#if (POCO_VERSION & 0xFFFFFFFF) < 0x01040000
    parser.setFeature(Poco::XML::DOMParser::FEATURE_WHITESPACE, false);
#else
    parser.setFeature(Poco::XML::DOMParser::FEATURE_FILTER_WHITESPACE, true);
#endif
    try {
        pXmlDoc = parser.parse(&xmlFile);
    }
    catch (Poco::Exception& e) {
        LOG(web, error, "parsing webradio station list failed: " + e.displayText());
        return;
    }

    Poco::XML::Node* pWebradio = pXmlDoc->documentElement();
    if (!pWebradio || pWebradio->nodeName() != "stationlist") {
        LOG(web, error, "error reading webradio station list, wrong file format");
        return;
    }
    if (pWebradio->hasChildNodes()) {
        Poco::XML::Node* pXmlStation = pWebradio->firstChild();
        while (pXmlStation && pXmlStation->nodeName() == "station") {
            Station* pStation = new Station;
            pStation->readXml(pXmlStation);
            if (!addStation(pStation)) {
                delete pStation;
            }

            pXmlStation = pXmlStation->nextSibling();
        }
    }
    else {
        LOG(web, error, "webradio station list contains no stations");
        return;
    }

    LOG(web, debug, "web radio, read xml finished.");
}


void
WebRadio::writeXml(std::ostream& ostream)
{
    LOG(web, debug, "web radio, write xml ...");
    Poco::AutoPtr<Poco::XML::Document> pXmlDoc = new Poco::XML::Document;

    Poco::XML::DOMWriter writer;
    writer.setOptions(Poco::XML::XMLWriter::WRITE_XML_DECLARATION | Poco::XML::XMLWriter::PRETTY_PRINT);

    Poco::XML::Element* pWebradio = pXmlDoc->createElement("stationlist");
    pXmlDoc->appendChild(pWebradio);
    try {
        for (std::map<std::string, Station*>::iterator it = stationBegin(); it != stationEnd(); ++it) {
            it->second->writeXml(pWebradio);
        }
        writer.writeNode(ostream, pXmlDoc);
    }
    catch (Poco::Exception& e) {
        LOG(web, error, "writing webradio description failed: " + e.displayText());
    }
    LOG(web, debug, "web radio, wrote xml.");
}


Station*
WebRadio::getStation(const std::string& stationName)
{
    return _stationMap[stationName];
}


std::istream*
WebRadio::getStream(Station* pStation)
{
    LOG(web, debug, "get stream of station: " + pStation->getName());

    return getStream(pStation->getUri());
}


void
WebRadio::freeStream(std::istream* pIstream)
{
    LOG(web, debug, "free stream ...");

    delete pIstream;

    LOG(web, debug, "free stream finished.");
}


bool
WebRadio::addStation(Station* pStation)
{
    std::pair<StationIterator, bool> ret = _stationMap.insert(std::make_pair(pStation->getName(), pStation));
    return ret.second;
}


std::istream*
WebRadio::getStream(const std::string& uri)
{
    Poco::URI streamUri(uri);

    Poco::Net::HTTPClientSession* pSession = new Poco::Net::HTTPClientSession(streamUri.getHost(), streamUri.getPort());
    std::string path = streamUri.getPath();
    if (path == "") {
        path = "/";
    }
    Poco::Net::HTTPRequest request("GET", path);
    pSession->sendRequest(request);
    std::stringstream requestHeader;
    request.write(requestHeader);
    LOG(web, debug, "proxy request header:\n" + requestHeader.str());

    Poco::Net::HTTPResponse response;
    std::istream& istr = pSession->receiveResponse(response);
    if (response.getStatus() == Poco::Net::HTTPResponse::HTTP_NOT_FOUND ||
        response.getStatus() == Poco::Net::HTTPResponse::HTTP_NO_CONTENT) {
        LOG(web, error, "web radio stream not available");
        delete pSession;
        return 0;
    }
    else if (response.getStatus() == Poco::Net::HTTPResponse::HTTP_SEE_OTHER ||
             response.getStatus() == Poco::Net::HTTPResponse::HTTP_FOUND ||
             response.getStatus() == Poco::Net::HTTPResponse::HTTP_MOVED_PERMANENTLY ||
             response.getStatus() == Poco::Net::HTTPResponse::HTTP_TEMPORARY_REDIRECT) {
        LOG(web, information, "web radio uri redirected to: " + response.get("Location"));
        delete pSession;
        return getStream(response.get("Location"));
    }

    if (istr.peek() == EOF) {
        LOG(web, error, "web radio failed to read data from stream uri: " + uri);
        return 0;
    }
    else {
        LOG(web, debug, "web radio success reading data from stream uri: " + uri);
    }

    LOG(web, information, "HTTP " + Poco::NumberFormatter::format(response.getStatus()) + " " + response.getReason());
    std::stringstream responseHeader;
    response.write(responseHeader);
    LOG(web, debug, "proxy response header:\n" + responseHeader.str());

    if (response.getContentType() == "audio/mpeg" || response.getContentType() == "application/ogg" || response.getContentType() == "audio/aac") {
        LOG(web, debug, "web radio detected audio content, streaming directly ...");
        return new Poco::Net::HTTPResponseStream(istr, pSession);
    }
    else if (response.getContentType() == "audio/x-scpls" || response.getContentType() == "audio/x-mpegurl") {
        std::vector<std::string> uris;
        // look for streamable URIs in the downloaded playlist
        LOG(web, debug, "web radio detected playlist, analyzing ...");
        std::string line;
        while (getline(istr, line)) {
            LOG(web, debug, line);
            std::string::size_type uriPos = line.find("http://");
            if (uriPos != std::string::npos) {
                std::string uri = Poco::trimRight(line.substr(uriPos));
                LOG(web, debug, "web radio found stream uri: " + uri);
                uris.push_back(uri);
            }
        }
        // try to stream one of the URIs in the downloaded playlist
        for (std::vector<std::string>::iterator it = uris.begin(); it != uris.end(); ++it) {
            delete pSession;
            std::istream* pStream = getStream(*it);
            if (pStream) {
                return pStream;
            }
        }
    }
    else {
        LOG(web, error, "web radio unknown stream type (no audio or playlist)");
    }
    return 0;
}


void
DirbleStation::scanStation(Poco::XML::Node* pXmlStation)
{
    Poco::XML::Node* pProp = pXmlStation->firstChild();
    while (pProp) {
        if (pProp->nodeName() == "name") {
            _name = pProp->innerText();
        }
        else if (pProp->nodeName() == "streamurl") {
            _uri = pProp->innerText();
        }
        else if (pProp->nodeName() == "website") {
            _webpage = pProp->innerText();
        }
        else if (pProp->nodeName() == "id") {
            _id = pProp->innerText();
        }
        else if (pProp->nodeName() == "country") {
            _country = pProp->innerText();
        }
        else if (pProp->nodeName() == "bitrate") {
            _bitrate = pProp->innerText();
        }
        else if (pProp->nodeName() == "status") {
            _status = pProp->innerText();
        }
        else {
            LOG(web, warning, "webradio station list entry has unknown property: " + pProp->nodeName());
        }
        pProp = pProp->nextSibling();
    }
    LOG(web, debug, "added dirble web radio station with name: " + _name + ", uri: " + _uri);
}


DirbleWebRadio::DirbleWebRadio():
_apiKey("5b8be71b13370ba1ade03b1801e8fe3dcd152dcf"),
_server("http://api.dirble.com"),
_baseUri("/v1"),
_requestUri("/country/apikey/" + _apiKey + "/country/de"),
_formatUri("/format/xml")
{
}


void
DirbleWebRadio::scanStationList()
{
    std::string uri = _server + _baseUri + _requestUri + _formatUri;
    Poco::URI streamUri(uri);

    Poco::Net::HTTPClientSession* pSession = new Poco::Net::HTTPClientSession(streamUri.getHost(), streamUri.getPort());
    std::string path = streamUri.getPath();
    if (path == "") {
        path = "/";
    }
    Poco::Net::HTTPRequest request("GET", path);
    pSession->sendRequest(request);
    std::stringstream requestHeader;
    request.write(requestHeader);
    LOG(web, debug, "dirble request header:\n" + requestHeader.str());

    Poco::Net::HTTPResponse response;
    std::istream& istr = pSession->receiveResponse(response);

    if (response.getStatus() == Poco::Net::HTTPResponse::HTTP_NOT_FOUND ||
        response.getStatus() == Poco::Net::HTTPResponse::HTTP_NO_CONTENT) {
        LOG(web, error, "dirble station list not available");
    }
    LOG(web, information, "HTTP " + Poco::NumberFormatter::format(response.getStatus()) + " " + response.getReason());
    std::stringstream responseHeader;
    response.write(responseHeader);
    LOG(web, debug, "response header:\n" + responseHeader.str());

    readStationList(istr);
}


void
DirbleWebRadio::readStationList(std::istream& istream)
{
    LOG(web, debug, "dirble web radio, read xml ...");

    Poco::AutoPtr<Poco::XML::Document> pXmlDoc = new Poco::XML::Document;
    Poco::XML::InputSource xmlFile(istream);

    Poco::XML::DOMParser parser;
#if (POCO_VERSION & 0xFFFFFFFF) < 0x01040000
    parser.setFeature(Poco::XML::DOMParser::FEATURE_WHITESPACE, false);
#else
    parser.setFeature(Poco::XML::DOMParser::FEATURE_FILTER_WHITESPACE, true);
#endif
    try {
        pXmlDoc = parser.parse(&xmlFile);
    }
    catch (Poco::Exception& e) {
        LOG(web, error, "parsing dirble webradio station list failed: " + e.displayText());
        return;
    }

    Poco::XML::Node* pWebradio = pXmlDoc->documentElement();
    if (!pWebradio || pWebradio->nodeName() != "xml") {
        LOG(web, error, "error reading dirble webradio station list, wrong file format");
        return;
    }
    if (pWebradio->hasChildNodes()) {
        Poco::XML::Node* pXmlStation = pWebradio->firstChild();
        while (pXmlStation && pXmlStation->nodeName() == "item") {
            DirbleStation* pStation = new DirbleStation;
            pStation->scanStation(pXmlStation);
            if (pStation->getName() == "" || pStation->getUri() == "" || !addStation(pStation)) {
                delete pStation;
            }

            pXmlStation = pXmlStation->nextSibling();
        }
    }
    else {
        LOG(web, error, "dirble webradio station list contains no stations");
        return;
    }

    LOG(web, debug, "dirble web radio, read xml finished.");
}

}  // namespace Omm
}  // namespace Web
