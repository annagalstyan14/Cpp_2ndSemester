#ifndef EVALUATOR_H
#define EVALUATOR_H

#include "Expression.h"

class Evaluator {
    public:
    static double evaluate(const std::shared_ptr<ExpressionNode>& root);
};

#endif