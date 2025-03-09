#pragma once
#ifndef MEMORYPOOL_H
#define MEMORYPOOL_H

#include <vector>
#include <cstddef>
#include <mutex>

class MemoryPool {
private:
    std::size_t BlockSize;
    std::vector<void*> blocks;
    std::size_t currentIndex;
    std::size_t objectSize;
    std::vector<void*> freeList;
    std::mutex mtx;

    void allocateNewBlock();
    void* allocateFromFreeList();

public:
    explicit MemoryPool(std::size_t objSize, std::size_t blockSize = 1024);
    ~MemoryPool();

    void* allocate();
    void deallocate(void* ptr);s
    void deallocateAll();
    std::size_t getAllocatedBlocks() const; 
    std::size_t getFreeBlocks() const; 
};

#endif


