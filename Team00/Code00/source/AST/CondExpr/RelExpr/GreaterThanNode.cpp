#include "GreaterThanNode.h"
GreaterThanNode::GreaterThanNode(std::shared_ptr<RelFactorNode> leftRelFactor,
               std::shared_ptr<RelFactorNode> rightRelFactor)
    : leftRelFactor(leftRelFactor), rightRelFactor(rightRelFactor) {}