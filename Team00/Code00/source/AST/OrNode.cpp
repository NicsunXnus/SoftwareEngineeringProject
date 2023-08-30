#include "OrNode.h"
OrNode::OrNode(std::shared_ptr<CondExprNode> leftCondExpr,
               std::shared_ptr<CondExprNode> rightCondExpr)
    : leftCondExpr(leftCondExpr), rightCondExpr(rightCondExpr) {}