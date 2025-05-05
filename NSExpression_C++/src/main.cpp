#include <iostream>
#include "Lexer.h"
#include "Parser.h"
#include "Evaluator.h"
#include <map>
#include <cmath>

int main() {
    std::string input;
    
    // Predefined constants
    std::map<std::string, double> variables = {
        {"pi", 3.14159},
        {"e", std::exp(1)},           // Euler's number
        {"g", 9.81},                 // Gravitational constant (m/s^2)
        {"Na", 6.02214076e23},       // Avogadro's number
        {"k", 1.380649e-23},         // Boltzmann constant (J/K)
        {"h", 6.62607015e-34},       // Planck's constant (J·s)
        {"q", 1.602176634e-19},
        {"c", 299792458}       // Elementary charge (Coulombs)
    };
    
    std::cout << "Enter an expression (or 'quit' to exit): ";
    
    while (std::getline(std::cin, input)) {
        if (input == "quit") break;
        
        try {
            Lexer lexer(input);
            Parser parser(lexer);
            auto expr = parser.parse();
            
            // Evaluate the expression using predefined constants and any other variables
            double result = expr->evaluate(variables);
            std::cout << "Result: " << result << std::endl;
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << std::endl;
        }
        
        std::cout << "\nEnter an expression (or 'quit' to exit): ";
    }
    
    return 0;
}
