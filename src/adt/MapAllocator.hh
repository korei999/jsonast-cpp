#pragma once

#include "HashMap.hh"

namespace adt
{

template<>
inline size_t
fnHash<void* const>(void* const& x)
{
    return reinterpret_cast<size_t>(x);
}

struct MapAllocator : BaseAllocator
{
    HashMap<void*> mPMap;

    MapAllocator() : mPMap(&StdAllocator) {}
    MapAllocator(size_t prealloc) : mPMap(&StdAllocator, prealloc) {}

    virtual void* alloc(size_t memberCount, size_t memberSize) override;
    virtual void free(void* p) override;
    virtual void* realloc(void* p, size_t size) override;
    void freeAll();
};

inline void*
MapAllocator::alloc(size_t memberCount, size_t memberSize)
{
    void* pr = ::calloc(memberCount, memberSize);
    this->mPMap.insert(pr);

    return pr;
};

inline void
MapAllocator::free(void* p)
{
    auto f = this->mPMap.search(p);
    if (f.pData) ::free(*f.pData);
    this->mPMap.remove(f.idx);
};

inline void*
MapAllocator::realloc(void* p, size_t size)
{
    auto pr = ::realloc(p, size);
    if (p != pr)
    {
        auto f = this->mPMap.search(p);
        this->mPMap.remove(f.idx);
        this->mPMap.insert(pr);
        return pr;
    }

    return p;
};

inline void
MapAllocator::freeAll()
{
    for (size_t i = 0; i < this->mPMap.capacity(); i++)
        if (this->mPMap[i].bOccupied)
            ::free(this->mPMap[i].data);

    this->mPMap.free();
}

} /* namespace adt */
