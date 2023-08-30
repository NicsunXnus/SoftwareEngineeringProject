#pragma once
#include "RelExprNode.h"
#include "RelFactorNode.h"
#include <memory>

class GreaterThanEqualNode : public RelExprNode {
public:
    GreaterThanEqualNode(std::shared_ptr<RelFactorNode> leftRelFactor, std::shared_ptr<RelFactorNode> rightRelFactor);

    std::shared_ptr<RelFactorNode> leftRelFactor;
    std::shared_ptr<RelFactorNode> rightRelFactor;
};