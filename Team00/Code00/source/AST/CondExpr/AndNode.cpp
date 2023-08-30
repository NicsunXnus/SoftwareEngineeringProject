#include "AndNode.h"
AndNode::AndNode(std::shared_ptr<CondExprNode> leftCondExpr,
               std::shared_ptr<CondExprNode> rightCondExpr)
    : leftCondExpr(conditionExpr), rightCondExpr(conditionExpr) {}