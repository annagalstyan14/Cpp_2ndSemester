#include "Optional.hpp"
#include <iostream>

int main() {
    
    Optional<int> a;  
    Optional<int> b(42);  

    if (b.is_present()) {
        std::cout << "Value: " << *b << std::endl;
    }

    std::cout << "a value_or (default): " << a.value_or(10) << std::endl;
    std::cout << "b value_or (default): " << b.value_or(10) << std::endl;

    auto c = b.map([](int x) { return x * 2; });
    std::cout << "Mapped value: " << *c << std::endl;

    return 0;
}
