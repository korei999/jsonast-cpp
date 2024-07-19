#pragma once

#include <string.h>

#include "allocator.hh"
#include "utils.hh"

namespace adt
{

struct String
{
    char* pData = nullptr;
    size_t size = 0;

    String() = default;
    String(char* sNullTerminated) : pData(sNullTerminated), size(strlen(sNullTerminated)) {}
    String(const char* sNullTerminated) : pData(const_cast<char*>(sNullTerminated)), size(strlen(sNullTerminated)) {}
    String(char* sNullTerminated, size_t len) : pData(sNullTerminated), size(len) {}

    char& operator[](size_t i) { return this->pData[i]; }
    const char& operator[](size_t i) const { return this->pData[i]; }

    char* data() { return this->pData; }
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

template<typename ALLOC>
constexpr String
StringCreate(ALLOC* p, const char* str)
{
    size_t size = strlen(str);
    char* pData = static_cast<char*>(p->alloc(size, sizeof(char)));
    strncpy(pData, str, size);
    return {pData, size};
}

} /* namespace adt */
