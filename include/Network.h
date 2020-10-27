
#pragma once

namespace net
{
    class Network
    {
    public:
        Network(const Network&) = delete;
        Network(Network&&) = delete;
        ~Network();

        Network& operator=(const Network&) = delete;
        Network& operator=(Network&&) = delete;

        static Network& instance();

    private:
        Network();
    };
}
