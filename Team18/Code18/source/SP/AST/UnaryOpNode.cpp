#include "UnaryOpNode.h"

vector<shared_ptr<Node>> UnaryOpNode::validate(vector<shared_ptr<Node>> children) {
  if (children.size() != 1) {
    throw invalid_argument(ExceptionMessages::wrongNumberChildren);
  }
  return children;
}
