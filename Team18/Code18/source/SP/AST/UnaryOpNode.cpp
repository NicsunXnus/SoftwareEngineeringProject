#include "UnaryOpNode.h"

std::vector<std::shared_ptr<Node>> UnaryOpNode::validate(std::vector<std::shared_ptr<Node>> children) {
  if (children.size() != 1) {
    throw std::invalid_argument(ExceptionMessages::wrongNumberChildren);
  }
  return children;
}
