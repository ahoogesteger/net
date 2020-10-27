
#pragma once

#include "IDataBlock.h"
#include <vector>
#include <memory>

namespace net
{
    class DataBlockContainer : public IDataBlock
    {
    public:
        using IDataBlockPtr = std::unique_ptr<IDataBlock>;

        DataBlockContainer();

        // from IDataBlock
        virtual void update() override;
        virtual void setParent(IDataBlock* const parent) override { parent_ = parent; }
        virtual size_t getSize() const override;
        virtual void insertInto(uint8_t* const buffer) const override;

        template <typename T, typename... Args>
        T& addBlock(Args&&... args);

    protected:
        IDataBlock* parent_;
        std::vector<IDataBlockPtr> blocks_;
    };

    template <typename T, typename... Args>
    T& DataBlockContainer::addBlock(Args&&... args)
    {
        blocks_.push_back(std::make_unique<T>(std::forward<Args>(args)...));
        blocks_.back()->setParent(this);
        update();

        return static_cast<T&>(*blocks_.back());
    }
}

