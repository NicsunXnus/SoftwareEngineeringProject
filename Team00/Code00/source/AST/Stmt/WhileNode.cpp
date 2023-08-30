#include "WhileNode.h"
WhileNode::WhileNode(std::shared_ptr<CondExprNode> conditionExpr,
               std::vector<std::shared_ptr<StmtNode>> loop)
    : conditionExpr(conditionExpr), loop(loop) {}