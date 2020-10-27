
#pragma once

#ifndef WIN32
#include "AbstractTCPConnection.h"

namespace net
{
    class TCPConnection :
        public AbstractTCPConnection
    {
    public:
        TCPConnection(Network& network, const int socket, const std::string& peerAddress, const std::uint16_t peerPort);
        TCPConnection(Network& network, const std::string& peerAddress, const std::uint16_t peerPort);
        TCPConnection(const TCPConnection&) = delete;
        ~TCPConnection();

        TCPConnection& operator=(const TCPConnection&) = delete;

        virtual void setReadTimeout(const std::size_t timeout) const override;
        virtual void setWriteTimeout(const std::size_t timeout) const override;
        virtual std::size_t read(unsigned char* const buffer, const std::size_t bufferLength) const override;
        virtual void write(const unsigned char* const buffer, const std::size_t bufferLength, const int flags) const override;
        virtual void write(const unsigned char* const buffer, const std::size_t bufferLength) const override;

    private:
        void setTimeout(const std::size_t timeout, const int option) const;

        int socket_;
    };
}

#endif

