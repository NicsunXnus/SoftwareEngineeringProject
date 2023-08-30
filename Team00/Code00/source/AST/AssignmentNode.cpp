#include "AssignmentNode.h"

AssignmentNode::AssignmentNode(std::shared_ptr<VariableNode> variable, std::shared_ptr<ExprNode> expr)
    : variable(variable), expr(expr) {}
