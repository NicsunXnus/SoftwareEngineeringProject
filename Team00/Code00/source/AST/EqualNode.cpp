#include "EqualNode.h"
EqualNode::EqualNode(std::shared_ptr<RelFactorNode> leftRelFactor,
               std::shared_ptr<RelFactorNode> rightRelFactor)
    : leftRelFactor(leftRelFactor), rightRelFactor(rightRelFactor) {}