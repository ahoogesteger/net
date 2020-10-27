
#ifdef WIN32
#include "Win32TCPConnection.h"
#include <WS2tcpip.h>
#include <array>
#include <cassert>

namespace net
{
    Win32TCPConnection::Win32TCPConnection(Network& network, const std::string& peerAddress, const uint16_t peerPort) :
        AbstractTCPConnection(network, peerAddress, peerPort),
        socket_(INVALID_SOCKET)
    {
        struct addrinfo* addressInfo = nullptr;
        struct addrinfo hints {};
        hints.ai_family = AF_INET; // IPv6 is not currently supported
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;

        const std::string peerPortStr = std::to_string(peerPort);
        const INT addressInfoResult = getaddrinfo(peerAddress.c_str(), peerPortStr.c_str(), &hints, &addressInfo);

        if (addressInfoResult) {
            throw SocketException("Unable to resolve hostname: " + peerAddress);
        }

        socket_ = socket(addressInfo->ai_family, addressInfo->ai_socktype, addressInfo->ai_protocol);

        if (socket_ == INVALID_SOCKET) {
            freeaddrinfo(addressInfo);
            throw SocketException("Failed to create the client socket");
        }

        const INT connectResult = connect(socket_, addressInfo->ai_addr, static_cast<int>(addressInfo->ai_addrlen));

        if (connectResult == SOCKET_ERROR) {
            closesocket(socket_);
            freeaddrinfo(addressInfo);
            throw IOException("Unable to connect to host");
        }

        freeaddrinfo(addressInfo);
        assert(socket_ != INVALID_SOCKET);
    }

    Win32TCPConnection::~Win32TCPConnection()
    {
        assert(socket_ != INVALID_SOCKET);
        closesocket(socket_);
    }

    void Win32TCPConnection::setTimeout(const std::size_t timeout, const int option) const
    {
        assert(option == SO_RCVTIMEO || option == SO_SNDTIMEO);

        const DWORD value = static_cast<DWORD>(timeout);

        if (setsockopt(socket_, SOL_SOCKET, option, reinterpret_cast<const char*>(&value), sizeof(value)) == SOCKET_ERROR) {
            const std::string timeoutTypeStr = option == SO_SNDTIMEO ? "SEND" : "RECEIVE";
            throw SocketModificationException("Unable to set " + timeoutTypeStr + " timeout to " + std::to_string(timeout) + "ms for " + name());
        }
    }

    void Win32TCPConnection::setReadTimeout(const std::size_t timeout) const
    {
        setTimeout(timeout, SO_RCVTIMEO);
    }

    void Win32TCPConnection::setWriteTimeout(const std::size_t timeout) const
    {
        setTimeout(timeout, SO_SNDTIMEO);
    }

    std::size_t Win32TCPConnection::read(unsigned char* const buffer, const std::size_t bufferLength) const
    {
        const int bytesRead = recv(socket_, reinterpret_cast<char*>(buffer), static_cast<int>(bufferLength), 0);

        if (bytesRead > 0) {
            return static_cast<std::size_t>(bytesRead);
        }

        if (bytesRead == 0) {
            throw ConnectionClosedException();
        } else if (bytesRead == SOCKET_ERROR) {
            const int error = WSAGetLastError();

            switch (error) {
            case WSAECONNRESET:
                throw ConnectionClosedException();
            case WSAETIMEDOUT:
                throw TimeoutException(TimeoutException::Cause::Reading);
            default:
                throwIOException(error);
            }
        }

        // this would only be reached if bytesRead < -1 which should be impossible
        assert(false);
        return 0;
    }

    void Win32TCPConnection::write(const unsigned char* const buffer, const std::size_t bufferLength) const
    {
        write(buffer, bufferLength, 0);
    }

    void Win32TCPConnection::write(const unsigned char * const buffer, const std::size_t bufferLength, const int flags) const
    {
        std::size_t totalBytesSent = 0;

        while (totalBytesSent < bufferLength) {
            const int bytesSent = send(socket_, reinterpret_cast<const char*>(buffer + totalBytesSent), static_cast<int>(bufferLength - totalBytesSent), flags);

            if (bytesSent != SOCKET_ERROR) {
                totalBytesSent += static_cast<std::size_t>(bytesSent);
            } else {
                const int error = WSAGetLastError();

                switch (error) {
                case WSAECONNRESET:
                    throw ConnectionClosedException();
                case WSAETIMEDOUT:
                    throw TimeoutException(TimeoutException::Cause::Writing);
                default:
                    throwIOException(error);
                }
            }
        }
    }

    void Win32TCPConnection::throwIOException(const int error) const
    {
        std::array<char, 255> message{};
        strerror_s(message.data(), message.size(), error);
        throw IOException(message.data());
    }
}
#endif

