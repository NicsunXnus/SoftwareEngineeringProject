#pragma once
#include "ExprNode.h"
#include <memory>

class ModNode : public ExprNode {
public:
    ModNode(std::shared_ptr<ExprNode> leftExpr, std::shared_ptr<ExprNode> rightExpr);

    std::shared_ptr<ExprNode> leftExpr;
    std::shared_ptr<ExprNode> rightExpr;
};