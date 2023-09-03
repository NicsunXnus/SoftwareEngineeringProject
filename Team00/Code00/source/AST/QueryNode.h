#pragma once
#include <vector>
#include <memory>
#include "ASTNode.h"
#include "SelectStmtNode.h"
#include "DeclarationStmtNode.h"
#include "ClauseStmtNode.h"

class QueryNode : public ASTNode {
public:
    QueryNode(std::vector<std::shared_ptr<SelectStmtNode>> selectStmts,
    std::vector<std::shared_ptr<DeclarationStmtNode>> declnStmts,
    std::vector<std::shared_ptr<ClauseStmtNode>> clauseStmts);
    std::vector<std::shared_ptr<SelectStmtNode>> selectStmts;
    std::vector<std::shared_ptr<DeclarationStmtNode>> declnStmts;
    std::vector<std::shared_ptr<ClauseStmtNode>> clauseStmts;
};