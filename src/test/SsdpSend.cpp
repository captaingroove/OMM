#include "Omm/Upnp.h"
#include "Omm/Log.h"
#include "UpnpPrivate.h"

#include <Poco/Logger.h>
#include <Poco/Net/SocketStream.h>
#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/NetException.h>
#include <Poco/Net/NetworkInterface.h>

int main(int argc, char** argv)
{
    Poco::Net::MulticastSocket*         _pSsdpSenderSocket;
    _pSsdpSenderSocket = new Poco::Net::MulticastSocket(Poco::Net::SocketAddress("0.0.0.0", Omm::SSDP_PORT), true);
    _pSsdpSenderSocket->setTimeToLive(4);
    std::string m("Hello Poco World!");
    const Poco::Net::SocketAddress receiver(Omm::SSDP_FULL_ADDRESS);
    int bytesSent = _pSsdpSenderSocket->sendTo(m.c_str(), m.length(), receiver);
}
