
#pragma once

#include <memory>

namespace net
{
    class AbstractTCPConnection;
    using AbstractTCPConnectionPtr = std::unique_ptr<AbstractTCPConnection>;

    class IConnectionPool
    {
    public:
        virtual void addConnection(AbstractTCPConnectionPtr connection) = 0;
    };
}
