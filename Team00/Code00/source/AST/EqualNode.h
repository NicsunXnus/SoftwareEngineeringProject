#pragma once
#include "RelExprNode.h"
#include "RelFactorNode.h"
#include <memory>

class EqualNode : public RelExprNode {
public:
    EqualNode(std::shared_ptr<RelFactorNode> leftRelFactor, std::shared_ptr<RelFactorNode> rightRelFactor);

    std::shared_ptr<RelFactorNode> leftRelFactor;
    std::shared_ptr<RelFactorNode> rightRelFactor;
};