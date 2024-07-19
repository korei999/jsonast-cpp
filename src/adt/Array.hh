#pragma once

#include "allocator.hh"

namespace adt
{

template<typename T, typename ALLOC>
struct Array
{
    ALLOC* allocator;
    T* pData = nullptr;
    size_t size = 0;
    size_t capacity = 0;

    Array(ALLOC* _allocator);
    Array(ALLOC* _allocator, size_t _capacity);

    T& operator[](size_t i) { return this->pData[i]; }

    T* push(const T& data);
    T& back();
    T& front();
    T* data() { return this->pData; }
    bool empty() const { return size == 0;  }
    void reallocate(size_t _size);
    void free() { this->allocator->free(this->pData); }
};

template<typename T, typename ALLOC>
Array<T, ALLOC>::Array(ALLOC* _allocator)
    : allocator(_allocator), capacity(SIZE_MIN)
{
    pData = static_cast<T*>(this->allocator->alloc(this->capacity, sizeof(T)));
}

template<typename T, typename ALLOC>
Array<T, ALLOC>::Array(ALLOC* _allocator, size_t _capacity)
    : allocator(_allocator), capacity(_capacity)
{
    pData = static_cast<T*>(this->allocator->alloc(this->capacity, sizeof(T)));
}

template<typename T, typename ALLOC>
T*
Array<T, ALLOC>::push(const T& data)
{
    if (this->size >= this->capacity - 1)
        this->reallocate(this->capacity * 2);

    this->pData[this->size++] = data;

    return &this->back();
}

template<typename T, typename ALLOC>
T&
Array<T, ALLOC>::back()
{
    return this->pData[this->size - 1];
}

template<typename T, typename ALLOC>
T&
Array<T, ALLOC>::front()
{
    return this->pData[0];
}

template<typename T, typename ALLOC>
void
Array<T, ALLOC>::reallocate(size_t _size)
{
    this->capacity = _size;
    this->pData = static_cast<T*>(this->allocator->realloc(this->pData, sizeof(T) * _size));

    /*memset(&this->pData[this->size], 0, sizeof(T) * (this->capacity - this->size));*/
}

} /* namespace adt */
