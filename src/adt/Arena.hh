#pragma once

#include <math.h>
#include <string.h>
#include <stddef.h>
#include <threads.h>

#include "allocator.hh"

#define ARENA_FIRST(A) ((A)->pBlocks)
#define ARENA_NEXT(AB) ((AB)->pNext)
#define ARENA_FOREACH(A, IT) for (ArenaBlock* IT = ARENA_FIRST(A); IT; IT = ARENA_NEXT(IT))
#define ARENA_FOREACH_SAFE(A, IT, TMP) for (ArenaBlock* IT = ARENA_FIRST(A), * TMP = nullptr; IT && ((TMP) = ARENA_NEXT(IT), true); (IT) = (TMP))

#define ARENA_NODE_GET_FROM_DATA(PDATA) (reinterpret_cast<adt::ArenaNode*>(reinterpret_cast<u8*>(PDATA) - offsetof(adt::ArenaNode, pData)))
#define ARENA_NODE_GET_FROM_BLOCK(PBLOCK) (reinterpret_cast<adt::ArenaNode*>(reinterpret_cast<u8*>(PBLOCK) + offsetof(adt::ArenaBlock, pData)))

namespace adt
{

struct ArenaNode;

struct ArenaBlock
{
    ArenaBlock* pNext = nullptr;
    u8 pData[];
};

struct ArenaNode
{
    ArenaNode* pNext = nullptr;
    size_t size = 0;
    u8 pData[]; /* flexible array member */
};

struct Arena : __BaseAllocator
{
    ArenaBlock* pBlocks = nullptr;
    size_t blockSize = 0;
    ArenaNode* pLatest = nullptr;
    ArenaBlock* pLatestBlock = nullptr;

    Arena(size_t cap);

    void reset();
    size_t alignedBytes(size_t bytes);
    void* alloc(size_t memberCount, size_t size);
    void free(void* p);
    void* realloc(void* p, size_t size);
    void freeArena();

private:
    ArenaBlock* newBlock();
    bool fitsNode(ArenaNode* pNode, size_t size);
    ArenaBlock* getFreeBlock();
};

inline 
Arena::Arena(size_t cap)
    : blockSize(alignedBytes(cap + sizeof(ArenaNode))) /* preventively align */
{
    this->newBlock();
}

inline void
Arena::reset()
{
    ARENA_FOREACH(this, pB)
    {
        ArenaNode* pNode = ARENA_NODE_GET_FROM_BLOCK(pB), * pNext = nullptr;

        while (pNode->pNext)
        {
            pNext = pNode->pNext;
            pNode->size = 0;
            pNode->pNext = nullptr;
            pNode = pNext;
        }
    }
}

inline ArenaBlock*
Arena::newBlock()
{
    ArenaBlock** ppLastBlock = &this->pBlocks;
    while (*ppLastBlock) ppLastBlock = &((*ppLastBlock)->pNext);

    *ppLastBlock = reinterpret_cast<ArenaBlock*>(malloc(sizeof(ArenaBlock) + this->blockSize));
    memset(*ppLastBlock, 0, sizeof(ArenaBlock) + this->blockSize);

    auto* pNode = ARENA_NODE_GET_FROM_BLOCK(*ppLastBlock);
    this->pLatest = pNode;
    this->pLatestBlock = *ppLastBlock;

    return *ppLastBlock;
}

inline bool
Arena::fitsNode(ArenaNode* pNode, size_t size)
{
    /* TODO: get max contiguous space if we get bunch of freed nodes in the row, while not overflowing */
    return static_cast<size_t>(reinterpret_cast<u8*>(pNode->pNext) - reinterpret_cast<u8*>(pNode)) > size;
}

inline ArenaBlock*
Arena::getFreeBlock()
{
    ArenaBlock* pCurrBlock = this->pBlocks, * pFreeBlock = this->pBlocks;
    while (pCurrBlock)
    {
        pFreeBlock = pCurrBlock;
        pCurrBlock = pCurrBlock->pNext;
    }

    return pFreeBlock;
}

inline size_t
Arena::alignedBytes(size_t bytes)
{
    f64 mulOf = static_cast<f64>(bytes) / static_cast<f64>(sizeof(size_t));
    return sizeof(size_t) * ceil(mulOf);
}

inline void*
Arena::alloc(size_t memberCount, size_t memberSize)
{
    auto* pFreeBlock = this->pLatestBlock;

    auto requested = memberCount * memberSize;
    auto aligned = this->alignedBytes(requested + sizeof(ArenaNode));
    assert(aligned <= this->blockSize && "requested size is > than one block");

repeat:
    /* skip pNext */
    auto* pFreeBlockOff = ARENA_NODE_GET_FROM_BLOCK(pFreeBlock);

    /* find node with pNext == nullptr, this one is free to allocate */
    ArenaNode* pNode = this->pLatest;
    while (pNode->pNext)
    {
        /*size_t off1 = (u8*)pNode - (u8*)pFreeBlockOff;*/
        /*printf("off1: %zu\n", off1);*/

        pNode = pNode->pNext;

        /*size_t off2 = (u8*)pNode - (u8*)pFreeBlockOff;*/
        /*printf("off2: %zu\n", off2);*/
    }

    /* cast to u8* to get correct byte offsets */
    size_t nextAligned = ((u8*)pNode + aligned) - (u8*)pFreeBlockOff;
    /*printf("nextAligned: %zu, (%% 8? %zu)\n", nextAligned, nextAligned % 8);*/

    /* heap overflow */
    if (nextAligned >= this->blockSize)
    {
        pFreeBlock = pFreeBlock->pNext;
        pFreeBlock = this->newBlock();
        goto repeat;
    }

    pNode->pNext = (ArenaNode*)((u8*)pNode + aligned);
    pNode->size = requested;
    this->pLatest = pNode;

    return &pNode->pData;
}

inline void
Arena::free(void* p)
{
    ArenaNode* pNode = ARENA_NODE_GET_FROM_DATA(p);
    pNode->size = 0;
}

inline void*
Arena::realloc(void* p, size_t size)
{
    ArenaNode* pNode = ARENA_NODE_GET_FROM_DATA(p);
    auto aligned = alignedBytes(size);

    /*if (size <= pNode->size || this->fitsNode(pNode, aligned))*/
    /*{*/
    /*    pNode->size = size;*/
    /*    return &pNode->pData;*/
    /*}*/
    /*else*/
    /*{*/
        void* pR = this->alloc(size, 1);
        memcpy(pR, p, pNode->size);
        /*this->free(p);*/
        return pR;
    /*}*/
}

inline void
Arena::freeArena()
{
    ARENA_FOREACH_SAFE(this, it, tmp)
        ::free(it);
}

struct AtomicArena : Arena
{
    mtx_t mtxA;

    void* alloc(size_t memberCount, size_t memberSize);
    void* realloc(void* p, size_t size);
    void free(void* p);
};

inline void*
AtomicArena::alloc(size_t memberCount, size_t memberSize)
{
    mtx_lock(&this->mtxA);
    void* ret = this->Arena::alloc(memberCount, memberSize);
    mtx_unlock(&this->mtxA);

    return ret;
}

inline void*
AtomicArena::realloc(void* p, size_t size)
{
    mtx_lock(&this->mtxA);
    void* ret = this->Arena::realloc(p, size);
    mtx_unlock(&this->mtxA);

    return ret;
}

inline void
AtomicArena::free(void* p)
{
    mtx_lock(&this->mtxA);
    this->Arena::free(p);
    mtx_unlock(&this->mtxA);
}

} /* namespace adt */
