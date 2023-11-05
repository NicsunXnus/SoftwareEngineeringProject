#ifndef CFGPATHNODE_H
#define CFGPATHNODE_H

#include <unordered_set>
#include <string>
#include <map>
#include <memory>

using namespace std;

class DFSPathNode {
private:
  shared_ptr<DFSPathNode> parent = nullptr;
  // Child nodes are weak pointers to prevent circular shared_ptr dependencies.
  // Why child instead of parent is because during unrolling (upwards traversal), paths will be stored in
  // the CFG, essentially storing a shared_ptr to the child. Hence, even when the unrolling traverses to
  // the parents, there still exists a shared_ptr to the child and hence the memory will not be freed
  // until the entire CFG object is deleted when the dataAccessLayer is refreshed.
  weak_ptr<DFSPathNode> child = make_shared<DFSPathNode>(nullptr);
  string lineNumber;
  map<string, int> whileVisitCount;

  static shared_ptr<DFSPathNode> deepCopy(shared_ptr<DFSPathNode> current);

public:
  // Overloaded Constructor, meant for traverse to use.
  DFSPathNode(string lineNumber, shared_ptr<DFSPathNode> parent, map<string, int> whileVisitCount)
    : lineNumber{ lineNumber }, parent{ parent }, whileVisitCount { whileVisitCount } {};
  // Constructor to initialise the beginning of a path.
  DFSPathNode(string lineNumber) : lineNumber{ lineNumber } {}

  shared_ptr<DFSPathNode> getParent();

  shared_ptr<DFSPathNode> getChild();

  string getLineNumber();

  // Gets the number of visits this path had to a certain stmt number.
  // Does not enforce that the statementNumber is a while statement.
  int getWhileVisitCount(string stmtNumber);

  // Increments the number of visits this path had to a certain stmt number.
  // Does not enforce that the statementNumber is a while statement.
  void incrementWhileVisitCount(string stmtNumber);

  // Functional Programming way to produce the next node in the traversal.
  static shared_ptr<DFSPathNode> traverse(shared_ptr<DFSPathNode> current, string nextLineNumber);

  // Functional Programming way to produce multiple next nodes in a if or while statement. Duplicates parents to allow for unique paths
  static unordered_set<shared_ptr<DFSPathNode>> diverge(shared_ptr<DFSPathNode> current, unordered_set<string> lineNumbers);

  static unordered_set<shared_ptr<DFSPathNode>> joinDescendents(shared_ptr<DFSPathNode> current, unordered_set<shared_ptr<DFSPathNode>> toJoin);
};

#endif