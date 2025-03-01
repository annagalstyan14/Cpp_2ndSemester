#pragma once
#ifndef MEMORYPOOL_H
#define MEMORYPOOL_H

#include <vector>
#include <cstddef>

class MemoryPool {
private: 
	static const std::size_t BlockSize = 1024;
	std::vector<void*> blocks;
	std::size_t currentIndex;
	std::size_t objectSize;

	void allocateNewBlock();

public:
	explicit MemoryPool(std::size_t objSize);
	~MemoryPool();

	void* allocate();
	void deallocateAll();
};

#endif 

