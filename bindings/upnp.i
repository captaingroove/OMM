/***************************************************************************|
|  OMM - Open Multimedia                                                    |
|                                                                           |
|  Copyright (C) 2016                                                       |
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

%module upnp
%module(directors="1") upnp

%feature("director");
//%feature("director") Controller;
%feature("nodirector") CtlMediaServerGroup::addDevice;
%feature("nodirector") CtlMediaServerGroup::removeDevice;
%feature("nodirector") CtlMediaRendererGroup::addDevice;
%feature("nodirector") CtlMediaRendererGroup::removeDevice;

%include "std_string.i"
%include "std_vector.i"
%include "std_map.i"
%include "typemaps.i"
%{
/* Includes the header in the wrapper code */
#include <Poco/Types.h>
#include <Poco/URI.h>
#include <Poco/Timestamp.h>
#include "../src/include/Omm/Log.h"
#include "../src/include/Omm/Upnp.h"
#include "../src/include/Omm/UpnpInternal.h"
#include "../src/include/Omm/UpnpAv.h"
#include "../src/include/Omm/UpnpAvCtlObject.h"
#include "../src/include/Omm/UpnpAvCtlRenderer.h"
#include "../src/include/Omm/UpnpAvCtlServer.h"
#include "../src/include/Omm/UpnpAvObject.h"
#include "../src/include/Omm/UpnpAvRenderer.h"
#include "../src/include/Omm/UpnpAvServer.h"

using namespace Omm;
using namespace Omm::Av;
using namespace Poco;
%}

%rename(Log) Omm::Util::Log;
%rename(Log_Av) Omm::Av::Log;
%rename(Log_Omm) Omm::Log;

/* Parse the header file to generate wrappers */
//%include "../src/include/Omm/Log.h"
//%include "../src/include/Omm/Upnp.h"
//%include "../src/include/Omm/UpnpInternal.h"
//%include "../src/include/Omm/UpnpAv.h"
//%include "../src/include/Omm/UpnpAvCtlRenderer.h"
//%include "../src/include/Omm/UpnpAvCtlServer.h"
//%include "../src/include/Omm/UpnpAvObject.h"
//%include "../src/include/Omm/UpnpAvRenderer.h"
//%include "../src/include/Omm/UpnpAvServer.h"

%include <Poco/Types.h>
//%include <Poco/URI.h>
//%include <Poco/Timestamp.h>

//typedef Poco::UInt8     ui1;
//typedef Poco::UInt16    ui2;
////typedef Poco::UInt32    ui4;
//typedef Poco::Int8      i1;
//typedef Poco::Int16     i2;
//typedef Poco::Int32     i4;
//typedef float           r4;
//typedef double          r8;
//typedef r8              number;
//typedef Poco::URI       uri;
//typedef Poco::Timestamp date;
//typedef Poco::Timestamp dateTime;
//typedef Poco::Timestamp time;

//%apply int& INOUT { ui1& };
//%apply int& INOUT { ui2& };
//%apply int& INOUT { ui4& };
//%apply int& INOUT { i1& };
//%apply int& INOUT { i2& };
//%apply int& INOUT { i4& };

//%apply int INOUT { ui4 };

typedef unsigned short  ui1;
typedef unsigned short  ui2;
typedef unsigned int    ui4;
typedef short           i1;
typedef short           i2;
typedef int             i4;
typedef float           r4;
typedef double          r8;
typedef double          number;


//////////////////////////////////// Log ////////////////////////////////////


namespace Omm {
namespace Util {

class Log
{
public:
    static Log* instance();
    static void logToFile(const std::string& fileName);
    Poco::Channel* channel();

    Poco::Logger& util();
    Poco::Logger& plugin();

private:
    Log(Poco::Channel* pChannel = 0);

    static Log*                         _pInstance;
    Poco::FormattingChannel*            _pChannel;
    Poco::Logger*                       _pUtilLogger;
    Poco::Logger*                       _pPluginLogger;
};

}
}


//////////////////////////////////// Upnp ////////////////////////////////////


template<class E>
class Container
{
public:
    typedef typename std::map<std::string, E*>::iterator KeyIterator;
    typedef typename std::vector<E*>::iterator Iterator;
    typedef typename std::vector<E*>::const_iterator ConstIterator;
    typedef typename std::vector<std::string>::iterator PosIterator;

