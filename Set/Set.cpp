#include "Set.hpp"
#include <iostream>

int main() {
    
    Set<int> mySet = {1, 2, 3};
    mySet.insert(4);
    mySet.print(); 

    mySet.erase(2);
    mySet.print(); 

    std::cout << "Contains 3: " << mySet.contains(3) << std::endl;
    std::cout << "Set size: " << mySet.size() << std::endl;

    return 0;
}
