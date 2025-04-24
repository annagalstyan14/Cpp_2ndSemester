#include <iostream>
#include "Lexer.h"
#include "Parser.h"
#include <string>

int main() {
    std::string input;
    std::cout << "Enter an expression (or 'quit' to exit): ";
    std::getline(std::cin, input);

    while (input != "quit") {
        try {
            // Tokenize the input
            Lexer lexer(input);
            std::vector<Token> tokens = lexer.tokenize();

            // Parse the tokens
            Parser parser(tokens);
            auto expr = parser.parse();

            // Evaluate and display result
            double result = expr->evaluate();
            std::cout << "Result: " << result << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }

        std::cout << "\nEnter an expression (or 'quit' to exit): ";
        std::getline(std::cin, input);
    }

    return 0;
}