    E& get(std::string key);
    E& get(int index) const;
    void append(std::string key, E* pElement);
    int size() const;
    int position(const std::string& key);
    bool contains(const std::string& key);
    void remove(std::string key);
    void replace(const std::string& key, E* pElement);
    template<typename T> T getValue(const std::string& key) const;
    template<typename T> void setValue(const std::string& key, const T& val);
    Iterator begin();
    Iterator end();
    KeyIterator beginKey();
    KeyIterator endKey();
    void clear();
    void deepClear();
};

%template(DeviceContainerT) Container<Device>;
%template(DeviceContainerContainerT) Container<DeviceContainer>;

class DeviceManager
{
public:
    typedef std::string State;
    static const std::string Transitioning;
    static const std::string Stopped;
    static const std::string Local;
    static const std::string Public;
    static const std::string PublicLocal;

    DeviceManager(Socket* pNetworkListener);
    virtual ~DeviceManager();

    typedef Container<DeviceContainer>::Iterator DeviceContainerIterator;
    DeviceContainerIterator beginDeviceContainer();
    DeviceContainerIterator endDeviceContainer();

    int getDeviceContainerCount();

    void init();
    void setState(State newState);
};


class Controller : public DeviceManager
{
public:
    Controller();
    virtual ~Controller();

    void setState(State newState);
    void registerDeviceGroup(DeviceGroup* pDeviceGroup);
    DeviceGroup* getDeviceGroup(const std::string& deviceType);
    void sendMSearch(const std::string& searchTarget = SearchTargetRootDevice);

    virtual void signalNetworkActivity(bool on) {}

    void setFeatureSubscribeEventing(bool on = true) { _featureSubscribeToEvents = on; }

protected:
    virtual void addDeviceContainer(DeviceContainer* pDeviceContainer, int index, bool begin) {}
    virtual void removeDeviceContainer(DeviceContainer* pDeviceContainer, int index, bool begin) {}
};


class Device
{
public:
    Device();
    virtual ~Device();

    void deepDelete();

    typedef Container<Service>::Iterator ServiceIterator;
    ServiceIterator beginService();
    ServiceIterator endService();

    typedef Container<std::string>::KeyIterator PropertyIterator;
    PropertyIterator beginProperty();
    PropertyIterator endProperty();

    typedef std::vector<Icon*>::iterator IconIterator;
    IconIterator beginIcon();
    IconIterator endIcon();

    DeviceContainer* getDeviceContainer() const;
    std::string getUuid() const;
    std::string getDeviceTypeFullString() const;
    Service* getServiceForTypeFullString(std::string serviceType);
    const std::string& getFriendlyName();
    const std::string& getProperty(const std::string& name);

    void setDeviceContainer(DeviceContainer* pDeviceContainer);
    void setDeviceData(DeviceData* pDeviceData);
    void setDevDeviceCode(DevDeviceCode* pDevDevice);
    void setCtlDeviceCode(CtlDeviceCode* pCtlDevice);
    void setUuid(std::string uuid);
    void setRandomUuid();
    void setProperty(const std::string& name, const std::string& val);
    void setFriendlyName(const std::string& name);

    void addIcon(Icon* pIcon);

    void initStateVars();
    void controllerSubscribeEventing();
    void controllerUnsubscribeEventing();

    void stopSubscriptionTimer();

    virtual void start() {}
    virtual void stop() {}
};


class DeviceServer : public DeviceManager
{
public:
    DeviceServer();
    virtual ~DeviceServer();

    void init();
    void setState(State newState);
};


class DeviceContainer
{
public:
    DeviceContainer();
    ~DeviceContainer();

    typedef Container<Device>::Iterator DeviceIterator;
    DeviceIterator beginDevice();
    DeviceIterator endDevice();

    typedef std::map<std::string,Service*>::iterator ServiceTypeIterator;
    ServiceTypeIterator beginServiceType();
    ServiceTypeIterator endServiceType();

    void addDevice(Device* pDevice);
    void clear();

    DeviceManager* getDeviceManager();
    /*const*/ Device* getDevice(std::string uuid) /*const*/;
    int getDeviceCount();
    Device* getRootDevice() const;
    Controller* getController() const;

    void setRootDevice(Device* pDevice);
};


class DeviceGroup
{
public:
    DeviceGroup(const std::string& deviceType, const std::string& shortName);
    DeviceGroup(DeviceGroupDelegate* pDeviceGroupDelegate);
    virtual ~DeviceGroup() {}

