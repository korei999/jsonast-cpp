#pragma once

#include <stdlib.h>
#include <assert.h>

#include "ultratypes.h"

namespace adt
{

using Nullptr = decltype(nullptr);

constexpr size_t SIZE_MIN = 8UL;
constexpr size_t SIZE_1K = 1024UL;
constexpr size_t SIZE_8K = 8 * SIZE_1K;
constexpr size_t SIZE_1M = SIZE_1K * SIZE_1K; 
constexpr size_t SIZE_8M = 8 * SIZE_1M; 
constexpr size_t SIZE_1G = SIZE_1M * SIZE_1K; 

struct __BaseAllocator
{
    void* alloc(size_t memberCount, size_t memberSize);
    void free(void* p);
    void* realloc(void* p, size_t size);
};

struct DefaultAllocator : __BaseAllocator
{
    void* alloc(size_t memberCount, size_t memberSize) { return ::calloc(memberCount, memberSize); }
    void free(void* p) { ::free(p); }
    void* realloc(void* p, size_t size) { return ::realloc(p, size); }
};

inline DefaultAllocator StdAllocator {};

} /* namespace adt */
