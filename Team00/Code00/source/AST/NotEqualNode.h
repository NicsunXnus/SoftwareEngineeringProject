#pragma once
#include "RelExprNode.h"
#include "RelFactorNode.h"
#include <memory>

class NotEqualNode : public RelExprNode {
public:
    NotEqualNode(std::shared_ptr<RelFactorNode> leftRelFactor, std::shared_ptr<RelFactorNode> rightRelFactor);

    std::shared_ptr<RelFactorNode> leftRelFactor;
    std::shared_ptr<RelFactorNode> rightRelFactor;
};