
#pragma once

#ifdef WIN32
#include "AbstractTCPConnection.h"
#include <WinSock2.h>

namespace net
{
    class Win32TCPConnection :
        public AbstractTCPConnection
    {
    public:
        Win32TCPConnection(Network& network, const std::string& peerAddress, const std::uint16_t peerPort);
        ~Win32TCPConnection();

        virtual void setReadTimeout(const std::size_t timeout) const override;
        virtual void setWriteTimeout(const std::size_t timeout) const override;
        virtual std::size_t read(unsigned char* const buffer, const std::size_t bufferLength) const override;
        virtual void write(const unsigned char* const buffer, const std::size_t bufferLength, const int flags) const override;
        virtual void write(const unsigned char* const buffer, const std::size_t bufferLength) const override;

    private:
        void throwIOException(const int error) const;
        void setTimeout(const std::size_t timeout, const int option) const;

        SOCKET socket_;
    };
}
#endif
