#pragma once

#include "allocator.hh"

namespace adt
{

template<typename T>
struct ArrayIt
{
}

template<typename T>
struct Array
{
    BaseAllocator* allocator;
    T* pData = nullptr;
    size_t size = 0;
    size_t capacity = 0;

    Array(BaseAllocator* _allocator);
    Array(BaseAllocator* _allocator, size_t _capacity);

    T& operator[](size_t i) { return this->pData[i]; }

    T* push(const T& data);
    T& back();
    T& front();
    T* data() { return this->pData; }
    bool empty() const { return size == 0;  }
    void reallocate(size_t _size);
    void free() { this->allocator->free(this->pData); }
};

template<typename T>
Array<T>::Array(BaseAllocator* _allocator)
    : allocator(_allocator), capacity(SIZE_MIN)
{
    pData = static_cast<T*>(this->allocator->alloc(this->capacity, sizeof(T)));
}

template<typename T>
Array<T>::Array(BaseAllocator* _allocator, size_t _capacity)
    : allocator(_allocator), capacity(_capacity)
{
    pData = static_cast<T*>(this->allocator->alloc(this->capacity, sizeof(T)));
}

template<typename T>
T*
Array<T>::push(const T& data)
{
    if (this->size >= this->capacity)
        this->reallocate(this->capacity * 2);

    this->pData[this->size++] = data;

    return &this->back();
}

    template<typename T>
    T&
Array<T>::back()
{
    return this->pData[this->size - 1];
}

template<typename T>
T&
Array<T>::front()
{
    return this->pData[0];
}

template<typename T>
void
Array<T>::reallocate(size_t _size)
{
    this->capacity = _size;
    this->pData = static_cast<T*>(this->allocator->realloc(this->pData, sizeof(T) * _size));

    /*memset(&this->pData[this->size], 0, sizeof(T) * (this->capacity - this->size));*/
}

} /* namespace adt */
