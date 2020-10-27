
#pragma once

#include <memory>
#include <string>
#include <stdexcept>
#include <cstdint>

namespace net
{
    class IDataBlock;
    class IPv4EndPoint;
    class Network;

    class SocketException :
        public std::runtime_error
    {
    public:
        SocketException(const std::string& message) :
            std::runtime_error(message)
        {

        }
    };

    class SocketModificationException :
        public SocketException
    {
    public:
        SocketModificationException(const std::string& message) :
            SocketException("Failed to modify the socket: " + message)
        {

        }
    };

    class IOException :
        public std::runtime_error
    {
    public:
        IOException(const std::string& message) :
            std::runtime_error(message)
        {

        }
    };

    class TimeoutException :
        public IOException
    {
    public:
        enum class Cause
        {
            Reading,
            Writing
        };

        TimeoutException(const Cause cause) :
            IOException("IO operation timed-out"),
            cause_(cause)
        {

        }

        Cause cause() const { return cause_; }

    private:
        Cause cause_;
    };

    class ConnectionClosedException :
        public IOException
    {
    public:
        ConnectionClosedException() :
            IOException("Connection closed by peer")
        {

        }
    };

    class AbstractTCPConnection;
    using AbstractTCPConnectionPtr = std::unique_ptr<AbstractTCPConnection>;

    class AbstractTCPConnection
    {
    public:
        AbstractTCPConnection(Network& network, const std::string& peerAddress, const std::uint16_t peerPort);
        virtual ~AbstractTCPConnection() {}

        virtual void setReadTimeout(const std::size_t timeout) const = 0;
        virtual void setWriteTimeout(const std::size_t timeout) const = 0;
        virtual std::size_t read(unsigned char* const buffer, const std::size_t bufferLength) const = 0;
        virtual void write(const unsigned char* const buffer, const std::size_t bufferLength, const int flags) const = 0;
        virtual void write(const unsigned char* const buffer, const std::size_t bufferLength) const = 0;

        void fill(unsigned char* const buffer, const std::size_t bufferLength) const;
        void write(const IDataBlock& data, const int flags) const;
        void write(const IDataBlock& data) const;
        std::string name() const{ return peerAddress_ + ":" + std::to_string(peerPort_); }

        static AbstractTCPConnectionPtr createInstance(const IPv4EndPoint& endPoint);
        static AbstractTCPConnectionPtr createInstance(const std::string& address, const std::uint16_t port);

    protected:
        Network& network_;
        std::string peerAddress_;
        std::uint16_t peerPort_;
    };
}
