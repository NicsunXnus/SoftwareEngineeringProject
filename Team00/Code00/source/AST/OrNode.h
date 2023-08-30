#pragma once
#include "CondExprNode.h"
#include <memory>

class OrNode : public CondExprNode {
public:
    OrNode(std::shared_ptr<CondExprNode> leftCondExpr, std::shared_ptr<CondExprNode> rightCondExpr);

    std::shared_ptr<CondExprNode> leftCondExpr;
    std::shared_ptr<CondExprNode> rightCondExpr;
};