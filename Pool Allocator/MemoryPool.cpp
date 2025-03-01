#include "MemoryPool.h"
#include <new>

MemoryPool::MemoryPool(std::size_t objSize) : currentIndex(0), objectSize(objSize) {
	allocateNewBlock();
}

MemoryPool::~MemoryPool() {
	for (void* block : blocks) {
		::operator delete(block);
	}
}

void* MemoryPool::allocate() {
	if (currentIndex >= BlockSize / objectSize) {
		allocateNewBlock();
	}
	void* ptr = static_cast<char*>(blocks.back()) + (currentIndex * objectSize);
	currentIndex++;
	return ptr;
}

void MemoryPool::deallocateAll() {
	currentIndex = 0;
}

void MemoryPool::allocateNewBlock() {
	void* newBlock = ::operator new(BlockSize);
	blocks.push_back(newBlock);
	currentIndex = 0;
}