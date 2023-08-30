#include "TimesNode.h"
TimesNode::TimesNode(std::shared_ptr<ExprNode> leftExpr,
               std::shared_ptr<ExprNode> rightExpr)
    : leftExpr(leftExpr), rightExpr(rightExpr) {}