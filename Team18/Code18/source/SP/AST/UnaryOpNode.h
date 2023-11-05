#ifndef UNARYOPNODE_H
#define UNARYOPNODE_H

#include "Node.h"

using namespace std;

/// <summary>
/// Represents any operation done on a single child node.
/// </summary>
class UnaryOpNode : public OpNode {
private:
  static vector<shared_ptr<Node>> validate(vector<shared_ptr<Node>> children);
public:
  UnaryOpNode(int stmtNum, string val, vector<shared_ptr<Node>> children)
    : OpNode{ stmtNum, val, validate(children) } {};
};

/// <summary>
/// Represents an Not Operation.
/// </summary>
class NotOpNode : public UnaryOpNode {
public:
  NotOpNode(int stmtNum, shared_ptr<Node> child)
    : UnaryOpNode{ stmtNum, "!", vector<shared_ptr<Node>>{child} } {};
};
#endif