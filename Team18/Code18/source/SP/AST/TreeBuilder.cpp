#include <stack>
#include "TreeBuilder.h"
#include "BinaryOpNode.h"
#include "TerminalNode.h"
#include "TreeBuilderHelper.h"
#include "../../HelperFunctions.h"

shared_ptr<Node> TreeBuilder::makeArithmeticNode(std::string op, std::shared_ptr<Node> left, std::shared_ptr<Node> right) {
	if (op == "+") return make_shared<PlusNode>(this->statementNumber, left, right);
	if (op == "-") return make_shared<MinusNode>(this->statementNumber, left, right);
	if (op == "/") return make_shared<DivideNode>(this->statementNumber, left, right);
	if (op == "*") return make_shared<MultiplyNode>(this->statementNumber, left, right);
	if (op == "%") return make_shared<ModuloNode>(this->statementNumber, left, right);
	throw std::invalid_argument(ExceptionMessages::invalidOperation);
}

void TreeBuilder::applyArithmeticOperation(std::stack<std::string>& ops, std::stack<std::shared_ptr<Node>>& values) {
	if (values.size() < 2) { //at least 2 items in values to apply mathematical operation
		throw std::invalid_argument(ExceptionMessages::missingOperatee);
	}
	shared_ptr<Node> right = values.top();
	values.pop();
	shared_ptr<Node> left = values.top();
	values.pop();
	values.push(this->makeArithmeticNode(ops.top(), left, right));
	ops.pop();
}

shared_ptr<Node> TreeBuilder::buildTree(std::vector<std::shared_ptr<Token>> tokens) {
	stack<shared_ptr<Node>> values; stack<string> ops;
	bool isPreviousCloseParenthesis = false, isPreviousOpenParenthesis = false, isPreviousAlphanumeric = false;
	int lengthTokens = tokens.size();

	for (int ind = 0; ind < lengthTokens; ind++) {
		std::string tokenName = tokens[ind]->getName();

		if (tokenName == "(") {
			if (isPreviousCloseParenthesis) throw std::invalid_argument(ExceptionMessages::closeBeforeOpenParen);
			ops.push("(");
			isPreviousOpenParenthesis = true;
			continue;
		}
		if (tokenName == ")") {
			if (isPreviousOpenParenthesis) throw std::invalid_argument(ExceptionMessages::emptyBrackets);
			isPreviousCloseParenthesis = true;
			while (!ops.empty() && ops.top() != "(") {
				this->applyArithmeticOperation(ops, values);
			}
			if (ops.empty()) {
				throw std::invalid_argument(ExceptionMessages::extraCloseParen);
			}
			ops.pop(); // Remove the '('
			isPreviousOpenParenthesis = false;
			continue;
		}

		isPreviousOpenParenthesis = false; //since everything after this is not a open parenthesis
		isPreviousCloseParenthesis = false; //set to false as rest is not a close parenthesis

		if (isNumber(tokenName)) {
			//Guard clause to prevent consecutive variables/constants
			if (isPreviousAlphanumeric) { throw std::invalid_argument(ExceptionMessages::consecutiveVariablesOrConstants); }
			values.push(make_shared<ConstantNode>(this->statementNumber, tokenName));
			isPreviousAlphanumeric = true;
			continue;
		}

		if (isValidName(tokenName)) {
			//Guard clause to prevent consecutive variables/constants
			if (isPreviousAlphanumeric) { throw std::invalid_argument(ExceptionMessages::consecutiveVariablesOrConstants); }
			values.push(make_shared<VariableNode>(this->statementNumber, tokenName));
			isPreviousAlphanumeric = true;
			continue;
		}

		int orderOfTopOpr = 0, orderOfCurrToken = 0;
		try {
			orderOfCurrToken = precedence.at(tokenName);
		}
		catch (std::out_of_range e) {
			throw std::invalid_argument(ExceptionMessages::invalidOperation);
		}
		while (!ops.empty()) {
			try {
				orderOfTopOpr = precedence.at(ops.top());
			}
			catch (std::out_of_range e) {
				throw std::invalid_argument(ExceptionMessages::invalidOperation);
			}
			if (orderOfTopOpr >= orderOfCurrToken) {
				this->applyArithmeticOperation(ops, values);
			}
			else {
				break;
			}
		}
		ops.push(tokenName);
		isPreviousAlphanumeric = false; //since the "previous" token here is guaranteed to be an operator on the next iteration
	}
	while (!ops.empty()) {
		this->applyArithmeticOperation(ops, values);
	}

	if (values.size() >= 2) {
		//Guard clause to ensure that the return result is the only item left in the stack
		//Otherwise, this would mean there are leftovers that have not been operated on.
		//This could be a result of missing operators.
		throw std::invalid_argument(ExceptionMessages::missingOperator);
	}
	return values.top();
}