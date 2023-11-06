#include <stack>
#include "TreeBuilder.h"
#include "BinaryOpNode.h"
#include "TerminalNode.h"
#include "OperatorEvaluator.h"
#include "../../HelperFunctions.h"

shared_ptr<Node> TreeBuilder::makeArithmeticNode(string op, shared_ptr<Node> left, shared_ptr<Node> right) {
	if (op == "+") return make_shared<PlusNode>(this->statementNumber, left, right);
	if (op == "-") return make_shared<MinusNode>(this->statementNumber, left, right);
	if (op == "/") return make_shared<DivideNode>(this->statementNumber, left, right);
	if (op == "*") return make_shared<MultiplyNode>(this->statementNumber, left, right);
	if (op == "%") return make_shared<ModuloNode>(this->statementNumber, left, right);
	throw invalid_argument(ExceptionMessages::invalidOperation);
}

void TreeBuilder::applyArithmeticOperation(stack<string>& ops, stack<shared_ptr<Node>>& values) {
	if (values.size() < 2) { //at least 2 items in values to apply mathematical operation
		throw invalid_argument(ExceptionMessages::missingOperatee);
	}
	shared_ptr<Node> right = values.top();
	values.pop();
	shared_ptr<Node> left = values.top();
	values.pop();
	values.push(this->makeArithmeticNode(ops.top(), left, right));
	ops.pop();
}

shared_ptr<Node> TreeBuilder::buildTree(vector<shared_ptr<Token>> tokens) {
	stack<shared_ptr<Node>> values; stack<string> ops;
	int lengthTokens = tokens.size();

	for (int ind = 0; ind < lengthTokens; ind++) {
		string tokenName = tokens[ind]->getName();

		if (tokenName == "(") {
			ops.push("(");
			continue;
		}
		if (tokenName == ")") {
			while (!ops.empty() && ops.top() != "(") {
				this->applyArithmeticOperation(ops, values);
			}
			ops.pop(); // Remove the '('
			continue;
		}

		if (isValidNumber(tokenName)) {
			values.push(make_shared<ConstantNode>(this->statementNumber, tokenName));
			continue;
		}

		if (isValidName(tokenName)) {
			values.push(make_shared<VariableNode>(this->statementNumber, tokenName));
			continue;
		}

		int orderOfTopOpr = 0, orderOfCurrToken = 0;
		orderOfCurrToken = precedence.at(tokenName);
		while (!ops.empty()) {
			orderOfTopOpr = precedence.at(ops.top());
			if (orderOfTopOpr >= orderOfCurrToken) {
				this->applyArithmeticOperation(ops, values);
			}
			else {
				break;
			}
		}
		ops.push(tokenName);
	}
	while (!ops.empty()) {
		this->applyArithmeticOperation(ops, values);
	}

	return values.top();
}