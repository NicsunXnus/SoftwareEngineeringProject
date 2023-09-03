#include "QueryNode.h"

QueryNode::QueryNode(std::vector<std::shared_ptr<SelectStmtNode>> selectStmts,
    std::vector<std::shared_ptr<DeclarationStmtNode>> declnStmts,
    std::vector<std::shared_ptr<ClauseStmtNode>> clauseStmts)
    : selectStmts(std::move(selectStmts)),declnStmts(std::move(declnStmts)),clauseStmts(std::move(clauseStmts))  {}