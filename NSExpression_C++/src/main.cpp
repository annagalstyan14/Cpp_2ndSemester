#include <iostream>
#include <string>
#include <sstream>
#include "Lexer.h"
#include "Parser.h"
#include "Expression.h"

int main() {
    std::string input;

    while (true) {
        std::cout << "Enter an expression (or 'quit' to exit): ";
        std::getline(std::cin, input);
        
        // Check for EOF or empty input
        if (std::cin.eof() || input.empty()) {
            std::cout << "Exiting..." << std::endl;
            break;
        }

        // Trim input
        input.erase(0, input.find_first_not_of(" \t\n\r"));
        input.erase(input.find_last_not_of(" \t\n\r") + 1);

        if (input == "quit") {
            std::cout << "Quitting..." << std::endl;
            break;
        }

        try {
            Lexer lexer(input);
            std::vector<Token> tokens = lexer.tokenize();

            Parser parser(tokens);
            auto expr = parser.parse();

            double result = expr->evaluate();
            std::cout << "Result: " << result << std::endl;

        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }

    return 0;
}
