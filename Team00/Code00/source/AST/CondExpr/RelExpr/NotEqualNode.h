#pragma once
#include "RelExprNode"
#include "RelFactorNode"
#include <memory>

class NotEqualNode : public RelExprNode {
public:
    NotEqualNode(std::shared_ptr<RelFactorNode> leftRelFactor, std::shared_ptr<RelFactorNode> rightRelFactor);

    std::shared_ptr<RelFactorNode> leftRelFactor;
    std::shared_ptr<RelFactorNode> rightRelFactor
};