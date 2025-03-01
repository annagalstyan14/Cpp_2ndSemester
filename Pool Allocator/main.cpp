
#include "PoolAllocator.h"
#include <iostream>

class Person {
public:
    std::string name;
    int age;

    Person() : name("Unknown"), age(0) {
        std::cout << "Person created: " << name << "\n";
    }

    ~Person() {
        std::cout << "Person destroyed: " << name << "\n";
    }
};

int main() {
    PoolAllocator<Person> allocator;

    // Allocate objects
    Person* p1 = allocator.new_element();
    Person* p2 = allocator.new_element();

    p1->name = "Alice";
    p1->age = 25;
    p2->name = "Bob";
    p2->age = 30;

    std::cout << p1->name << ", " << p1->age << '\n';
    std::cout << p2->name << ", " << p2->age << '\n';

    // Destroy only one object
    allocator.destroy_element(p1);

    // The other object (p2) still exists
    std::cout << "After destroying p1: " << p2->name << ", " << p2->age << '\n';

        // Reset allocator (deallocate all remaining objects)
        allocator.reset();

    return 0;
}
