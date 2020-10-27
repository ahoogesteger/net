
#include "AbstractTCPConnection.h"
#include "TCPConnection.h"
#include "Win32TCPConnection.h"
#include "IDataBlock.h"
#include "IPv4EndPoint.h"
#include "Network.h"
#include <vector>

namespace net
{
    AbstractTCPConnection::AbstractTCPConnection(Network& network, const std::string& peerAddress, const std::uint16_t peerPort) :
        network_(network),
        peerAddress_(peerAddress),
        peerPort_(peerPort)
    {

    }

    void AbstractTCPConnection::write(const IDataBlock& data, const int flags) const
    {
        std::vector<uint8_t> buffer(data.getSize());
        data.insertInto(buffer.data());
        write(buffer.data(), buffer.size(), flags);
    }

    void AbstractTCPConnection::write(const IDataBlock& data) const
    {
        write(data, 0);
    }

    void AbstractTCPConnection::fill(unsigned char* const buffer, const std::size_t bufferLength) const
    {
        std::size_t bytesRead = 0;
        while (bytesRead < bufferLength) {
            bytesRead += read(buffer + bytesRead, bufferLength - bytesRead);
        }
    }

    AbstractTCPConnectionPtr AbstractTCPConnection::createInstance(const IPv4EndPoint& endPoint)
    {
        return createInstance(endPoint.host(), endPoint.port());
    }

    AbstractTCPConnectionPtr AbstractTCPConnection::createInstance(const std::string& address, const std::uint16_t port)
    {
#ifdef WIN32
        return std::make_unique<Win32TCPConnection>(Network::instance(), address, port);
#else
        return std::make_unique<TCPConnection>(Network::instance(), address, port);
#endif
    }
}