    int getDeviceCount() const;
    Device* getDevice(int index) const;
    Device* getDevice(const std::string& uuid);
    Controller* getController() const;
    void selectDevice(Device* pDevice);
    Device* getSelectedDevice() const;
    void clearDevices();

    virtual std::string getDeviceType();
    virtual std::string shortName();
    virtual Icon* groupIcon();

    virtual Device* createDevice();
    /// factory method to create a device of a certain type.
    virtual void showDeviceGroup() {}

protected:
    virtual void addDevice(Device* pDevice, int index, bool begin) {}
    virtual void removeDevice(Device* pDevice, int index, bool begin) {}
    virtual void selectDevice(Device* pDevice, int index) {}
    virtual void addDeviceContainer(DeviceContainer* pDeviceContainer, int index, bool begin) {}
    virtual void removeDeviceContainer(DeviceContainer* pDeviceContainer, int index, bool begin) {}
};


class DeviceGroupDelegate
{
public:
    DeviceGroupDelegate();
    virtual ~DeviceGroupDelegate() {}

    virtual std::string getDeviceType() { return ""; }
    virtual std::string shortName() { return ""; }
};


//////////////////////////////////// UpnpAv ////////////////////////////////////

class AvClass
{
public:
    static const std::string OBJECT;
    static const std::string ITEM;
    static const std::string IMAGE_ITEM;
    static const std::string AUDIO_ITEM;
    static const std::string VIDEO_ITEM;
    static const std::string PLAYLIST_ITEM;
    static const std::string TEXT_ITEM;
    static const std::string PHOTO;
    static const std::string MUSIC_TRACK;
    static const std::string AUDIO_BROADCAST;
    static const std::string AUDIO_BOOK;
    static const std::string MOVIE;
    static const std::string VIDEO_BROADCAST;
    static const std::string MUSIC_VIDEO_CLIP;
    static const std::string CONTAINER;
    static const std::string PERSON;
    static const std::string PLAYLIST_CONTAINER;
    static const std::string ALBUM;
    static const std::string GENRE;
    static const std::string STORAGE_SYSTEM;
    static const std::string STORAGE_VOLUME;
    static const std::string STORAGE_FOLDER;
    static const std::string MUSIC_ARTIST;
    static const std::string MUSIC_ALBUM;
    static const std::string PHOTO_ALBUM;
    static const std::string MUSIC_GENRE;
    static const std::string MOVIE_GENRE;

    static std::string className(const std::string& c1 = "",
        const std::string& c2 = "",
        const std::string& c3 = "",
        const std::string& c4 = "");
    static bool matchClass(const std::string& name,
        const std::string& c1 = "",
        const std::string& c2 = "",
        const std::string& c3 = "",
        const std::string& c4 = "");
};


class AvProperty
{
public:
    static const std::string ID;
    static const std::string TITLE;
    static const std::string CREATOR;
    static const std::string RES;
    static const std::string CLASS;
    static const std::string CLASS_NAME;
    static const std::string CONTAINER_SEARCHABLE;
    static const std::string SEARCH_CLASS;
    static const std::string SEARCH_CLASS_INCLUDE_DERIVED;
    static const std::string SEARCH_CLASS_NAME;
    static const std::string CREATE_CLASS;
    static const std::string CREATE_CLASS_INCLUDE_DERIVED;
    static const std::string CREATE_CLASS_NAME;
    static const std::string PARENT_ID;
    static const std::string REF_ID;
    static const std::string RESTRICTED;
    static const std::string WRITE_STATUS;
    static const std::string CHILD_COUNT;
    static const std::string ARTIST;
    static const std::string ARTIST_ROLE;
    static const std::string ACTOR;
    static const std::string ACTOR_ROLE;
    static const std::string AUTHOR;
    static const std::string AUTHOR_ROLE;
    static const std::string PRODUCER;
    static const std::string DIRECTOR;
    static const std::string PUBLISHER;
    static const std::string CONTRIBUTOR;
    static const std::string GENRE;
    static const std::string ALBUM;
    static const std::string PLAYLIST;
    static const std::string SIZE;
    static const std::string DURATION;
    static const std::string BITRATE;
    static const std::string SAMPLE_FREQUENCY;
    static const std::string BITS_PER_SAMPLE;
    static const std::string NR_AUDIO_CHANNELS;
    static const std::string RESOLUTION;
    static const std::string COLOR_DEPTH;
    static const std::string PROTOCOL_INFO;
    static const std::string PROTECTION;
    static const std::string IMPORT_URI;
    static const std::string ALBUM_ART_URI;
    static const std::string ARTIST_DISCOGRAPHY_URI;
    static const std::string LYRICS_URI;
    static const std::string RELATION;
    static const std::string STORAGE_TOTAL;
    static const std::string STORAGE_USED;
    static const std::string STORAGE_FREE;
    static const std::string STORAGE_MAX_PARTITION;
    static const std::string STORAGE_MEDIUM;
    static const std::string DESCRIPTION;
    static const std::string LONG_DESCRIPTION;
    static const std::string ICON;
    static const std::string REGION;
    static const std::string RATING;
    static const std::string RIGHTS;
    static const std::string DATE;
    static const std::string LANGUAGE;
    static const std::string RADIO_CALL_SIGN;
    static const std::string RADIO_STATION_ID;
    static const std::string RADIO_BAND;
    static const std::string CHANNEL_NR;
    static const std::string CHANNEL_NAME;
    static const std::string SCHEDULED_START_TIME;
    static const std::string SCHEDULED_END_TIME;
    static const std::string DVD_REGION_CODE;
    static const std::string ORIGINAL_TRACK_NUMBER;
    static const std::string TOC;
    static const std::string USER_ANNOTATION;
};


