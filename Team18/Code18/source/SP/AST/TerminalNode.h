#ifndef TERMINALNODE_H
#define TERMINALNODE_H

#include "Node.h"

using namespace std;

class Extractor;
class UsesExtractor;

/// <summary>
/// Represents the last element in the tree-like node structure without any children, which should be either constants or variables.
/// </summary>
class TerminalNode : public Node {
public:
  TerminalNode(int stmtNum, std::string val)
    : Node{ stmtNum, val, std::vector<std::shared_ptr<Node>>() } {};
};

/// <summary>
/// Represents variables.
/// </summary>
class VariableNode : public TerminalNode, public std::enable_shared_from_this<VariableNode> {
public:
  VariableNode(int stmtNum, std::string val)
    : TerminalNode{ stmtNum, val } {};
  void accept(std::shared_ptr<Extractor> extractor) override;
};

/// <summary>
/// Represents constants
/// </summary>
class ConstantNode : public TerminalNode, public std::enable_shared_from_this<ConstantNode> {
public:
  ConstantNode(int stmtNum, std::string val)
    : TerminalNode{ stmtNum, val } {};
  void accept(std::shared_ptr<Extractor> extractor) override;
};

#endif