#include "NotNode.h"
NotNode::NotNode(std::shared_ptr<CondExprNode> leftCondExpr,
               std::shared_ptr<CondExprNode> rightCondExpr)
    : leftCondExpr(conditionExpr), rightCondExpr(conditionExpr) {}