class AvTypeConverter
{
public:
    static r8 readDuration(const std::string& duration);
    static std::string writeDuration(const r8& duration);
    /// duration in seconds
    static Omm::time readTime(const std::string& timeString);
    static std::string writeTime(const Omm::time& timeVal);
    /// timeVal in micro seconds
};


class Mime : Poco::Net::MediaType
{
public:
    Mime();
    Mime(const Mime& mime);
    Mime(const std::string& mimeString);

    static const std::string TYPE_AUDIO;
    static const std::string TYPE_VIDEO;
    static const std::string TYPE_IMAGE;
    static const std::string AUDIO_MPEG;
    static const std::string VIDEO_MPEG;
    static const std::string VIDEO_QUICKTIME;
    static const std::string VIDEO_AVI;
    static const std::string IMAGE_JPEG;
    static const std::string PLAYLIST;

    bool isAudio();
    bool isVideo();
    bool isImage();
};


class DeviceType
{
public:
    static const std::string MEDIA_RENDERER_1;
    static const std::string MEDIA_SERVER_1;
};


class ServiceType
{
public:
    static const std::string RC_1;
    static const std::string AVT_1;
    static const std::string CM_1;
    static const std::string CD_1;
};


class AvTransportEventedStateVar
{
public:
    static const std::string TRANSPORT_STATE;
    static const std::string TRANSPORT_STATUS;
    static const std::string PLAYBACK_STORAGE_MEDIUM;
    static const std::string POSSIBLE_PLAYBACK_STORAGE_MEDIA;
    static const std::string POSSIBLE_RECORD_STORAGE_MEDIA;
    static const std::string CURRENT_PLAY_MODE;
    static const std::string TRANSPORT_PLAY_SPEED;
    static const std::string RECORD_MEDIUM_WRITE_STATUS;
    static const std::string POSSIBLE_RECORD_QUALITY_MODES;
    static const std::string CURRENT_RECORD_QUALITY_MODE;
    static const std::string NUMBER_OF_TRACKS;
    static const std::string CURRENT_TRACK;
    static const std::string CURRENT_TRACK_DURATION;
    static const std::string CURRENT_MEDIA_DURATION;
    static const std::string CURRENT_TRACK_URI;
    static const std::string CURRENT_TRACK_META_DATA;
    static const std::string AVTRANSPORT_URI;
    static const std::string AVTRANSPORT_URI_META_DATA;
    static const std::string NEXT_AVTRANSPORT_URI;
    static const std::string NEXT_AVTRANSPORT_URI_META_DATA;
    static const std::string CURRENT_TRANSPORT_ACTIONS;
};


