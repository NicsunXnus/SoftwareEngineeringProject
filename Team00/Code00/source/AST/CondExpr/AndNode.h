#pragma once
#include "CondExprNode"
#include <memory>

class AndNode : public CondExprNode {
public:
    AndNode(std::shared_ptr<CondExprNode> leftCondExpr, std::shared_ptr<CondExprNode> rightCondExpr);

    std::shared_ptr<CondExprNode> leftCondExpr;
    std::shared_ptr<CondExprNode> rightCondExpr
};