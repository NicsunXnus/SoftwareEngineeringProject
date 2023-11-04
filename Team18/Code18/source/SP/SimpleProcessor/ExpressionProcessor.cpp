#include "ExpressionProcessor.h"

#include "../SimpleTokens/TokenFactory.h"
#include "../../ExceptionMessages.h"
#include "../../HelperFunctions.h"
#include "../AST/TreeBuilder.h"
#include "../AST/BinaryOpNode.h"
#include "../AST/UnaryOpNode.h"

std::vector<std::shared_ptr<Token>> ExpressionProcessor::tokenizeArithmeticExpression(std::string expression) {
	std::string delimiter = arithmeticOpsWithWhitespaceRegex;
	std::string trimmed = trimWhitespaces(expression);
	std::vector<std::shared_ptr<Token>> output;
	std::vector<std::string> split = splitString(trimmed, delimiter, true);
	for (std::string s : split) {
		s = trimWhitespaces(s);
		// ignore whitespaces. equality check is basically saying: not cannot find so == can find
		// whitespaces are declared in HelperFunctions.h
		if (whitespaces.find(s) != std::string::npos || s.empty()) continue;
		output.push_back(TokenFactory::generateTokenForSimple(s, true));
	}
	return output;
}

std::shared_ptr<Node> ExpressionProcessor::nodifyCondExprTwoPairHelper(std::string trimmed, std::vector<std::shared_ptr<std::pair<int, int>>> bracketPairs) {
	if (bracketPairs.size() != 2) { // should not be called with more than size 2 anyways
		throw std::invalid_argument(ExceptionMessages::invalidCondExp);
	}
	int maxIndex = trimmed.size() - 1;
	std::pair<int, int> pair1 = *(bracketPairs[0]);
	std::pair<int, int> pair2 = *(bracketPairs[1]);
	bool firstLastParen = pair1.first == 0 && pair2.second == maxIndex;
	if (!firstLastParen) {
		// If first pair's "(" is not the first index or second pair's ")" is not the last index of *trimmed* string
		// This means that this definitely cannot be the brackets for || and &&
		return nodifyRelationalExpression(trimmed);
	}

	std::string betweenPairs = trimWhitespaces(substring(trimmed, pair1.second + 1, pair2.first - 1));
	bool isBinBoolOp = betweenPairs == "&&" || betweenPairs == "||";
	if (isBinBoolOp) {
		// definitely (...) || (...) or (...) && (...)
		std::string left = trimWhitespaces(substring(trimmed, pair1.first + 1, pair1.second - 1)); // + 1 - 1 to remove parentheses
		std::string right = trimWhitespaces(substring(trimmed, pair2.first + 1, pair2.second - 1)); // + 1 - 1 to remove parentheses
		std::shared_ptr<Node> leftResult = nodifyConditionalExpression(left);
		std::shared_ptr<Node> rightResult = nodifyConditionalExpression(right);
		std::vector<std::shared_ptr<Node>> children = { leftResult, rightResult };
		return std::make_shared<BinaryOpNode>(statementNumber, betweenPairs, children);
	}
	return nodifyRelationalExpression(trimmed);
}

std::shared_ptr<Node> ExpressionProcessor::nodifyCondExprOnePairHelper(std::string trimmed, std::shared_ptr<std::pair<int, int>> bracketIndexes) {
	int maxIndex = trimmed.size() - 1;
	int first = bracketIndexes->first;
	int second = bracketIndexes->second;
	if (first == 0) {
		// definitely not a !(...)
		return nodifyRelationalExpression(trimmed);
	}
	if (trimmed[0] == '!' && second == maxIndex) { // should be things like `!   (...)`
		std::string afterExclamationMark = trimWhitespaces(substring(trimmed, 1, maxIndex)); // remove the exclamation mark
		if (afterExclamationMark[0] != '(') {
			throw std::invalid_argument(ExceptionMessages::invalidCondExp);
		}
		std::string inner = trimWhitespaces(substring(afterExclamationMark, 1, afterExclamationMark.size() - 2)); // remove the parentheses
		std::shared_ptr<Node> innerResult = nodifyConditionalExpression(inner);
		return std::make_shared<NotOpNode>(statementNumber, innerResult);
	}
	// first character is not a `(`, and [first character is not `!` or last character is not `)`]. In both cases, this implies that there are no more condExpr here
	return nodifyRelationalExpression(trimmed);
}

std::shared_ptr<Node> ExpressionProcessor::nodifyRelationalExpression(std::string trimmed) {
	std::vector<std::string> split = splitString(trimmed, relationalOpsRegex, true);
	if (split.size() != 3) {
		throw std::invalid_argument(ExceptionMessages::invalidRelExp);
	}
	std::string left = trimWhitespaces(split[0]);
	std::string right = trimWhitespaces(split[2]);
	std::string op = trimWhitespaces(split[1]);
	std::shared_ptr<Node> leftResult = nodifyArithmeticExpression(left);
	std::shared_ptr<Node> rightResult = nodifyArithmeticExpression(right);
	std::vector<std::shared_ptr<Node>> children = { leftResult, rightResult };
	return std::make_shared<BinaryOpNode>(statementNumber, op, children);
}

std::shared_ptr<Node> ExpressionProcessor::nodifyArithmeticExpression(std::string expression) {
	std::string trimmed = trimWhitespaces(expression);
	if (trimmed.empty()) {
		throw std::invalid_argument(ExceptionMessages::invalidArithExp);
	}
	TreeBuilder tb = TreeBuilder(statementNumber);
	std::vector<std::shared_ptr<Token>> tokens = tokenizeArithmeticExpression(trimmed);
	return tb.buildTree(tokens);
}

std::shared_ptr<Node> ExpressionProcessor::nodifyConditionalExpression(std::string expression) {
	std::string trimmed = trimWhitespaces(expression);
	if (trimmed.empty()) {
		throw std::invalid_argument(ExceptionMessages::invalidCondExp);
	}
	std::vector<std::shared_ptr<std::pair<int, int>>> bracketPairs = outermostSepDetector(trimmed, Separator::BRACKET);

	int numPairs = bracketPairs.size();
	if (numPairs == 2) {
		return nodifyCondExprTwoPairHelper(trimmed, bracketPairs);
	}
	if (numPairs == 1) {
		return nodifyCondExprOnePairHelper(trimmed, bracketPairs[0]);
	}
	return nodifyRelationalExpression(trimmed); // should be just a rel_expr within, ie (1) + (2) > (3) with 3 pairs of brackets or 1 + 2 > 3 with 0 pairs
}