
#pragma once

#include "IDataBlock.h"

namespace net
{
    template <typename T>
    class DataBlock : public IDataBlock
    {
    public:
        DataBlock(const T& data) :
            data_(data)
        {

        }

        // from IDataBlock
        virtual void update() override {}
        virtual void setParent(IDataBlock* const /*parent*/) override {}
        virtual size_t getSize() const override { return sizeof(T); }
        virtual void insertInto(uint8_t* const buffer) const override { *reinterpret_cast<T*>(buffer) = data_; }

    private:
        T data_;
    };
}
