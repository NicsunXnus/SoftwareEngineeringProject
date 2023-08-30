#pragma once
#include "CondExprNode"
#include <memory>

class OrNode : public CondExprNode {
public:
    OrNode(std::shared_ptr<CondExprNode> leftCondExpr, std::shared_ptr<CondExprNode> rightCondExpr);

    std::shared_ptr<CondExprNode> leftCondExpr;
    std::shared_ptr<CondExprNode> rightCondExpr
};