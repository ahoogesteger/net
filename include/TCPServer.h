
#pragma once

#ifndef WIN32
#include "AbstractTCPServer.h"
#include <thread>

namespace net
{
    class IConnectionPool;
    class IPv4EndPoint;

    class TCPServer :
        public AbstractTCPServer
    {
    public:
        TCPServer(IConnectionPool& connectionPool, const IPv4EndPoint& endPoint, const int backlogSize);
        TCPServer(IConnectionPool& connectionPool, const std::string& bindAddress, const uint16_t port, const int backlogSize);
        ~TCPServer();

        virtual bool start() override;
        virtual void stop() override;

    private:
        bool createSocket();
        void closeSocket();

        int socket_;
        std::thread serverTask_;
        volatile bool isRunning_;
    };
}
#endif

