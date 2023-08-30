#include "AndNode.h"
AndNode::AndNode(std::shared_ptr<CondExprNode> leftCondExpr,
               std::shared_ptr<CondExprNode> rightCondExpr)
    : leftCondExpr(leftCondExpr), rightCondExpr(rightCondExpr) {}