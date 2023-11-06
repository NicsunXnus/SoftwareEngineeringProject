#include "BinaryOpNode.h"

vector<shared_ptr<Node>> BinaryOpNode::validate(vector<shared_ptr<Node>> children) {
  if (children.size() != 2) {
    throw invalid_argument(ExceptionMessages::wrongNumberChildren);
  }
  return children;
}