#pragma once
#include <memory>
#include <vector>
#include "StmtNode.h"
#include "CondExprNode.h"

class IfNode : public StmtNode {
public:
    IfNode(std::shared_ptr<CondExpr> conditionExpr,
           std::vector<std::shared_ptr<StmtNode>> trueBranch,
           std::vector<std::shared_ptr<StmtNode>> falseBranch);
    
    std::shared_ptr<CondExprNode> conditionExpr;
    std::vector<std::shared_ptr<StmtNode>> trueBranch;
    std::vector<std::shared_ptr<StmtNode>> falseBranch;
};