#pragma once
#include "RelExprNode"
#include "RelFactorNode"
#include <memory>

class LessThanNode : public RelExprNode {
public:
    LessThanNode(std::shared_ptr<RelFactorNode> leftRelFactor, std::shared_ptr<RelFactorNode> rightRelFactor);

    std::shared_ptr<RelFactorNode> leftRelFactor;
    std::shared_ptr<RelFactorNode> rightRelFactor
};