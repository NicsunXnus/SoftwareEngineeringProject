#ifndef EXPRESSIONPROCESSOR_H
#define EXPRESSIONPROCESSOR_H

#include <memory>
#include <string>
#include <string_view>
#include <vector>
#include <utility>

#include "SimpleProcessor.h"
#include "../AST/Node.h"

using namespace std::string_view_literals;

class ExpressionProcessor {
private:
	int statementNumber;

	// Convert Arithmetic expressions into tokens
	static std::vector<std::shared_ptr<Token>> tokenizeArithmeticExpression(std::string expression);

	// Converts conditional expressions involving 2 bracket-pairs into nodes. A recursive call is done.
	std::shared_ptr<Node> nodifyCondExprTwoPairHelper(std::string trimmed, std::vector<std::shared_ptr<std::pair<int, int>>> bracketPairs);

	// Converts conditional expressions involving 1 bracket-pair into nodes. A recursive call is done.
	std::shared_ptr<Node> nodifyCondExprOnePairHelper(std::string trimmed, std::shared_ptr<std::pair<int, int>> bracketIndexes);

	// Converts relational expressions involving nodes. A recursive call is done.
	std::shared_ptr<Node> nodifyRelationalExpression(std::string trimmed);

public:
	ExpressionProcessor(int statementNumber = -1) :
		statementNumber{ statementNumber } {};
	/// <summary>
	/// Converts arithmetic expressions into nodes. Prioritises identifiers for valid names.
	/// No assumptions made about whether whitespaces have been trimmed or not.
	/// </summary>
	/// <param name="expression">The string containing the arithmetic expression</param>
	/// <exception cref="std::invalid_argument">Thrown when the the declaration is invalid syntactically</exception>
	/// <returns>A pointer to the Node</returns>
	std::shared_ptr<Node> nodifyArithmeticExpression(std::string expression);


	/// <summary>
	/// Converts conditional expressions into nodes. Prioritises identifiers for valid names.
	/// No assumptions made about whether whitespaces have been trimmed or not.
	/// </summary>
	/// <param name="expression">The string containing the conditional expression</param>
	/// <exception cref="std::invalid_argument">Thrown when the the declaration is invalid syntactically, even for brackets.</exception>
	/// <returns>A pointer to the Node</returns>
	std::shared_ptr<Node> nodifyConditionalExpression(std::string expression);
};
#endif
