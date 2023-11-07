#ifndef BINARYOPNODE_H
#define BINARYOPNODE_H

#include "Node.h"

using namespace std;

/// <summary>
/// Represents any operation done on two children node.
/// </summary>
class BinaryOpNode : public OpNode {
private:
  static vector<shared_ptr<Node>> validate(vector<shared_ptr<Node>> children);
public:
  BinaryOpNode(int stmtNum, string val, vector<shared_ptr<Node>> children)
    : OpNode{ stmtNum, val, validate(children) } {};
};

/// <summary>
/// Represents an Addition operation.
/// </summary>
class PlusNode : public BinaryOpNode {
public:
  PlusNode(int stmtNum, shared_ptr<Node> left, shared_ptr<Node> right)
    : BinaryOpNode{ stmtNum, "+", vector<shared_ptr<Node>>{left, right} } {};
};

/// <summary>
/// Represents a Subtraction operation.
/// </summary>
class MinusNode : public BinaryOpNode {
public:
  MinusNode(int stmtNum, shared_ptr<Node> left, shared_ptr<Node> right)
    : BinaryOpNode{ stmtNum, "-", vector<shared_ptr<Node>>{left, right} } {};
};

/// <summary>
/// Represents a Multiplication operation.
/// </summary>
class MultiplyNode : public BinaryOpNode {
public:
  MultiplyNode(int stmtNum, shared_ptr<Node> left, shared_ptr<Node> right)
    : BinaryOpNode{ stmtNum, "*", vector<shared_ptr<Node>>{left, right} } {};
};

/// <summary>
/// Represents a Division operation.
/// </summary>
class DivideNode : public BinaryOpNode {
public:
  DivideNode(int stmtNum, shared_ptr<Node> left, shared_ptr<Node> right)
    : BinaryOpNode{ stmtNum, "/", vector<shared_ptr<Node>>{left, right} } {};
};

/// <summary>
/// Represents a Modulo operation.
/// </summary>
class ModuloNode : public BinaryOpNode {
public:
  ModuloNode(int stmtNum, shared_ptr<Node> left, shared_ptr<Node> right)
    : BinaryOpNode{ stmtNum, "%", vector<shared_ptr<Node>>{left, right} } {};
};

/// <summary>
/// Represents a logical AND operation.
/// </summary>
class AndNode : public BinaryOpNode {
public:
  AndNode(int stmtNum, shared_ptr<Node> left, shared_ptr<Node> right)
    : BinaryOpNode{ stmtNum, "&&", vector<shared_ptr<Node>>{left, right} } {};
};

/// <summary>
/// Represents a logical OR operation.
/// </summary>
class OrNode : public BinaryOpNode {
public:
  OrNode(int stmtNum, shared_ptr<Node> left, shared_ptr<Node> right)
    : BinaryOpNode{ stmtNum, "||", vector<shared_ptr<Node>>{left, right} } {};
};

/// <summary>
/// Represents a More Than comparison.
/// </summary>
class MoreThanNode : public BinaryOpNode {
public:
  MoreThanNode(int stmtNum, shared_ptr<Node> left, shared_ptr<Node> right)
    : BinaryOpNode{ stmtNum, ">", vector<shared_ptr<Node>>{left, right} } {};
};

/// <summary>
/// Represents a More Than Equals comparison.
/// </summary>
class MoreThanEqualNode : public BinaryOpNode {
public:
  MoreThanEqualNode(int stmtNum, shared_ptr<Node> left, shared_ptr<Node> right)
    : BinaryOpNode{ stmtNum, ">=", vector<shared_ptr<Node>>{left, right} } {};
};

/// <summary>
/// Represents a Less Than comparison.
/// </summary>
class LessThanNode : public BinaryOpNode {
public:
  LessThanNode(int stmtNum, shared_ptr<Node> left, shared_ptr<Node> right)
    : BinaryOpNode{ stmtNum, "<", vector<shared_ptr<Node>>{left, right} } {};
};

/// <summary>
/// Represents a Less Than Equals comparison.
/// </summary>
class LessThanEqualNode : public BinaryOpNode {
public:
  LessThanEqualNode(int stmtNum, shared_ptr<Node> left, shared_ptr<Node> right)
    : BinaryOpNode{ stmtNum, "<=", vector<shared_ptr<Node>>{left, right} } {};
};

/// <summary>
/// Represents a Equality comparison.
/// </summary>
class EqualityNode : public BinaryOpNode {
public:
  EqualityNode(int stmtNum, shared_ptr<Node> left, shared_ptr<Node> right)
    : BinaryOpNode{ stmtNum, "==", vector<shared_ptr<Node>>{left, right} } {};
};

/// <summary>
/// Represents a Equality comparison.
/// </summary>
class InequalityNode : public BinaryOpNode {
public:
  InequalityNode(int stmtNum, shared_ptr<Node> left, shared_ptr<Node> right)
    : BinaryOpNode{ stmtNum, "!=", vector<shared_ptr<Node>>{left, right} } {};
};
#endif