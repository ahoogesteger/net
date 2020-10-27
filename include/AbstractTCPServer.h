
#pragma once

#include <cstdint>
#include <string>

namespace net
{
    class IConnectionPool;
    class IPv4EndPoint;

    class AbstractTCPServer
    {
    public:
        AbstractTCPServer(IConnectionPool& connectionPool, const IPv4EndPoint& endPoint, const int backlogSize);
        AbstractTCPServer(IConnectionPool& connectionPool, const std::string& bindAddress, const uint16_t port, const int backlogSize);
        virtual ~AbstractTCPServer() {}

        virtual bool start() = 0;
        virtual void stop() = 0;

    protected:
        IConnectionPool& connectionPool_;
        std::string bindAddress_;
        uint16_t port_;
        int backlogSize_;

    };
}
