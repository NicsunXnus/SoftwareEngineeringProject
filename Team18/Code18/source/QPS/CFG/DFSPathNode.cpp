#include "DFSPathNode.h"

using namespace std;

shared_ptr<DFSPathNode> DFSPathNode::getParent() {
  return this->parent;
}

shared_ptr<DFSPathNode> DFSPathNode::getChild() {
  return this->child.lock();
}

string DFSPathNode::getLineNumber() {
  return this->lineNumber;
}

// Gets the number of visits this path had to a certain stmt number.
// Does not enforce that the statementNumber is a while statement.
int DFSPathNode::getWhileVisitCount(string stmtNumber) {
  if (whileVisitCount.find(stmtNumber) == whileVisitCount.end()) {
    return 0;
  }
  return this->whileVisitCount.at(stmtNumber);
}
// Increments the number of visits this path had to a certain stmt number.
// Does not enforce that the statementNumber is a while statement.
void DFSPathNode::incrementWhileVisitCount(string stmtNumber) {
  if (whileVisitCount.find(stmtNumber) == whileVisitCount.end()) {
    whileVisitCount.emplace(stmtNumber, 1);
  }
  else {
    whileVisitCount[stmtNumber]++;
  }
}

shared_ptr<DFSPathNode> DFSPathNode::deepCopy(shared_ptr<DFSPathNode> current) {
  if (current == nullptr) {
    return nullptr;
  }
  shared_ptr<DFSPathNode> parentCopy = deepCopy(current->getParent());
  shared_ptr<DFSPathNode> thisCopy = make_shared<DFSPathNode>(current->lineNumber, parentCopy, current->whileVisitCount);
  if (parentCopy) {
    parentCopy->child = thisCopy;
  }
  return thisCopy;
}

shared_ptr<DFSPathNode> DFSPathNode::traverse(shared_ptr<DFSPathNode> current, string nextLineNumber) {
  // Traverse simply adds a child node without copying.
  shared_ptr<DFSPathNode> childNode = make_shared<DFSPathNode>(nextLineNumber, current, current->whileVisitCount);
  current->child = childNode;
  return childNode;
}

unordered_set<shared_ptr<DFSPathNode>> DFSPathNode::diverge(shared_ptr<DFSPathNode> current, unordered_set<string> lineNumbers) {
  // Traverses the original copy of current, and copies of the original for everything else
  unordered_set<shared_ptr<DFSPathNode>> output;
  for (auto it = lineNumbers.begin(); it != lineNumbers.end(); it++) {
    if (it == lineNumbers.begin()) {
      output.insert(traverse(current, *it));
    }
    else {
      output.insert(traverse(deepCopy(current), *it));
    }
  }
  return output;
}

unordered_set<shared_ptr<DFSPathNode>> DFSPathNode::joinDescendants(shared_ptr<DFSPathNode> current, unordered_set<shared_ptr<DFSPathNode>> toJoin) {
  // Joins one descendant to the current's parent, and others to copies of the parent
  unordered_set<shared_ptr<DFSPathNode>> output;
  shared_ptr<DFSPathNode> currParent = current->getParent();
  for (auto it = toJoin.begin(); it != toJoin.end(); it++) {
    shared_ptr<DFSPathNode> node = *it;
    if (it == toJoin.begin()) {
      currParent->child = node;
      node->parent = currParent;
      output.insert(node);
    }
    else {
      shared_ptr<DFSPathNode> parentCopy = deepCopy(currParent);
      parentCopy->child = node;
      node->parent = parentCopy;
      output.insert(node);
    }
  }
  return output;
}