#pragma once
#include <memory>
#include "StmtNode.h"
#include "VariableNode.h"

class ReadNode : public StmtNode {
public:
    ReadNode(std::shared_ptr<VariableNode> variable);

    std::shared_ptr<VariableNode> variable;
};