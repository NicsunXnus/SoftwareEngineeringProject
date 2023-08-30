#pragma once
#include "RelExprNode.h"
#include "RelFactorNode.h"
#include <memory>

class LessThanEqualNode : public RelExprNode {
public:
    LessThanEqualNode(std::shared_ptr<RelFactorNode> leftRelFactor, std::shared_ptr<RelFactorNode> rightRelFactor);

    std::shared_ptr<RelFactorNode> leftRelFactor;
    std::shared_ptr<RelFactorNode> rightRelFactor;
};