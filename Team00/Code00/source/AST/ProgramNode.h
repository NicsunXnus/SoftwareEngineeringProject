#pragma once
#include <vector>
#include <memory>
#include "ASTNode.h"
#include "ProcedureNode.h"

class ProgramNode : public ASTNode {
public:
    ProgramNode(std::vector<std::shared_ptr<ProcedureNode>> procedures);

    std::vector<std::shared_ptr<ProcedureNode>> procedures;
};