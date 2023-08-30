#pragma once
#include <memory>
#include "StmtNode.h"
#include "VariableNode.h"

class PrintNode : public StmtNode {
public:
    PrintNode(std::shared_ptr<VariableNode> variable);

    std::shared_ptr<VariableNode> variable;
};