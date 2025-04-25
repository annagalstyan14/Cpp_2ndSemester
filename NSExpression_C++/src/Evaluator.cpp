#include "Evaluator.h"
#include <stdexcept>

double Evaluator::evaluate(const std::shared_ptr<ExpressionNode>& root) {
    if (!root){
        throw std::runtime_error("Null expression node provided");
    }
    return root->evaluate();
}