class AvTransportArgument
{
public:
    static const std::string TRANSPORT_STATE_STOPPED;
    static const std::string TRANSPORT_STATE_PLAYING;
    static const std::string TRANSPORT_STATE_TRANSITIONING;
    static const std::string TRANSPORT_STATE_PAUSED_PLAYBACK;
    static const std::string TRANSPORT_STATE_PAUSED_RECORDING;
    static const std::string TRANSPORT_STATE_RECORDING;
    static const std::string TRANSPORT_STATE_NO_MEDIA_PRESENT;
    static const std::string TRANSPORT_STATUS_OK;
    static const std::string TRANSPORT_STATUS_ERROR_OCCURRED;
    static const std::string PLAYBACK_STORAGE_MEDIUM_UNKNOWN;
    static const std::string PLAYBACK_STORAGE_MEDIUM_DV;
    static const std::string PLAYBACK_STORAGE_MEDIUM_MINI_DV;
    static const std::string PLAYBACK_STORAGE_MEDIUM_VHS;
    static const std::string PLAYBACK_STORAGE_MEDIUM_W_VHS;
    static const std::string PLAYBACK_STORAGE_MEDIUM_S_VHS;
    static const std::string PLAYBACK_STORAGE_MEDIUM_D_VHS;
    static const std::string PLAYBACK_STORAGE_MEDIUM_VHSC;
    static const std::string PLAYBACK_STORAGE_MEDIUM_VIDEO8;
    static const std::string PLAYBACK_STORAGE_MEDIUM_HI8;
    static const std::string PLAYBACK_STORAGE_MEDIUM_CD_ROM;
    static const std::string PLAYBACK_STORAGE_MEDIUM_CD_DA;
    static const std::string PLAYBACK_STORAGE_MEDIUM_CD_R;
    static const std::string PLAYBACK_STORAGE_MEDIUM_CD_RW;
    static const std::string PLAYBACK_STORAGE_MEDIUM_VIDEO_CD;
    static const std::string PLAYBACK_STORAGE_MEDIUM_SACD;
    static const std::string PLAYBACK_STORAGE_MEDIUM_MD_AUDIO;
    static const std::string PLAYBACK_STORAGE_MEDIUM_MD_PICTURE;
    static const std::string PLAYBACK_STORAGE_MEDIUM_DVD_ROM;
    static const std::string PLAYBACK_STORAGE_MEDIUM_DVD_VIDEO;
    static const std::string PLAYBACK_STORAGE_MEDIUM_DVD_R;
    static const std::string PLAYBACK_STORAGE_MEDIUM_DVD_PLUS_RW;
    static const std::string PLAYBACK_STORAGE_MEDIUM_DVD_RW;
    static const std::string PLAYBACK_STORAGE_MEDIUM_DVD_RAM;
    static const std::string PLAYBACK_STORAGE_MEDIUM_DVD_AUDIO;
    static const std::string PLAYBACK_STORAGE_MEDIUM_DAT;
    static const std::string PLAYBACK_STORAGE_MEDIUM_LD;
    static const std::string PLAYBACK_STORAGE_MEDIUM_HDD;
    static const std::string PLAYBACK_STORAGE_MEDIUM_MICRO_MV;
    static const std::string PLAYBACK_STORAGE_MEDIUM_NETWORK;
    static const std::string PLAYBACK_STORAGE_MEDIUM_NONE;
    static const std::string PLAYBACK_STORAGE_MEDIUM_NOT_IMPLEMENTED;
    static const std::string CURRENT_PLAY_MODE_NORMAL;
    static const std::string CURRENT_PLAY_MODE_SHUFFLE;
    static const std::string CURRENT_PLAY_MODE_REPEAT_ONE;
    static const std::string CURRENT_PLAY_MODE_REPEAT_ALL;
    static const std::string CURRENT_PLAY_MODE_RANDOM;
    static const std::string CURRENT_PLAY_MODE_DIRECT_1;
    static const std::string CURRENT_PLAY_MODE_INTRO;
    static const std::string TRANSPORT_PLAY_SPEED_1;
    static const std::string RECORD_MEDIUM_WRITE_STATUS_WRITABLE;
    static const std::string RECORD_MEDIUM_WRITE_STATUS_PROTECTED;
    static const std::string RECORD_MEDIUM_WRITE_STATUS_NOT_WRITABLE;
    static const std::string RECORD_MEDIUM_WRITE_STATUS_UNKOWN;
    static const std::string RECORD_MEDIUM_WRITE_STATUS_NOT_IMPLEMENTED;
    static const std::string CURRENT_RECORD_QUALITY_MODE_0_EP;
    static const std::string CURRENT_RECORD_QUALITY_MODE_1_LP;
    static const std::string CURRENT_RECORD_QUALITY_MODE_2_SP;
    static const std::string CURRENT_RECORD_QUALITY_MODE_0_BASIC;
    static const std::string CURRENT_RECORD_QUALITY_MODE_1_MEDIUM;
    static const std::string CURRENT_RECORD_QUALITY_MODE_2_HIGH;
    static const std::string CURRENT_RECORD_QUALITY_MODE_NOT_IMPLEMENTED;
    static const std::string CURRENT_TRACK_DURATION_0;
    static const std::string CURRENT_TRACK_META_DATA_NOT_IMPLEMENTED;
    static const std::string RELATIVE_TIME_POSITION_NOT_IMPLEMENTED;
    static const std::string CURRENT_TRANSPORT_ACTIONS_NOT_IMPLEMENTED;
    static const std::string SEEK_MODE_TRACK_NR;
    static const std::string SEEK_MODE_ABS_TIME;
    static const std::string SEEK_MODE_REL_TIME;
    static const std::string SEEK_MODE_ABS_COUNT;
    static const std::string SEEK_MODE_REL_COUNT;
    static const std::string SEEK_MODE_CHANNEL_FREQ;
    static const std::string SEEK_MODE_TAPE_INDEX;
    static const std::string SEEK_MODE_FRAME;
    static const i4 RELATIVE_COUNTER_POSITION_UNDEFINED;
};


