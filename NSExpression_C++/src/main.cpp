#include "Evaluator.h"
#include "Lexer.h"
#include "Parser.h"
#include "FunctionAnalyzer.h"
#include <iostream>
#include <string>
#include <unordered_map>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <cmath>

int main() {
    std::unordered_map<std::string, double> variables;
    variables["pi"] = 3.14159;
    variables["e"] = 2.71828;
    variables["Na"] = 6.02214076e23;
    variables["g"] = 9.81;

    std::cout << "Enter an expression, equation, or 'quit' to exit.\n";
    std::cout << "Use 'analyze' to analyze a function or 'plot' to graph it.\n";

    while (true) {
        std::cout << "> ";
        std::string input;
        std::getline(std::cin, input);

        if (input == "quit") {
            break;
        }

        try {
            auto trim = [](std::string& s) {
                s.erase(0, s.find_first_not_of(" \t"));
                s.erase(s.find_last_not_of(" \t") + 1);
                return s;
            };
            std::string trimmed_input = trim(input);

            if (trimmed_input.empty()) {
                throw std::runtime_error("Empty input");
            }

            bool isAnalyze = false, isPlot = false;
            std::string expression;

            auto starts_with = [](const std::string& str, const std::string& prefix) {
                if (str.length() < prefix.length()) return false;
                for (size_t i = 0; i < prefix.length(); ++i) {
                    if (std::tolower(str[i]) != std::tolower(prefix[i])) return false;
                }
                return true;
            };

            std::cout << "Debug: Raw input: '" << input << "'\n";
            std::cout << "Debug: Raw input ASCII: ";
            for (char c : input) {
                std::cout << (int)c << " ";
            }
            std::cout << "\n";
            std::cout << "Debug: Trimmed input: '" << trimmed_input << "'\n";

            if (starts_with(trimmed_input, "analyze ") && trimmed_input.length() > 7) {
                isAnalyze = true;
                expression = trimmed_input.substr(7);
                trim(expression);
            } else if (starts_with(trimmed_input, "plot ") && trimmed_input.length() > 5) {
                isPlot = true;
                expression = trimmed_input.substr(5);
                trim(expression);
            } else if (trimmed_input == "analyze" || trimmed_input == "plot") {
                throw std::runtime_error("Command '" + trimmed_input + "' requires an expression");
            } else {
                expression = trimmed_input;
            }

            std::cout << "Debug: Command: " << (isAnalyze ? "analyze" : isPlot ? "plot" : "none") << "\n";
            std::cout << "Debug: Expression: '" << expression << "'\n";

            if (expression.empty()) {
                throw std::runtime_error("No expression provided after command");
            }

            Lexer lexer(expression);
            auto tokens = lexer.tokenize();
            for (const auto& token : tokens) {
                std::cout << "Token: Type = " << static_cast<int>(token.type) << ", Value = " << token.value << "\n";
            }

            Parser parser(tokens, variables);
            auto expr = parser.parse();

            if (isAnalyze) {
                std::cout << "Debug: Starting FunctionAnalyzer for expression\n";
                FunctionAnalyzer analyzer(expr);
                std::cout << "Debug: FunctionAnalyzer constructed\n";

                auto domain = analyzer.getDomain();
                std::cout << "Domain: [" << (std::isinf(-domain.first) ? "-inf" : std::to_string(domain.first))
                          << ", " << (std::isinf(domain.second) ? "inf" : std::to_string(domain.second)) << "]\n";

                auto range = analyzer.getRange();
                std::cout << "Range: [" << (std::isinf(-range.first) ? "-inf" : std::to_string(range.first))
                          << ", " << (std::isinf(range.second) ? "inf" : std::to_string(range.second)) << "]\n";

                std::cout << "Is even? " << (analyzer.isEven() ? "Yes" : "No") << "\n";
                std::cout << "Is odd? " << (analyzer.isOdd() ? "Yes" : "No") << "\n";

                double yIntercept = analyzer.getYAxisIntercept();
                std::cout << "Y-intercept: " << (std::isnan(yIntercept) ? "undefined" : std::to_string(yIntercept)) << "\n";

                auto xIntercepts = analyzer.getXAxisIntercepts();
                std::cout << "X-intercepts: ";
                if (xIntercepts.empty()) {
                    std::cout << "none";
                } else {
                    bool isSin = std::dynamic_pointer_cast<FunctionNode>(expr) &&
                                 std::dynamic_pointer_cast<FunctionNode>(expr)->getName() == "sin";
                    for (const auto& intercept : xIntercepts) {
                        double x = intercept.first;
                        if (isSin) {
                            double k = x / 3.141592653589793;
                            int k_int = std::round(k);
                            if (std::abs(k - k_int) < 1e-6) {
                                if (k_int == 0) {
                                    std::cout << "(0, 0) ";
                                } else if (k_int == 1) {
                                    std::cout << "(pi, 0) ";
                                } else if (k_int == -1) {
                                    std::cout << "(-pi, 0) ";
                                } else {
                                    std::cout << "(" << k_int << "pi, 0) ";
                                }
                            } else {
                                std::cout << "(" << x << ", 0) ";
                            }
                        } else {
                            std::cout << "(" << x << ", " << intercept.second << ") ";
                        }
                    }
                    if (isSin) {
                        std::cout << "(and multiples of pi)";
                    }
                }
                std::cout << "\n";
            } else if (isPlot) {
                FunctionAnalyzer analyzer(expr);
                analyzer.plotAscii(std::cout, 50, 20);
            } else if (expr->isEquation()) {
                auto equation = std::dynamic_pointer_cast<EquationNode>(expr);
                double solution = equation->solveFor("x", variables);
                std::cout << "Solution for x: " << solution << "\n";
            } else {
                Evaluator evaluator;
                double result = evaluator.evaluate(expr, variables);
                std::cout << "Result: " << result << "\n";
            }
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << "\n";
        }
    }

    return 0;
}