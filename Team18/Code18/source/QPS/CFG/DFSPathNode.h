#ifndef CFGPATHNODE_H
#define CFGPATHNODE_H

#include <unordered_set>
#include <string>
#include <map>
#include <memory>

using namespace std;

class DFSPathNode {
private:
  shared_ptr<DFSPathNode> child = nullptr;
  weak_ptr<DFSPathNode> parent = make_shared<DFSPathNode>(nullptr);
  string lineNumber;
  map<string, int> whileVisitCount;

  // Overloaded Constructor, meant for traverse to use.
  DFSPathNode(string lineNumber, shared_ptr<DFSPathNode> parent, map<string, int> whileVisitCount)
    : lineNumber{ lineNumber }, parent{ parent }, whileVisitCount { whileVisitCount } {};
public:
  // Constructor to initialise the beginning of a path.
  DFSPathNode(string lineNumber) : lineNumber{ lineNumber } {}
  weak_ptr<DFSPathNode> getParent() {
    return this->parent;
  }
  shared_ptr<DFSPathNode> getChild() {
    return this->child;
  }
  string getLineNumber() {
    return this->lineNumber;
  }
  // Gets the number of visits this path had to a certain stmt number.
  // Does not enforce that the statementNumber is a while statement.
  int getWhileVisitCount(string stmtNumber) {
    if (whileVisitCount.find(stmtNumber) == whileVisitCount.end()) {
      return 0;
    }
    return this->whileVisitCount.at(stmtNumber);
  }
  // Increments the number of visits this path had to a certain stmt number.
  // Does not enforce that the statementNumber is a while statement.
  void incrementWhileVisitCount(string stmtNumber) {
    if (whileVisitCount.find(stmtNumber) == whileVisitCount.end()) {
      whileVisitCount.emplace(stmtNumber, 0);
    }
    else {
      whileVisitCount[stmtNumber]++;
    }
  }
  // Functional Programming way to produce the next node in the traversal.
  static shared_ptr<DFSPathNode> traverse(shared_ptr<DFSPathNode> current, string nextLineNumber) {
    shared_ptr<DFSPathNode> childNode = make_shared<DFSPathNode>(nextLineNumber, current, current->whileVisitCount);
    current->child = childNode;
    return childNode;
  }
  // Functional Programming way to produce multiple next nodes in a if or while statement.
  static unordered_set<shared_ptr<DFSPathNode>> diverge(shared_ptr<DFSPathNode> current, unordered_set<string> lineNumbers) {
    unordered_set<shared_ptr<DFSPathNode>> output;
    for (string lineNumber : lineNumbers) {
      output.insert(traverse(current, lineNumber));
    }
    return output;
  }
};

#endif