class RenderingControlEventedStateVar
{
public:
    static const std::string PRESET_NAME_LIST;
    static const std::string BRIGHTNESS;
    static const std::string CONTRAST;
    static const std::string SHARPNESS;
    static const std::string RED_VIDEO_GAIN;
    static const std::string GREEN_VIDEO_GAIN;
    static const std::string BLUE_VIDEO_GAIN;
    static const std::string RED_BLACK_LEVEL;
    static const std::string GREEN_BLACK_LEVEL;
    static const std::string BLUE_BLACK_LEVEL;
    static const std::string COLOR_TEMPERATURE;
    static const std::string HORIZONTAL_KEYSTONE;
    static const std::string VERTICAL_KEYSTONE;
    static const std::string MUTE;
    static const std::string VOLUME;
    static const std::string VOLUME_DB;
    static const std::string LOUDNESS;
};


class AvChannel
{
public:
    static const std::string MASTER;
    static const std::string LF;
    static const std::string RF;
    static const std::string CF;
    static const std::string LFE;
    static const std::string LS;
    static const std::string RS;
    static const std::string LFC;
    static const std::string RFC;
    static const std::string SD;
    static const std::string SL;
    static const std::string SR;
    static const std::string T;
    static const std::string B;
};


class PresetName
{
public:
    static const std::string FACTORY_DEFAULTS;
    static const std::string INSTALLATION_DEFAULTS;
};


class ProtocolInfo
{
public:
    ProtocolInfo();
    ProtocolInfo(const ProtocolInfo& protInfo);
    ProtocolInfo(const std::string& infoString);

    std::string getMimeString() const;
    std::string getDlnaString() const;
};


class CsvList
{
public:
    CsvList(const std::string& csvListString = "");
    CsvList(const std::string& item1, const std::string& item2, const std::string& item3 = "", const std::string& item4 = "", const std::string& item5 = "");
    CsvList(const CsvList& csvList);
    virtual ~CsvList() {}

    typedef std::list<std::string>::iterator Iterator;
    Iterator begin();
    Iterator end();

    std::size_t getSize();
    void append(const std::string& item);
    void remove(const std::string& item);
    Iterator find(const std::string& item);

    virtual std::string toString();
};


class ConnectionManagerId
{
public:
    ConnectionManagerId();
    ConnectionManagerId(const std::string& uuid, const std::string& serviceId);

    void parseManagerIdString(const std::string& idString);

    std::string toString();
    std::string getUuid();
    std::string getServiceId();
};


class ConnectionPeer
{
public:
    ConnectionPeer();

    ui4 getConnectionId();
    ConnectionManagerId& getConnectionManagerId();
};

class Connection
{
public:
//    Connection(Device* pServer, Device* pRenderer);
    Connection(const std::string& serverUuid, const std::string& rendererUuid, const std::string& serverServiceType = ServiceType::CD_1, const std::string& rendererServiceType = ServiceType::AVT_1);

    i4 getAvTransportId();
    ConnectionPeer& getRenderer();
    ConnectionPeer& getServer();
    ConnectionPeer& getThisPeer(const std::string& deviceType);
    ConnectionPeer& getRemotePeer(const std::string& deviceType);
};


class ConnectionManager
{
public:
    ConnectionManager(Device* pDevice);
    virtual ~ConnectionManager() {}

