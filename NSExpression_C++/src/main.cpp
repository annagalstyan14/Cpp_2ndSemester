#include <iostream>
#include <string>
#include <unordered_map>
#include "Lexer.h"
#include "Parser.h"
#include "Evaluator.h"

int main() {
    std::string input;
    std::unordered_map<std::string, double> variables;

    // Add constants to the variables map
    variables["pi"] = 3.14159;
    variables["e"] = 2.71828;
    variables["g"] = 9.81; // Gravitational constant
    variables["Na"] = 6.02214076e23; // Avogadro's number
    variables["k"] = 1.380649e-23; // Boltzmann constant
    variables["h"] = 6.62607015e-34; // Planck's constant
    variables["q"] = 1.602176634e-19; // Elementary charge

    std::cout << "Enter an expression (or 'quit' to exit): ";
    while (std::getline(std::cin, input)) {
        if (input == "quit") break;
        
        try {
            Lexer lexer(input);  // Create a Lexer to tokenize the input
            std::vector<Token> tokens = lexer.tokenize(); 
             // Get the tokens from the lexer
             for (const auto& token : tokens) {
                std::cout << "Token: Type = " << static_cast<int>(token.type) << ", Value = " << token.value << std::endl;
            }
            Parser parser(tokens, variables);  // Pass the tokens and constants to the Parser
            auto expr = parser.parse();  // Parse the tokens into an expression
            
            double result = expr->evaluate(static_cast<const std::unordered_map<std::string, double>&>(variables));
  // Evaluate the expression with the variables map
            std::cout << "Result: " << result << std::endl;
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << std::endl;
        }
        
        std::cout << "\nEnter an expression (or 'quit' to exit): ";
    }
    
    return 0;
}
