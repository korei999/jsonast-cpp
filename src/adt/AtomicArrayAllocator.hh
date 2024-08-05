#pragma once

#include <threads.h>

#include "ArrayAllocator.hh"

namespace adt
{

struct AtomicArrayAllocator : Allocator
{
    mtx_t _mtx;
    ArrayAllocator _lAlloc;

    AtomicArrayAllocator() { mtx_init(&_mtx, mtx_plain); }
    AtomicArrayAllocator(u32 prealloc) : _lAlloc(prealloc) { mtx_init(&_mtx, mtx_plain); }

    virtual void* alloc(size_t memberCount, size_t memberSize) override final;
    virtual void free(void* p) override final;
    virtual void* realloc(void* p, size_t size) override final;
    void freeAll();
};

inline void*
AtomicArrayAllocator::alloc(size_t memberCount, size_t memberSize)
{
    mtx_lock(&_mtx);
    void* r = _lAlloc.alloc(memberCount, memberSize);
    mtx_unlock(&_mtx);

    return r;
}

inline void
AtomicArrayAllocator::free(void* p)
{
    mtx_lock(&_mtx);
    _lAlloc.free(p);
    mtx_unlock(&_mtx);
}

inline void*
AtomicArrayAllocator::realloc(void* p, size_t size)
{
    mtx_lock(&_mtx);
    void* r = _lAlloc.realloc(p, size);
    mtx_unlock(&_mtx);

    return r;
}

inline void
AtomicArrayAllocator::freeAll()
{
     _lAlloc.freeAll();
     mtx_destroy(&_mtx);
}

} /* namespace adt */