    typedef std::map<ui4, Connection*>::iterator ConnectionIterator;
    ConnectionIterator beginConnection();
    ConnectionIterator endConnection();

    virtual void addConnection(Connection* pConnection, const std::string& protInfo);
    virtual void removeConnection(ui4 connectionId);
    virtual Connection* getConnection(ui4 connectionId);
    virtual CsvList getConnectionIds();
    virtual int getConnectionCount();
};


class CtlMediaServer : public Device
{
public:
    virtual void addCtlDeviceCode();
    virtual void initController();

    virtual CtlMediaObject* createMediaObject();
    CtlMediaObject* getRootObject() const;

    void selectMediaObject(CtlMediaObject* pObject, CtlMediaObject* pParentObject = 0, ui4 row = 0);
    void browseRootObject(bool useBlockCache = true);
    void setSort(const std::string& sortText);
    std::string getSort();

    ConnectionManager* getConnectionManager();
    CtlMediaObject* getMediaObjectFromResource(const std::string& resource);

    virtual void newSystemUpdateId(ui4 id) {}
};


class CtlMediaServerGroup : public DeviceGroup
{
public:
    CtlMediaServerGroup();

    CtlMediaServer* getDevice(int index) const;
    CtlMediaServer* getDevice(const std::string& uuid);
    CtlMediaServer* getSelectedDevice() const;
    virtual CtlMediaServer* createDevice();

protected:
    virtual void addCtlMediaServer(CtlMediaServer* pDevice, int index, bool begin) {}
    virtual void removeCtlMediaServer(CtlMediaServer* pDevice, int index, bool begin) {}
};


class CtlMediaRendererDelegate
{
public:
    CtlMediaRendererDelegate() {}
    virtual ~CtlMediaRendererDelegate() {}
    virtual void newPosition(r8 duration, r8 position) {}
    virtual void newUri(const std::string& uri) {}
    virtual void newTrack(const std::string& title, const std::string& artist, const std::string& album, const std::string& objectClass, const std::string& server, const std::string& uri) {}
    virtual void newVolume(const int volume) {}
    virtual void newTransportState(const std::string& transportState) {}
};


class CtlMediaRenderer : public Device
{
public:
    CtlMediaRenderer();

    virtual void addCtlDeviceCode();
    virtual void initController();

    void setObject(CtlMediaObject* pObject, CtlMediaObject* pParentObject, ui4 row);
    CtlMediaObject* getObject();
    void playPressed();
    void stopPressed();
    void pausePressed();
    void forwardPressed();
    void backPressed();
    void positionMoved(r8 position);
    /// positionMoved() seeks to position in secs
    void volumeChanged(int value);
    ui2 getVolume();
    void setMute(bool mute);
    bool getMute();

    void startPositionTimer(bool start = true);
    ConnectionManager* getConnectionManager();

    void setDelegate(CtlMediaRendererDelegate* pDelegate);
    CtlMediaRendererDelegate* getDelegate();

    virtual void newPosition(r8 duration, r8 position);
    virtual void newUri(const std::string& uri);
    virtual void newTrack(const std::string& title, const std::string& artist, const std::string& album, const std::string& objectClass, const std::string& server, const std::string& uri);
    virtual void newVolume(const int volume);
    virtual void newTransportState(const std::string& transportState);
};


class CtlMediaRendererGroup : public DeviceGroup
{
public:
    CtlMediaRendererGroup();

    CtlMediaRenderer* getDevice(int index) const;
    CtlMediaRenderer* getDevice(const std::string& uuid);
    CtlMediaRenderer* getSelectedDevice() const;
    virtual CtlMediaRenderer* createDevice();

protected:
    virtual void addCtlMediaRenderer(CtlMediaRenderer* pDevice, int index, bool begin) {}
    virtual void removeCtlMediaRenderer(CtlMediaRenderer* pDevice, int index, bool begin) {}

};


class AbstractProperty
{
public:
    AbstractProperty(PropertyImpl* pPropertyImpl);
    virtual ~AbstractProperty() {}

    virtual void setName(const std::string& name);
    virtual void setValue(const std::string& value);
    virtual void setAttribute(const std::string& name, const std::string& value);

    virtual std::string getName();
    virtual std::string getValue();
    virtual std::string getAttributeName(int index);
    virtual std::string getAttributeValue(int index);
    virtual std::string getAttributeValue(const std::string& name);
    virtual int getAttributeCount();
};


