#pragma once
#include <memory>
#include "StmtNode.h"
#include "VariableNode.h"
#include "ExprNode.h"

class AssignmentNode : public StmtNode {
public:
    AssignmentNode(std::shared_ptr<VariableNode> variable, std::shared_ptr<ExprNode> expr);

    std::shared_ptr<VariableNode> variable;
    std::shared_ptr<ExprNode> expr;
};