#pragma once
#include "CondExprNode.h"
#include <memory>

class NotNode : public CondExprNode {
public:
    NotNode(std::shared_ptr<CondExprNode> condExpr);

    std::shared_ptr<CondExprNode> condExpr;
};