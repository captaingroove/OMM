/***************************************************************************|
|  OMM - Open Multimedia                                                    |
|                                                                           |
|  Copyright (C) 2009, 2010, 2011, 2012                                     |
|  Jörg Bakker (jb'at'open-multimedia.org)                                  |
|                                                                           |
|  This file is part of OMM.                                                |
|                                                                           |
|  OMM is free software: you can redistribute it and/or modify              |
|  it under the terms of the MIT License                                    |
 ***************************************************************************/

#include "Log.h"
#include "DvbLogger.h"


namespace Omm {
namespace Dvb {

#ifndef NDEBUG
Log* Log::_pInstance = 0;

// possible log levels: trace, debug, information, notice, warning, error, critical, fatal

Log::Log()
{
    Poco::Channel* pChannel = Util::Log::instance()->channel();
    _pDvbLogger = &Poco::Logger::create("DVB", pChannel, Poco::Message::PRIO_TRACE);
//    _pDvbLogger = &Poco::Logger::create("DVB", pChannel, Poco::Message::PRIO_DEBUG);
//    _pDvbLogger = &Poco::Logger::create("DVB", pChannel, Poco::Message::PRIO_ERROR);
}


Log*
Log::instance()
{
    if (!_pInstance) {
        _pInstance = new Log;
    }
    return _pInstance;
}


Poco::Logger&
Log::dvb()
{
    return *_pDvbLogger;
}
#endif // NDEBUG


}  // namespace Omm
}  // namespace Dvb
