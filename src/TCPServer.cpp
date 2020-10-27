
#ifndef WIN32
#include "TCPServer.h"
#include "IConnectionPool.h"
#include "TCPConnection.h"
#include "IPv4EndPoint.h"
#include "Network.h"
#include <cstring>
#include <cassert>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>
#include <unistd.h>

namespace net
{
    TCPServer::TCPServer(IConnectionPool& connectionPool, const IPv4EndPoint& endPoint, const int backlogSize) :
        AbstractTCPServer(connectionPool, endPoint, backlogSize)
    {

    }

    TCPServer::TCPServer(IConnectionPool& connectionPool, const std::string& bindAddress, const uint16_t port, const int backlogSize) :
        AbstractTCPServer(connectionPool, bindAddress, port, backlogSize),
        socket_(0),
        isRunning_(false)
    {

    }

    TCPServer::~TCPServer()
    {
        stop();
    }

    void TCPServer::closeSocket()
    {
        assert(socket_);

        close(socket_);
        socket_ = 0;
    }

    bool TCPServer::createSocket()
    {
        assert(!socket_);

        socket_ = socket(AF_INET, SOCK_STREAM, 0);

        if (socket_ == -1) {
            socket_ = 0;
            return false;
        }

        struct sockaddr_in serverAddress;
        std::memset(&serverAddress, 0, sizeof(serverAddress));

        struct hostent* const hostInfo = gethostbyname(bindAddress_.data());

        if (!hostInfo) {
            closeSocket();
            return false;
        }

        serverAddress.sin_family = AF_INET;
        std::memcpy(&serverAddress.sin_addr, hostInfo->h_addr_list[0], static_cast<size_t>(hostInfo->h_length));
        serverAddress.sin_port = htons(port_);

        const int portReUse = 1;
        if (setsockopt(socket_, SOL_SOCKET, SO_REUSEPORT, &portReUse, sizeof(portReUse)) == -1) {
            // warning, no serious error
            // TODO give feedback about this
        }

        if (bind(socket_, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress)) == -1) {
            closeSocket();
            return false;
        }

        if (listen(socket_, backlogSize_) == -1) {
            closeSocket();
            return false;
        }

        return true;
    }

    bool TCPServer::start()
    {
        assert(!isRunning_);

        if (!createSocket()) {
            return false;
        }

        isRunning_ = true;

        serverTask_ = std::thread([this]() {
            while (isRunning_) {
                struct pollfd pollResult;
                pollResult.fd = socket_;
                pollResult.events = POLLIN;
                pollResult.revents = 0;

                poll(&pollResult, 1, 500);

                if ((pollResult.revents & POLLIN) == POLLIN) {
                    struct sockaddr_in peerAddress;
                    socklen_t addressLength = sizeof(peerAddress);
                    std::memset(&peerAddress, 0, sizeof(peerAddress));
                    const int peerSocket = accept(socket_, reinterpret_cast<sockaddr*>(&peerAddress), &addressLength);

                    const uint16_t peerPort = ntohs(peerAddress.sin_port);
                    std::array<char, INET_ADDRSTRLEN> addressBuffer;
                    std::memset(addressBuffer.data(), 0, addressBuffer.size());
                    inet_ntop(AF_INET, &peerAddress.sin_addr, addressBuffer.data(), addressLength);
                    connectionPool_.addConnection(std::make_unique<TCPConnection>(Network::instance(), peerSocket, addressBuffer.data(), peerPort));
                }
            }

            closeSocket();
        });

        return true;
    }

    void TCPServer::stop()
    {
        isRunning_ = false;

        if (serverTask_.joinable()) {
            serverTask_.join();
        }
    }
}
#endif
