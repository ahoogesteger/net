
#include "Network.h"

#ifdef WIN32
#include <WinSock2.h>
#include <exception>
#include <string>
#endif

namespace net
{
    Network::Network()
    {
#ifdef WIN32
        WSADATA wsaData;

        const int result = WSAStartup(MAKEWORD(2, 2), &wsaData);

        if (result) {
            const std::string message = std::string("Unable to find Winsock library: ") + std::to_string(result);
            throw std::exception(message.c_str());
        }

        const int highByte = static_cast<int>(HIBYTE(wsaData.wVersion));
        const int lowByte = static_cast<int>(LOBYTE(wsaData.wVersion));

        if (highByte != 2 || lowByte != 2) {
            const std::string message = std::string("Unable to load Winsock v2.2: ") + std::to_string(highByte) + "." + std::to_string(lowByte);
            throw std::exception(message.c_str());
        }
#endif
    }

    Network::~Network()
    {
#ifdef WIN32
        WSACleanup();
#endif
    }

    Network& Network::instance()
    {
        static Network network;
        return network;
    }
}
