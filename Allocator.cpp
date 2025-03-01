// Allocator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <new>
using namespace std;

template <typename T>
class Allocator {
private:
    static const size_t MemorySize = 1024;
    T* memoryBlock;
    bool allocated;
    size_t ObjectCount;
public:
    Allocator() : memoryBlock(nullptr), allocated(false), ObjectCount(MemorySize / sizeof(T)) {}

    T* allocate() {
        if (allocated) {
            cerr << "Allocation failed: Memory already allocated.\n";
            return nullptr;
        }

        try {
            memoryBlock = static_cast<T*>(::operator new(MemorySize));
            allocated = true;
        }
        catch (const bad_alloc&) {
            cerr << "Allocation failed: Not enough memory.\n";
            return nullptr;
        }
        return memoryBlock;
    }
    void construct(size_t index) {
        if (!allocated) {
            cerr << "Construction failed: No memory allocated.\n";
            return;
        }
        if (index >= ObjectCount) {
            cerr << "Construction failed: Index out of bounds.\n";
            return;
        }
        new (memoryBlock + index) T();
    }

    void destroy(size_t index) {
        if (!allocated) {
            cerr << "Destruction failed: No memory allocated.\n";
            return;
        }
        if (index >= ObjectCount) {
            cerr << "Destruction failed: Index out of bounds.\n";
            return;
        }
        memoryBlock[index].~T();
    }
    void deallocate() {
        if (!allocated) {
            cerr << "Deallocation failed: No memory to free.\n";
            return;
        }
        ::operator delete(memoryBlock);
        memoryBlock = nullptr;
        allocated = false;
    }
    ~Allocator() {
        if (allocated) {
            deallocate();

        }
    }
};

class Person {
public:
    string name;
    int age;

    Person() : name("Unknown"), age(0) {
        cout << "Default person created. \n";
    }
    ~Person() {
        cout << "Person " << name << " destroyed.\n";
    }
};

int main()
{
    Allocator<Person> allocator;
    Person* ptr = allocator.allocate();

    if (ptr) {
        cout << "Memory allocated for Person.\n";

        allocator.construct(0);
        allocator.construct(1);

        cout << ptr[0].name << ", " << ptr[0].age << '\n';
        cout << ptr[1].name << ", " << ptr[1].age << '\n';

        allocator.destroy(0);
        allocator.destroy(1);
    }
    allocator.deallocate();
    cout << "Memory deallocated.\n";
    return 0;
}
