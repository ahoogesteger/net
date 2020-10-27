
#pragma once

#include <cstdint>
#include <endian.h>

namespace net
{
    template <typename T>
    T fromLittleEndian16(const T val)
    {
        static_assert(sizeof(T) == 2, "sizeof T != 2 bytes");
        return static_cast<T>(le16toh(val));
    }

    template <typename T>
    T toLittleEndian16(const T val)
    {
        static_assert(sizeof(T) == 2, "sizeof T != 2 bytes");
        return static_cast<T>(htole16(val));
    }

    template <typename T>
    T fromLittleEndian32(const T val)
    {
        static_assert(sizeof(T) == 4, "sizeof T != 4 bytes");
        return static_cast<T>(le32toh(val));
    }

    template <typename T>
    T toLittleEndian32(const T val)
    {
        static_assert(sizeof(T) == 4, "sizeof T != 4 bytes");
        return static_cast<T>(htole32(val));
    }

    template <typename T>
    T fromBigEndian16(const T val)
    {
        static_assert(sizeof(T) == 2, "sizeof T != 2 bytes");
        return static_cast<T>(be16toh(val));
    }

    template <typename T>
    T toBigEndian16(const T val)
    {
        static_assert(sizeof(T) == 2, "sizeof T != 2 bytes");
        return static_cast<T>(htobe16(val));
    }

    template <typename T>
    T fromBigEndian32(const T val)
    {
        static_assert(sizeof(T) == 4, "sizeof T != 4 bytes");
        return static_cast<T>(be32toh(val));
    }

    template <typename T>
    T toBigEndian32(const T val)
    {
        static_assert(sizeof(T) == 4, "sizeof T != 4 bytes");
        return static_cast<T>(htobe32(val));
    }
}
