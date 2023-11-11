#include "ExpressionProcessor.h"

#include "../AST/TreeBuilder.h"
#include "../AST/BinaryOpNode.h"
#include "../AST/UnaryOpNode.h"
#include "../SimpleTokens/TokenFactory.h"
#include "../../ExceptionMessages.h"
#include "../../HelperFunctions.h"
#include "../../Constants/SPConstants.h"

vector<shared_ptr<Token>> ExpressionProcessor::tokenizeArithmeticExpression(string expression) {
	string delimiter = arithmeticOpsWithWhitespaceRegex;
	string trimmed = trimWhitespaces(expression);
	vector<shared_ptr<Token>> output;
	vector<string> split = splitString(trimmed, delimiter, true);
	for (string s : split) {
		s = trimWhitespaces(s);
		// ignore whitespaces. equality check is basically saying: not cannot find so == can find
		// whitespaces are declared in HelperFunctions.h
		if (whitespaces.find(s) != string::npos || s.empty()) continue;
		output.push_back(TokenFactory::generateTokenForSimple(s, true));
	}
	return output;
}

shared_ptr<Node> ExpressionProcessor::nodifyCondExprTwoPairHelper(string trimmed, vector<shared_ptr<pair<int, int>>> bracketPairs) {
	if (bracketPairs.size() != 2) { // should not be called with more than size 2 anyways
		throw invalid_argument(ExceptionMessages::invalidCondExp);
	}
	int maxIndex = trimmed.size() - 1;
	pair<int, int> pair1 = *(bracketPairs[0]);
	pair<int, int> pair2 = *(bracketPairs[1]);
	bool firstLastParen = pair1.first == 0 && pair2.second == maxIndex;
	if (!firstLastParen) {
		// If first pair's "(" is not the first index or second pair's ")" is not the last index of *trimmed* string
		// This means that this definitely cannot be the brackets for || and &&
		return nodifyRelationalExpression(trimmed);
	}

	string betweenPairs = trimWhitespaces(substring(trimmed, pair1.second + SEPARATOR_LEN, pair2.first - SEPARATOR_LEN));
	bool isBinBoolOp = betweenPairs == AND_BOOL_OP_STRING || betweenPairs == OR_BOOL_OP_STRING;
	if (isBinBoolOp) {
		// definitely (...) || (...) or (...) && (...)
		string left = trimWhitespaces(substring(trimmed, pair1.first + SEPARATOR_LEN, pair1.second - SEPARATOR_LEN));
		string right = trimWhitespaces(substring(trimmed, pair2.first + SEPARATOR_LEN, pair2.second - SEPARATOR_LEN));
		shared_ptr<Node> leftResult = nodifyConditionalExpression(left);
		shared_ptr<Node> rightResult = nodifyConditionalExpression(right);
		vector<shared_ptr<Node>> children = { leftResult, rightResult };
		return make_shared<BinaryOpNode>(statementNumber, betweenPairs, children);
	}
	return nodifyRelationalExpression(trimmed);
}

shared_ptr<Node> ExpressionProcessor::nodifyCondExprOnePairHelper(string trimmed, shared_ptr<pair<int, int>> bracketIndexes) {
	int maxIndex = trimmed.size() - 1;
	int first = bracketIndexes->first;
	int second = bracketIndexes->second;
	if (first == 0) {
		// definitely not a !(...)
		return nodifyRelationalExpression(trimmed);
	}
	if (trimmed[0] == EXCLAMATION_MARK_CHAR && second == maxIndex) { // should be things like `!   (...)`
		string afterExclamationMark = trimWhitespaces(substring(trimmed, SEPARATOR_LEN, maxIndex)); // remove the exclamation mark
		if (afterExclamationMark[0] != OPEN_BRACKET_CHAR) {
			throw invalid_argument(ExceptionMessages::invalidCondExp);
		}
		string inner = trimWhitespaces(substring(afterExclamationMark, 1, afterExclamationMark.size() - SEPARATOR_LEN - 1)); // remove the parentheses
		shared_ptr<Node> innerResult = nodifyConditionalExpression(inner);
		return make_shared<NotOpNode>(statementNumber, innerResult);
	}
	// first character is not a `(`, and [first character is not `!` or last character is not `)`]. In both cases, this implies that there are no more condExpr here
	return nodifyRelationalExpression(trimmed);
}

shared_ptr<Node> ExpressionProcessor::nodifyRelationalExpression(string trimmed) {
	vector<string> split = splitString(trimmed, relationalOpsRegex, true);
	if (split.size() != 3) {
		throw invalid_argument(ExceptionMessages::invalidRelExp);
	}
	string left = trimWhitespaces(split[0]);
	string right = trimWhitespaces(split[2]);
	string op = trimWhitespaces(split[1]);
	shared_ptr<Node> leftResult = nodifyArithmeticExpression(left);
	shared_ptr<Node> rightResult = nodifyArithmeticExpression(right);
	vector<shared_ptr<Node>> children = { leftResult, rightResult };
	return make_shared<BinaryOpNode>(statementNumber, op, children);
}

shared_ptr<Node> ExpressionProcessor::nodifyArithmeticExpression(string expression) {
	string trimmed = trimWhitespaces(expression);
	if (trimmed.empty()) {
		throw invalid_argument(ExceptionMessages::invalidArithExp);
	}
	outermostSepDetector(trimmed, Separator::BRACKET); // checks for validity of brackets
	TreeBuilder tb = TreeBuilder(statementNumber);
	vector<shared_ptr<Token>> tokens = tokenizeArithmeticExpression(trimmed);
	return tb.buildTree(tokens);
}

shared_ptr<Node> ExpressionProcessor::nodifyConditionalExpression(string expression) {
	string trimmed = trimWhitespaces(expression);
	if (trimmed.empty()) {
		throw invalid_argument(ExceptionMessages::invalidCondExp);
	}
	vector<shared_ptr<pair<int, int>>> bracketPairs = outermostSepDetector(trimmed, Separator::BRACKET);

	int numPairs = bracketPairs.size();
	if (numPairs == 2) {
		return nodifyCondExprTwoPairHelper(trimmed, bracketPairs);
	}
	if (numPairs == 1) {
		return nodifyCondExprOnePairHelper(trimmed, bracketPairs[0]);
	}
	return nodifyRelationalExpression(trimmed); // should be just a rel_expr within, ie (1) + (2) > (3) with 3 pairs of brackets or 1 + 2 > 3 with 0 pairs
}