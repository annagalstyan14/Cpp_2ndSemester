#include <iostream>
#include "Lexer.h"
#include "Parser.h"
#include <map>

int main() {
    std::string input;
    std::map<std::string, double> variables;
    
    std::cout << "Enter an expression (or 'quit' to exit): ";
    while (std::getline(std::cin, input)) {
        if (input == "quit") break;
        
        try {
            Lexer lexer(input);
            Parser parser(lexer);
            auto expr = parser.parse();
            
            // Set some test variables
            variables["x"] = 5.0;
            variables["y"] = 10.0;
            variables["pi"] = 3.14159;
            
            double result = expr->evaluate(variables);
            std::cout << "Result: " << result << std::endl;
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << std::endl;
        }
        
        std::cout << "\nEnter an expression (or 'quit' to exit): ";
    }
    
    return 0;
}
