
#include "DataBlockContainer.h"
#include <numeric>

namespace net
{
    DataBlockContainer::DataBlockContainer() :
        parent_(nullptr)
    {

    }

    void DataBlockContainer::update()
    {
        if (parent_) {
            parent_->update();
        }
    }

    size_t DataBlockContainer::getSize() const
    {
        return std::accumulate(blocks_.cbegin(), blocks_.cend(), static_cast<size_t>(0), [](const size_t& lhs, const IDataBlockPtr& rhs) {
            return lhs + rhs->getSize();
        });
    }

    void DataBlockContainer::insertInto(uint8_t* const buffer) const
    {
        uint8_t* offset = buffer;
        for (const IDataBlockPtr& dataBlock : blocks_) {
            dataBlock->insertInto(offset);
            offset += dataBlock->getSize();
        }
    }
}
