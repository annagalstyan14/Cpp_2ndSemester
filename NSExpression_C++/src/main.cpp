#include <iostream>
#include <string>
#include <unordered_map>
#include <fstream>
#include "Lexer.h"
#include "Parser.h"
#include "Evaluator.h"
#include "FunctionAnalyzer.h"

int main() {
    std::string input;
    std::unordered_map<std::string, double> variables;

    // Initialize constants
    variables["pi"] = 3.14159;
    variables["e"] = 2.71828;
    variables["g"] = 9.81;
    variables["Na"] = 6.02214076e23;
    variables["k"] = 1.380649e-23;
    variables["h"] = 6.62607015e-34;
    variables["q"] = 1.602176634e-19;

    std::cout << "Enter an expression, equation, or 'quit' to exit.\n";
    std::cout << "Use 'analyze' to analyze a function or 'plot' to graph it.\n";
    std::cout << "> ";
    while (std::getline(std::cin, input)) {
        if (input == "quit") break;

        try {
            if (input == "analyze" || input == "plot") {
                std::cout << "Enter function to " << input << " (e.g., x^2, sin(x)): ";
                std::getline(std::cin, input);
                if (input == "quit") break;
            }

            Lexer lexer(input);
            std::vector<Token> tokens = lexer.tokenize();
            for (const auto& token : tokens) {
                std::cout << "Token: Type = " << static_cast<int>(token.type) << ", Value = " << token.value << std::endl;
            }
            Parser parser(tokens, variables);
            auto expr = parser.parse();

            if (input == "analyze") {
                FunctionAnalyzer analyzer(expr);
                auto domain = analyzer.getDomain();
                auto range = analyzer.getRange();
                std::cout << "Domain: [" << domain.first << ", " << domain.second << "]\n";
                std::cout << "Range: [" << range.first << ", " << range.second << "]\n";
                std::cout << "Is even? " << (analyzer.isEven() ? "Yes" : "No") << "\n";
                std::cout << "Is odd? " << (analyzer.isOdd() ? "Yes" : "No") << "\n";
                std::cout << "Y-intercept: " << analyzer.getYAxisIntercept() << "\n";
                auto xIntercepts = analyzer.getXAxisIntercepts();
                std::cout << "X-intercepts: ";
                for (double x : xIntercepts) std::cout << x << " ";
                std::cout << "\n";
            } else if (input == "plot") {
                FunctionAnalyzer analyzer(expr);
                analyzer.plotAscii(std::cout);
            } else if (expr->isEquation()) {
                auto equation = std::dynamic_pointer_cast<EquationNode>(expr);
                double solution = equation->solveFor("x", variables);
                std::cout << "Solution for x: " << solution << "\n";
            } else {
                double result = Evaluator::evaluate(expr, variables);
                std::cout << "Result: " << result << "\n";
            }
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << "\n";
        }

        std::cout << "\n> ";
    }

    return 0;
}