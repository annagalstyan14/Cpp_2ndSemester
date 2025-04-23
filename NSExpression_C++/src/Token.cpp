#include "Token.h"
#include <iostream>


void Token::displayToken() const {
    std::cout << "Token Type: " << static_cast<int>(type)
              << ", Value: " << value
              << ", Precedence: " << static_cast<int>(precedence)
              << std::endl;
}
