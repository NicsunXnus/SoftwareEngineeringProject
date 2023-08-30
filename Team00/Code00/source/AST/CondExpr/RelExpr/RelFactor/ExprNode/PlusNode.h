#pragma once
#include "ExprNode"
#include <memory>

class PlusNode : public ExprNode {
public:
    PlusNode(std::shared_ptr<ExprNode> leftExpr, std::shared_ptr<ExprNode> rightExpr);

    std::shared_ptr<ExprNode> leftExpr;
    std::shared_ptr<ExprNode> rightExpr
};