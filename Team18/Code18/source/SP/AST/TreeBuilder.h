#ifndef TREEBUILDER_H
#define TREEBUILDER_H

#include <vector>
#include <stack>
#include <string>
#include <memory>

#include "Node.h"
#include "../SimpleTokens/Token.h"

using namespace std;

/// <summary>
/// Represents a Builder to build an AST based off on arithmetic or conditional expressions
/// </summary>
/// <remarks>Originally done by Nicholas, refactored by Han Yang. Both people put in more work afterwards to complete this</remarks>
class TreeBuilder {
private:
  // Should be initialised with this
  int statementNumber;

  // Combines a left and a right Node into a new node representing the arithmetic operation
  shared_ptr<Node> makeArithmeticNode(string op, shared_ptr<Node> left, shared_ptr<Node> right);

  // Applies arithmetic operations on the stacks given
  void applyArithmeticOperation(stack<string>& ops, stack<shared_ptr<Node>>& values);

public:
  // Initialise with statement number, default value of -1 (more applicable to QPS side)
  TreeBuilder(int statementNumber = -1)
    : statementNumber{ statementNumber } {};

  /// <summary>
  /// Constructs an AST based of a vector of token pointers. Uses the Shunting Yard Algorithm.
  /// </summary>
  /// <remarks>This should not be called in anywhere else other than ExpressionProcessor, as there should be no API to produce a vector of tokens
  ///     appropriately.</remarks>
  /// 
  /// <param name="tokens">the tokens to be converted into an AST</param>
  /// <exception cref="invalid_argument">Thrown when anything invalid is detected. This can include mismatched parentheses, 
  ///     operations, operations or variable/constants</exception>
  /// <returns>the AST built from the tokens</returns>
  shared_ptr<Node> buildTree(vector<shared_ptr<Token>> tokens);
};
#endif