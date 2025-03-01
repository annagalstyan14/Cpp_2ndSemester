#pragma once

#include "MemoryPool.h"
#include <new>

template <typename T>
class PoolAllocator {
private:
    MemoryPool pool;

public:
    PoolAllocator();  // Constructor declaration
    T* new_element();  // new_element declaration
    void destroy_element(T* ptr);  // destroy_element declaration
    void reset();  // reset declaration
};

// Constructor definition
template <typename T>
PoolAllocator<T>::PoolAllocator() : pool(sizeof(T)) {}

// new_element function definition
template <typename T>
T* PoolAllocator<T>::new_element() {
    void* memory = pool.allocate();
    return new (memory) T();  // Placement new to construct object in allocated memory
}

// destroy_element function definition
template <typename T>
void PoolAllocator<T>::destroy_element(T* ptr) {
    if (ptr) {
        ptr->~T();  // Explicitly call the destructor
    }
}

// reset function definition
template <typename T>
void PoolAllocator<T>::reset() {
    pool.deallocateAll();
}

