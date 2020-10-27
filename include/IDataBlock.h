
#pragma once

#include <cstddef>
#include <cstdint>

namespace net
{
    class IDataBlock
    {
    public:
        virtual ~IDataBlock() {}

        virtual void update() = 0;
        virtual void setParent(IDataBlock* const parent) = 0;
        virtual size_t getSize() const = 0;
        virtual void insertInto(uint8_t* const buffer) const = 0;
    };
}
