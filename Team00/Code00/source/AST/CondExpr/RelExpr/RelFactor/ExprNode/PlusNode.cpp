#include "PlusNode.h"
PlusNode::PlusNode(std::shared_ptr<ExprNode> leftExpr,
               std::shared_ptr<ExprNode> rightExpr)
    : leftExpr(leftExpr), rightExpr(rightExpr) {}