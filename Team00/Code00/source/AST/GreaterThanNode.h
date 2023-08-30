#pragma once
#include "RelExprNode.h"
#include "RelFactorNode.h"
#include <memory>

class GreaterThanNode : public RelExprNode {
public:
    GreaterThanNode(std::shared_ptr<RelFactorNode> leftRelFactor, std::shared_ptr<RelFactorNode> rightRelFactor);

    std::shared_ptr<RelFactorNode> leftRelFactor;
    std::shared_ptr<RelFactorNode> rightRelFactor;
};