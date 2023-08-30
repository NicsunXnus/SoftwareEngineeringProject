#pragma once
#include <string>
#include <vector>
#include <memory>
#include "ASTNode.h"
#include "StmtNode.h"

class ProcedureNode : public ASTNode {
public:
    ProcedureNode(const std::string& name, std::vector<std::shared_ptr<StmtNode>> stmts);

    std::string name;
    std::vector<std::shared_ptr<StmtNode>> stmts;
};