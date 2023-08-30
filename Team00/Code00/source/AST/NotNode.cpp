#include "NotNode.h"
NotNode::NotNode(std::shared_ptr<CondExprNode> condExpr)
    : condExpr(condExpr) {}