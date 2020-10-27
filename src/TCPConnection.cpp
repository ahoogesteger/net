
#ifndef WIN32
#include "TCPConnection.h"
#include "IDataBlock.h"
#include <cassert>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>
#include <netdb.h>
#include <vector>
#include <cstring>

namespace net
{
    TCPConnection::TCPConnection(Network& network, const int socket, const std::string& peerAddress, const std::uint16_t peerPort) :
        AbstractTCPConnection(network, peerAddress, peerPort),
        socket_(socket)
    {
        assert(socket);
    }

    TCPConnection::TCPConnection(Network& network, const std::string& peerAddress, const std::uint16_t peerPort) :
        AbstractTCPConnection(network, peerAddress, peerPort),
        socket_(0)
    {
        struct addrinfo* addressInfo = nullptr;
        struct addrinfo hints {};
        hints.ai_family = AF_INET; // IPv6 is not currently supported
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;

        const std::string peerPortStr = std::to_string(peerPort);
        const int addrinfoResult = getaddrinfo(peerAddress.c_str(), peerPortStr.c_str(), &hints, &addressInfo);

        if (addrinfoResult) {
            throw SocketException("Unable to resolve hostname: " + peerAddress);
        }

        socket_ = socket(addressInfo->ai_family, addressInfo->ai_socktype, addressInfo->ai_protocol);

        if (socket_ == -1) {
            freeaddrinfo(addressInfo);
            throw SocketException("Failed to create the client socket");
        }

        if (connect(socket_, addressInfo->ai_addr, addressInfo->ai_addrlen) == -1) {
            close(socket_);
            freeaddrinfo(addressInfo);
            throw IOException("Unable to connect to host");
        }

        freeaddrinfo(addressInfo);
        assert(socket_ != -1);
    }

    TCPConnection::~TCPConnection()
    {
        close(socket_);
    }

    void TCPConnection::setTimeout(const std::size_t timeout, const int option) const
    {
        assert(option == SO_SNDTIMEO || option == SO_RCVTIMEO);

        struct timeval t;
        t.tv_sec = timeout / 1000;
        t.tv_usec = timeout % 1000 * 1000;

        if (setsockopt(socket_, SOL_SOCKET, option, &t, sizeof(t)) == -1) {
            const std::string timeoutTypeStr = option == SO_SNDTIMEO ? "SEND" : "RECEIVE";
            throw SocketModificationException("Unable to set " + timeoutTypeStr + " timeout to " + std::to_string(timeout) + "ms for " + name());
        }
    }

    void TCPConnection::setWriteTimeout(const std::size_t timeout) const
    {
        setTimeout(timeout, SO_SNDTIMEO);
    }

    void TCPConnection::setReadTimeout(const std::size_t timeout) const
    {
        setTimeout(timeout, SO_RCVTIMEO);
    }

    std::size_t TCPConnection::read(unsigned char* const buffer, const std::size_t bufferLength) const
    {
        const ssize_t bytesRead = recv(socket_, buffer, bufferLength, 0);

        if (bytesRead > 0) {
            return static_cast<std::size_t>(bytesRead);
        }

        if (bytesRead == 0) {
            throw ConnectionClosedException();
        } else if (bytesRead == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
            throw TimeoutException(TimeoutException::Cause::Reading);
        }

        char errorMsgBuffer[1024];
        const char* const errorMsg = strerror_r(errno, errorMsgBuffer, sizeof(errorMsgBuffer));
        throw IOException(errorMsg);
    }

    void TCPConnection::write(const unsigned char* const buffer, const std::size_t bufferLength, const int flags) const
    {
        std::size_t totalBytesSent = 0;

        while (totalBytesSent < bufferLength) {
            const ssize_t bytesSent = send(socket_, buffer + totalBytesSent, bufferLength - totalBytesSent, flags);

            if (bytesSent > 0) {
                totalBytesSent += static_cast<std::size_t>(bytesSent);
            } else if (bytesSent == 0) {
                throw ConnectionClosedException();
            } else if (bytesSent == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
                throw TimeoutException(TimeoutException::Cause::Writing);
            } else {
                char errorMsgBuffer[1024];
                const char* const errorMsg = strerror_r(errno, errorMsgBuffer, sizeof(errorMsgBuffer));
                throw IOException(errorMsg);
            }
        }
    }

    void TCPConnection::write(const unsigned char* const buffer, const std::size_t bufferLength) const
    {
        write(buffer, bufferLength, 0);
    }
}
#endif
