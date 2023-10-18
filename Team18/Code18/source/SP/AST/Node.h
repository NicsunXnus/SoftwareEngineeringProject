#ifndef NODE_H
#define NODE_H

#include <string>
#include <vector>
#include <memory>
#include <stdexcept>

#include "../../ExceptionMessages.h"

using namespace std;

class Extractor;

class Node {
private:
  int statementNumber; // The statement that this Node appears in
  std::string value; // The representation of this Node as a string. Example: "123", "abc", "+"
  std::vector<std::shared_ptr<Node>> children;
public:
  Node(int stmtNum, std::string val, std::vector<std::shared_ptr<Node>> children)
    : statementNumber{ stmtNum },
      value{ val },
      children{ children } {};
  virtual ~Node() = default;

  int getStatementNumber();
  std::string getValue();
  std::vector<std::shared_ptr<Node>> getChildren();

  int getChildrenCount();
  bool hasChildren();

  /// <summary>
  /// Checks if this node and its children are identical to the other node and their children
  /// </summary>
  /// <param name="that">The other node to check against</param>
  /// <param name="isStrict">Whether to check for statement number or not. Default of false.</param>
  /// <returns>Whether the nodes are identical</returns>
  bool isIdentical(std::shared_ptr<Node> that, bool isStrict=false);
  // Overloaded method to take in Node* instead of shared_ptr<Node>
  bool isIdentical(Node* that, bool isStrict = false);

  /// <summary>
  /// Checks if this node is a subtree of the other node
  /// </summary>
  /// <param name="other">The other node to check the subtree of</param>
  /// <param name="isStrict">Whether to check for statement number or not. Default of false</param>
  /// <returns>Whether this is a subtree of the Other node</returns>
  bool isSubtreeOf(std::shared_ptr<Node> other, bool isStrict = false);
  // Overloaded method to take in Node* instead of shared_ptr<Node>
  bool isSubtreeOf(Node* other, bool isStrict);

  virtual void accept(std::shared_ptr<Extractor> extractor) {};
};

/// <summary>
/// Represents any operation done between Nodes. 
/// </summary>
class OpNode : public Node, public std::enable_shared_from_this<OpNode> {
private:
  static std::vector<std::shared_ptr<Node>> validate(std::vector<std::shared_ptr<Node>> children);
public:
  OpNode(int stmtNum, std::string val, std::vector<std::shared_ptr<Node>> children)
    : Node{ stmtNum, val, validate(children) } {};
  void accept(std::shared_ptr<Extractor> extractor) override;
};

#endif