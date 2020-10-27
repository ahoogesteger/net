
#pragma once

#include "IDataBlock.h"

namespace net
{
    template <typename T>
    class DataViewBlock : public IDataBlock
    {
    public:
        DataViewBlock(const T& data) :
            data_(data)
        {

        }

        // from IDataBlock
        virtual void update() override {}
        virtual void setParent(IDataBlock* const /*parent*/) override {}
        virtual size_t getSize() const override { return sizeof(T); }
        virtual void insertInto(uint8_t* const buffer) const override { *reinterpret_cast<T*>(buffer) = data_; }

    private:
        const T& data_;
    };
}
