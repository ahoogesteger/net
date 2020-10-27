
#include "AbstractTCPServer.h"
#include "IPv4EndPoint.h"

namespace net
{
    AbstractTCPServer::AbstractTCPServer(IConnectionPool& connectionPool, const IPv4EndPoint& endPoint, const int backlogSize) :
        AbstractTCPServer(connectionPool, endPoint.host(), endPoint.port(), backlogSize)
    {

    }

    AbstractTCPServer::AbstractTCPServer(IConnectionPool& connectionPool, const std::string& bindAddress, const uint16_t port, const int backlogSize) :
        connectionPool_(connectionPool),
        bindAddress_(bindAddress),
        port_(port),
        backlogSize_(backlogSize)
    {

    }
}
