#ifndef UNARYOPNODE_H
#define UNARYOPNODE_H

#include "Node.h"

using namespace std;

/// <summary>
/// Represents any operation done on a single child node.
/// </summary>
class UnaryOpNode : public OpNode {
private:
  static std::vector<std::shared_ptr<Node>> validate(std::vector<std::shared_ptr<Node>> children);
public:
  UnaryOpNode(int stmtNum, std::string val, std::vector<std::shared_ptr<Node>> children)
    : OpNode{ stmtNum, val, validate(children) } {};
};

/// <summary>
/// Represents an Not Operation.
/// </summary>
class NotOpNode : public UnaryOpNode {
public:
  NotOpNode(int stmtNum, std::shared_ptr<Node> child)
    : UnaryOpNode{ stmtNum, "!", std::vector<std::shared_ptr<Node>>{child} } {};
};
#endif