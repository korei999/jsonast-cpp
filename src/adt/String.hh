#pragma once

#include <string.h>

#include "allocator.hh"
#include "utils.hh"

namespace adt
{

constexpr size_t
nullTermStringSize(const char* str)
{
    size_t i = 0;
    while (str[i] != '\0')
        i++;

    return i;
}

struct String
{
    char* pData = nullptr;
    size_t size = 0;

    constexpr String() = default;
    constexpr String(char* sNullTerminated) : pData(sNullTerminated), size(nullTermStringSize(sNullTerminated)) {}
    constexpr String(const char* sNullTerminated) : pData(const_cast<char*>(sNullTerminated)), size(nullTermStringSize(sNullTerminated)) {}
    constexpr String(char* sNullTerminated, size_t len) : pData(sNullTerminated), size(len) {}

    constexpr char& operator[](size_t i) { return this->pData[i]; }
    constexpr const char& operator[](size_t i) const { return this->pData[i]; }

    constexpr char* data() { return this->pData; }
};

constexpr bool
operator==(const String& sL, const String& sR)
{
    auto m = min(sL.size, sR.size);
    for (size_t i = 0; i < m; i++)
        if (sL[i] != sR[i])
            return false;

    return true;
}

constexpr size_t
find(String sv, char c)
{
    for (size_t i = 0; i < sv.size; i++)
        if (sv[i] == c)
            return i;

    return NPOS;
}

constexpr String
StringCreate(BaseAllocator* p, const char* str, size_t size)
{
    char* pData = static_cast<char*>(p->alloc(size, sizeof(char)));
    strncpy(pData, str, size);
    return {pData, size};
}

constexpr String
StringCreate(BaseAllocator* p, const char* str)
{
    return StringCreate(p, str, nullTermStringSize(str));
}

} /* namespace adt */
