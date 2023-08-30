#pragma once
#include "ExprNode"
#include <memory>

class MinusNode : public ExprNode {
public:
    MinusNode(std::shared_ptr<ExprNode> leftExpr, std::shared_ptr<ExprNode> rightExpr);

    std::shared_ptr<ExprNode> leftExpr;
    std::shared_ptr<ExprNode> rightExpr
};