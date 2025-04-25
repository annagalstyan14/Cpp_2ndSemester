#include <iostream>
#include <string>
#include "Lexer.h"
#include "Parser.h"
#include "Expression.h"

int main() {
    SymbolTable symbolTable;
    std::string input;

    std::cout << "Enter an expression (or 'quit' to exit): ";
    std::getline(std::cin, input);

    while (input != "quit") {
        try {
            // Trim input
            input.erase(0, input.find_first_not_of(" \t\n\r"));
            input.erase(input.find_last_not_of(" \t\n\r") + 1);

            size_t eqPos = input.find('=');
            if (eqPos != std::string::npos) {
                // Handle assignment: e.g., x = 5 + 2
                std::string varName = input.substr(0, eqPos);
                std::string exprStr = input.substr(eqPos + 1);

                // Trim both sides
                varName.erase(0, varName.find_first_not_of(" \t\n\r"));
                varName.erase(varName.find_last_not_of(" \t\n\r") + 1);
                exprStr.erase(0, exprStr.find_first_not_of(" \t\n\r"));
                exprStr.erase(exprStr.find_last_not_of(" \t\n\r") + 1);

                Lexer lexer(exprStr);
                std::vector<Token> tokens = lexer.tokenize();

                Parser parser(tokens);
                auto expr = parser.parse();

                double result = expr->evaluate(symbolTable);
                symbolTable.set(varName, result);

                std::cout << varName << " = " << result << std::endl;

            } else {
                // Handle regular expression
                Lexer lexer(input);
                std::vector<Token> tokens = lexer.tokenize();

                Parser parser(tokens);
                auto expr = parser.parse();

                double result = expr->evaluate(symbolTable);
                std::cout << "Result: " << result << std::endl;
            }

        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }

        std::cout << "\nEnter an expression (or 'quit' to exit): ";
        std::getline(std::cin, input);
    }

    return 0;
}
