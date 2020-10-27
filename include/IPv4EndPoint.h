
#pragma once

#include <string>

namespace net
{
    class IPv4EndPoint
    {
    public:
        IPv4EndPoint(const std::string& host, const uint16_t port) :
            host_(host),
            port_(port)
        {

        }

        const std::string& host() const { return host_; }
        uint16_t port() const { return port_; }

    private:
        std::string host_;
        uint16_t port_;
    };
}
