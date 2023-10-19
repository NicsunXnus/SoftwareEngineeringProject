#include "BinaryOpNode.h"

std::vector<std::shared_ptr<Node>> BinaryOpNode::validate(std::vector<std::shared_ptr<Node>> children) {
  if (children.size() != 2) {
    throw std::invalid_argument(ExceptionMessages::wrongNumberChildren);
  }
  return children;
}