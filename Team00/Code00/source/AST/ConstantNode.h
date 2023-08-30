#pragma once
#include "RefNode.h"
#include "RelFactorNode.h"

class ConstantNode : public RefNode, public RelFactorNode {
public:
    ConstantNode(int value);

    int value;
};