class AbstractResource : public AbstractProperty
{
public:
//     AbstractResource(const std::string& uri, const std::string& protInfo, ui4 size);
    AbstractResource(PropertyImpl* pPropertyImpl);
    virtual ~AbstractResource() {}

    virtual void setName(const std::string& name) {}
    void setUri(const std::string& uri);
    void setProtInfo(const std::string& protInfo);
    void setSize(ui4 size);

    virtual std::string getName() { return "res"; }
    virtual std::string getUri();
    virtual std::string getProtInfo();
    virtual std::streamsize getSize();
};


class AbstractMediaObject
{
public:
    virtual ~AbstractMediaObject() {};

    // factory methods
    virtual AbstractMediaObject* createChildObject() { return 0; }
    virtual AbstractProperty* createProperty() { return 0; }
    virtual AbstractResource* createResource() { return 0; }

    virtual std::string getId() { return ""; }
    virtual void setId(const std::string& id) {}
    virtual std::string getParentId() { return ""; }

    // attributes
    virtual bool isRestricted() { return true; }
    virtual void setIsRestricted(bool isRestricted) {}
    virtual bool isSearchable() { return false; }
    virtual void setIsSearchable(bool isSearchable = true) {}

    // properties
    std::string getTitle();
    void setTitle(const std::string& title);
    std::string getClass();
    void setClass(const std::string& subclass);
    virtual void addProperty(AbstractProperty* pProperty) {}
    // TODO: title and class are mandatory properties
    virtual int getPropertyCount(const std::string& name = "") = 0;
    virtual AbstractProperty* getProperty(int index) = 0;
    virtual AbstractProperty* getProperty(const std::string& name, int index = 0) = 0;
    virtual void setUniqueProperty(const std::string& name, const std::string& value);

    // resources
    int getResourceCount();
    void addResource(AbstractResource* pResource);
    virtual AbstractResource* getResource(int index = 0);

    // parent and descendants
    virtual bool isContainer() { return false; }
    virtual void setIsContainer(bool isContainer) {}
    virtual ui4 getChildCount() { return 0; }
    virtual void setChildCount(ui4 childCount) {}
    virtual CsvList* getSortCaps() { return 0; }
    virtual CsvList* getSearchCaps() { return 0; }
};


class MemoryMediaObject : public AbstractMediaObject
{
public:
    MemoryMediaObject();
    MemoryMediaObject(const MemoryMediaObject& mediaObject);
    virtual ~MemoryMediaObject() {}

    // factory methods
    virtual AbstractMediaObject* createChildObject();
    virtual AbstractProperty* createProperty();
    virtual AbstractResource* createResource();

    // attributes
    virtual bool isRestricted();
    virtual bool isSearchable();
    void setIsRestricted(bool restricted = true);
    void setIsSearchable(bool searchable = true);

    // properties
    virtual int getPropertyCount(const std::string& name = "");
    void addProperty(AbstractProperty* pProperty);
    virtual AbstractProperty* getProperty(int index);
    virtual AbstractProperty* getProperty(const std::string& name, int index = 0);

    // descendants
    virtual bool isContainer();
    virtual void setIsContainer(bool isContainer);
    virtual ui4 getFetchedChildCount();
    virtual ui4 getChildCount();
    virtual void setChildCount(ui4 childCount);
    virtual AbstractMediaObject* getChildForRow(ui4 row);
};


class CtlMediaObject : public MemoryMediaObject, public BlockCache
{
public:
    CtlMediaObject();
    CtlMediaObject(const CtlMediaObject& mediaObject);

    virtual CtlMediaObject* createChildObject();

    CtlMediaServer* getServer() const;
    void setServer(CtlMediaServer* pServer);
    void setServerController(CtlMediaServerCode* pServerCode);

    virtual std::string getId();
    virtual void setId(const std::string& id);
    void setSearch(const std::string& searchText);

    int fetchChildren(ui4 count = 25);
    bool fetchedAllChildren();
    CtlMediaObject* getParent();

    virtual CtlMediaObject* getChildForRow(ui4 row, bool useBlockCache = true);
    /// if you don't use the block cache, you have to fetch some children first with fetchChildren()

    Icon* getIcon();
    Icon* getImageRepresentation();

    void writeResource(const std::string& sourceUri, int index = 0);
    void createChildObject(CtlMediaObject* pObject);
    void destroyObject(const std::string& objectId);
};
