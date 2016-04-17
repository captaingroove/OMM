/***************************************************************************|
|  OMM - Open Multimedia                                                    |
|                                                                           |
|  Copyright (C) 2009, 2010, 2011                                           |
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

#ifndef UpnpAv_INCLUDED
#define UpnpAv_INCLUDED

#include <string>
#include <list>
#include <map>

#include "UpnpTypes.h"

namespace Omm {

class Device;

namespace Av {


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

private:
    std::string _mime;
    std::string _dlna;
};


class CsvList
{
public:
    CsvList(const std::string& csvListString = "");
    CsvList(const std::string& item1, const std::string& item2, const std::string& item3 = "", const std::string& item4 = "", const std::string& item5 = "");
    CsvList(const CsvList& csvList);

    typedef std::list<std::string>::iterator Iterator;
    Iterator begin();
    Iterator end();

    std::size_t getSize();
    void append(const std::string& item);
    void remove(const std::string& item);
    Iterator find(const std::string& item);

    virtual std::string toString();

private:
    std::list<std::string>      _items;
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

private:
    std::string         _uuid;
    std::string         _serviceId; // NOTE: which service?
};


class ConnectionPeer
{
    friend class Connection;
    friend class CtlConnectionManagerImpl;
    friend class DevConnectionManagerRendererImpl;
    friend class DevConnectionManagerServerImpl;

public:
    ConnectionPeer();

    ui4 getConnectionId();
    ConnectionManagerId& getConnectionManagerId();

private:
    ConnectionManagerId         _managerId;
    ui4                         _connectionId;
    ui4                         _AVTId;
    ui4                         _RCId;
    ProtocolInfo                _protInfo;
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

private:
//    Device*                     _pServer;
    ConnectionPeer              _server;
//    Device*                     _pRenderer;
    ConnectionPeer              _renderer;
    bool                        _pull;
};


class ConnectionManager
{
public:
    ConnectionManager(Device* pDevice);

    typedef std::map<ui4, Connection*>::iterator ConnectionIterator;
    ConnectionIterator beginConnection();
    ConnectionIterator endConnection();

    virtual void addConnection(Connection* pConnection, const std::string& protInfo);
    virtual void removeConnection(ui4 connectionId);
    virtual Connection* getConnection(ui4 connectionId);
    virtual CsvList getConnectionIds();
    virtual int getConnectionCount();

protected:
    // device that owns this connection manager
    Device*                     _pDevice;

private:
    std::map<ui4, Connection*>  _connections;
};


}  // namespace Omm
}  // namespace Av


#endif

