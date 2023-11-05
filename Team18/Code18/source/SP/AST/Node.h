#ifndef NODE_H
#define NODE_H

#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
#include <unordered_set>

#include "../../ExceptionMessages.h"

using namespace std;

class Extractor;

class Node {
private:
  int statementNumber; // The statement that this Node appears in
  string value; // The representation of this Node as a string. Example: "123", "abc", "+"
  vector<shared_ptr<Node>> children;
public:
  Node(int stmtNum, string val, vector<shared_ptr<Node>> children)
    : statementNumber{ stmtNum },
      value{ val },
      children{ children } {};
  virtual ~Node() = default;

  int getStatementNumber();
  string getValue();
  vector<shared_ptr<Node>> getChildren();

  int getChildrenCount();
  bool hasChildren();

  /// <summary>
  /// Checks if this node and its children are identical to the other node and their children
  /// </summary>
  /// <param name="that">The other node to check against</param>
  /// <param name="isStrict">Whether to check for statement number or not. Default of false.</param>
  /// <returns>Whether the nodes are identical</returns>
  bool isIdentical(shared_ptr<Node> that, bool isStrict=false);
  // Overloaded method to take in Node* instead of shared_ptr<Node>
  bool isIdentical(Node* that, bool isStrict = false);

  /// <summary>
  /// Checks if this node is a subtree of the other node
  /// </summary>
  /// <param name="other">The other node to check the subtree of</param>
  /// <param name="isStrict">Whether to check for statement number or not. Default of false</param>
  /// <returns>Whether this is a subtree of the Other node</returns>
  bool isSubtreeOf(shared_ptr<Node> other, bool isStrict = false);
  // Overloaded method to take in Node* instead of shared_ptr<Node>
  bool isSubtreeOf(Node* other, bool isStrict);

  /// <summary>
  /// Gets all the terminal nodes of this subtree that are variable nodes
  /// </summary>
  /// <param name="node">The node which is the root of the subtree</param>
  /// <returns>A set of all variable names of the terminal nodes</returns>
  unordered_set<string> getTerminalVariablesHelper(shared_ptr<Node> node);

  // Overloaded method to take in Node* instead of shared_ptr<Node>
  unordered_set<string> getTerminalVariablesHelper(Node* node);

  /// <summary>
  /// Gets all the terminal nodes of this subtree that are variable nodes
  /// </summary>
  /// <returns>A set of all the terminal variable nodes</returns>
  unordered_set<string> getTerminalVariables();

  virtual void accept(shared_ptr<Extractor> extractor) {};
};

/// <summary>
/// Represents any operation done between Nodes. 
/// </summary>
class OpNode : public Node, public enable_shared_from_this<OpNode> {
private:
  static vector<shared_ptr<Node>> validate(vector<shared_ptr<Node>> children);
public:
  OpNode(int stmtNum, string val, vector<shared_ptr<Node>> children)
    : Node{ stmtNum, val, validate(children) } {};
  void accept(shared_ptr<Extractor> extractor) override;
};

#endif