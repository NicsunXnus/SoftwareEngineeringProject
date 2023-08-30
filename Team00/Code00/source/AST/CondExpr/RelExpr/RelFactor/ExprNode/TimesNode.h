#pragma once
#include "ExprNode"
#include <memory>

class TimesNode : public ExprNode {
public:
    TimesNode(std::shared_ptr<ExprNode> leftExpr, std::shared_ptr<ExprNode> rightExpr);

    std::shared_ptr<ExprNode> leftExpr;
    std::shared_ptr<ExprNode> rightExpr
};