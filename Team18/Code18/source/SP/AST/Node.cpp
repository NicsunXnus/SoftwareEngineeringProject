#include "Node.h"
#include "../DesignExtractor/EntityExtractor.h"
#include "../DesignExtractor/UsesExtractor.h"
#include "../../HelperFunctions.h"

int Node::getStatementNumber() {
  return this->statementNumber;
}

string Node::getValue() {
  return this->value;
}

vector<shared_ptr<Node>> Node::getChildren() {
  return this->children;
}

int Node::getChildrenCount() {
  return this->children.size();
}

bool Node::hasChildren() {
  return !(this->children.empty());
}

bool Node::isIdentical(shared_ptr<Node> that, bool isStrict) {
  Node* thatPtr = &(*that);
  return this->isIdentical(thatPtr, isStrict);
}

bool Node::isIdentical(Node* that, bool isStrict) {
  /* base cases */
  if (that == nullptr) return false;

  bool sameValue = this->value == that->value;
  bool strictCheck = (!isStrict || this->statementNumber == that->statementNumber);
  bool sameChildNum = this->getChildrenCount() == that->getChildrenCount();
  if (!sameValue || !strictCheck || !sameChildNum) return false;

  for (int i = 0; i < this->getChildrenCount(); i++) {
    shared_ptr<Node> thisChild = this->children[i];
    shared_ptr<Node> thatChild = that->children[i];
    bool sameChild = thisChild->isIdentical(thatChild, isStrict);
    if (!sameChild) return false;
  }
  return true;
}

bool Node::isSubtreeOf(shared_ptr<Node> other, bool isStrict) {
  Node* otherPtr = &(*other);
  return this->isSubtreeOf(otherPtr, isStrict);
}

bool Node::isSubtreeOf(Node* other, bool isStrict) {
  /* base cases */
  if (other == nullptr) return false;

  // Implementation logic: this must be identical to other, or any of other's descendants
  bool isIdentical = this->isIdentical(other, isStrict);
  if (isIdentical) return true;

  for (shared_ptr<Node> otherChild : other->children) {
    // Use isSubtreeOf to allow for exploration of all descendants, since isIdentical cuts short nearer to the root if something mismatches
    bool childIdentical = this->isSubtreeOf(otherChild, isStrict);
    if (childIdentical) return true;
  }
  return false;
}

unordered_set<string> Node::getTerminalVariables() {
  return this->getTerminalVariablesHelper(this);
}

unordered_set<string> Node::getTerminalVariablesHelper(shared_ptr<Node> node) {
  Node* node_ptr = &(*node);
  return this->getTerminalVariablesHelper(node_ptr);
}

unordered_set<string> Node::getTerminalVariablesHelper(Node* node) {
  unordered_set<string> values;
  if (!node->hasChildren()) {  // Terminal node: Check if variable node
    string val = node->getValue();
    if (isValidName(val)) {
      values.insert(val);
    }
  } else {
    for (const auto& child : node->getChildren()) {
      const auto childValues = getTerminalVariablesHelper(child);
      values.insert(childValues.begin(), childValues.end());
    }
  }
  return values;
}

vector<shared_ptr<Node>> OpNode::validate(vector<shared_ptr<Node>> children) {
  if (children.size() == 0 || children.size() > 2) {
    throw invalid_argument(ExceptionMessages::wrongNumberChildren);
  }
  return children;
}


void OpNode::accept(shared_ptr<Extractor> extractor) {
  extractor->extract(shared_from_this());
}