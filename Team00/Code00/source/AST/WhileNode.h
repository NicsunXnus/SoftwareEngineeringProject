#pragma once
#include <memory>
#include <vector>
#include "StmtNode.h"
#include "CondExprNode.h"

class WhileNode : public StmtNode {
public:
    WhileNode(std::shared_ptr<CondExprNode> conditionExpr,
           std::vector<std::shared_ptr<StmtNode>> loop);
    
    std::shared_ptr<CondExprNode> conditionExpr;
    std::vector<std::shared_ptr<StmtNode>> loop;
};