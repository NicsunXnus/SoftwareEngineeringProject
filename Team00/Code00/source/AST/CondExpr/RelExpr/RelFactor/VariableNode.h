#pragma once
#include <string>
#include "RefNode.h"
#include "RelFactorNode.h"

class VariableNode : public RefNode, public RelFactorNode {
public:
    VariableNode(const std::string& name);

    std::string name;
};