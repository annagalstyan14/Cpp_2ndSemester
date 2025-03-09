#include "MemoryPool.h"
#include <new>
#include <stdexcept>

MemoryPool::MemoryPool(std::size_t objSize, std::size_t blockSize) 
    : BlockSize(blockSize), currentIndex(0), objectSize(objSize) {
    allocateNewBlock();
}

MemoryPool::~MemoryPool() {
    try {
        for (void* block : blocks) {
            ::operator delete(block);
        }
    } catch (...) {
    }
}

void* MemoryPool::allocate() {
    std::lock_guard<std::mutex> lock(mtx);

    if (!freeList.empty()) {
        void* ptr = freeList.back();
        freeList.pop_back();
        return ptr;
    }

    if (currentIndex >= BlockSize / objectSize) {
        allocateNewBlock();
    }

    void* ptr = static_cast<char*>(blocks.back()) + (currentIndex * objectSize);
    currentIndex++;
    return ptr;
}

void MemoryPool::deallocate(void* ptr) {
    std::lock_guard<std::mutex> lock(mtx);
    freeList.push_back(ptr);
}

void MemoryPool::deallocateAll() {
    std::lock_guard<std::mutex> lock(mtx); 
    currentIndex = 0;
    freeList.clear();
}

void MemoryPool::allocateNewBlock() {
    void* newBlock = ::operator new(BlockSize, std::align_val_t{alignof(std::max_align_t)});
    if (!newBlock) {
        throw std::bad_alloc(); 
    }

    blocks.push_back(newBlock);
    currentIndex = 0;
}

std::size_t MemoryPool::getAllocatedBlocks() const {
    return blocks.size();
}

std::size_t MemoryPool::getFreeBlocks() const {
    return freeList.size();
}
