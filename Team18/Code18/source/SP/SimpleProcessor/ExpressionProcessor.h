#ifndef EXPRESSIONPROCESSOR_H
#define EXPRESSIONPROCESSOR_H

#include <memory>
#include <string>
#include <string_view>
#include <vector>
#include <utility>

#include "SimpleProcessor.h"
#include "../AST/Node.h"

using namespace std;

class ExpressionProcessor {
protected:
	// Convert Arithmetic expressions into tokens
	static vector<shared_ptr<Token>> tokenizeArithmeticExpression(string expression);

private:
	int statementNumber;

	// Converts conditional expressions involving 2 bracket-pairs into nodes. A recursive call is done.
	shared_ptr<Node> nodifyCondExprTwoPairHelper(string trimmed, vector<shared_ptr<pair<int, int>>> bracketPairs);

	// Converts conditional expressions involving 1 bracket-pair into nodes. A recursive call is done.
	shared_ptr<Node> nodifyCondExprOnePairHelper(string trimmed, shared_ptr<pair<int, int>> bracketIndexes);

	// Converts relational expressions involving nodes. A recursive call is done.
	shared_ptr<Node> nodifyRelationalExpression(string trimmed);

public:
	ExpressionProcessor(int statementNumber = -1) :
		statementNumber{ statementNumber } {};

	/// <summary>
	/// Converts arithmetic expressions into nodes. Prioritises identifiers for valid names.
	/// No assumptions made about whether whitespaces have been trimmed or not.
	/// </summary>
	/// <param name="expression">The string containing the arithmetic expression</param>
	/// <exception cref="invalid_argument">Thrown when the the declaration is invalid syntactically</exception>
	/// <returns>A pointer to the Node</returns>
	shared_ptr<Node> nodifyArithmeticExpression(string expression);


	/// <summary>
	/// Converts conditional expressions into nodes. Prioritises identifiers for valid names.
	/// No assumptions made about whether whitespaces have been trimmed or not.
	/// </summary>
	/// <param name="expression">The string containing the conditional expression</param>
	/// <exception cref="invalid_argument">Thrown when the the declaration is invalid syntactically, even for brackets.</exception>
	/// <returns>A pointer to the Node</returns>
	shared_ptr<Node> nodifyConditionalExpression(string expression);
};
#endif
