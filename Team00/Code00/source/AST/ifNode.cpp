#include "IfNode.h"

IfNode::IfNode(std::shared_ptr<CondExprNode> conditionExpr,
               std::vector<std::shared_ptr<StmtNode>> trueBranch,
               std::vector<std::shared_ptr<StmtNode>> falseBranch)
    : conditionExpr(conditionExpr), trueBranch(trueBranch), falseBranch(falseBranch) {}