#include "stdafx.h"
#include "CppUnitTest.h"
#include "../source/SP/SimpleProcessor/ExpressionProcessor.h"
#include "../source/SP/SimpleProcessor/SimpleProcessor.h"
#include "../source/SP/AST/TreeBuilder.h"
#include "../source/SP/SimpleTokens/TokenFactory.h"
#include <cassert>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace TreeBuilder_Test
{
	TEST_CLASS(arithmeticExpressions_Test) {
private:
	std::vector<std::shared_ptr<Token>> tokeniseWrapper(std::string expression) {
		std::vector<std::shared_ptr<Token>> out;
		for (char c : expression) {
			if (c == ' ') {
				continue;
			}
			std::string s{ c };
			out.push_back(TokenFactory::generateTokenForSimple(std::string_view(s)));
		}
		return out;
	}
public:
	TEST_METHOD(PlusMinus_success) {
		std::string expression = "a + b - 3";
		int statementNum = 5;

		std::vector<std::shared_ptr<Token>> tokens = tokeniseWrapper(expression);
		TreeBuilder tb = TreeBuilder(statementNum);
		std::shared_ptr<Node> result = tb.buildTree(tokens);

		std::vector<std::shared_ptr<Node>> empty = {};
		std::vector<std::shared_ptr<Node>> plusChildren = {
			std::make_shared<Node>(statementNum, "a", empty),
			std::make_shared<Node>(statementNum, "b", empty)
		};
		Node minus = Node(statementNum, "-", {
		  std::make_shared<Node>(statementNum, "+", plusChildren),
		  std::make_shared<Node>(statementNum, "3", empty)
			});

		bool strictChecking = true;
		assert(minus.isIdentical(result, strictChecking));
	}

	TEST_METHOD(MultiplyDivideModulo_success) {
		std::string expression = "a * b / 3 % 5";
		int statementNum = 051;

		std::vector<std::shared_ptr<Token>> tokens = tokeniseWrapper(expression);
		TreeBuilder tb = TreeBuilder(statementNum);
		std::shared_ptr<Node> result = tb.buildTree(tokens);

		std::vector<std::shared_ptr<Node>> empty = {};
		std::vector<std::shared_ptr<Node>> multiplyChildren = {
			std::make_shared<Node>(statementNum, "a", empty),
			std::make_shared<Node>(statementNum, "b", empty)
		};
		std::vector<std::shared_ptr<Node>> divideChildren = {
		  std::make_shared<Node>(statementNum, "*", multiplyChildren),
		  std::make_shared<Node>(statementNum, "3", empty)
		};
		Node modulo = Node(statementNum, "%", {
		  std::make_shared<Node>(statementNum, "/", divideChildren),
		  std::make_shared<Node>(statementNum, "5", empty)
			});

		bool strictChecking = true;
		assert(modulo.isIdentical(result, strictChecking));
	}

	TEST_METHOD(PlusMultiply_success) {
		std::string expression = "a + b * 3 + 5";
		int statementNum = 510;

		std::vector<std::shared_ptr<Token>> tokens = tokeniseWrapper(expression);
		TreeBuilder tb = TreeBuilder(statementNum);
		std::shared_ptr<Node> result = tb.buildTree(tokens);

		std::vector<std::shared_ptr<Node>> empty = {};
		std::vector<std::shared_ptr<Node>> multiplyChildren = {
			std::make_shared<Node>(statementNum, "b", empty),
			std::make_shared<Node>(statementNum, "3", empty)
		};
		std::vector<std::shared_ptr<Node>> plusLeftChildren = {
		  std::make_shared<Node>(statementNum, "a", empty),
		  std::make_shared<Node>(statementNum, "*", multiplyChildren)
		};
		Node plusRight = Node(statementNum, "+", {
		  std::make_shared<Node>(statementNum, "+", plusLeftChildren),
		  std::make_shared<Node>(statementNum, "5", empty)
			});

		bool strictChecking = true;
		assert(plusRight.isIdentical(result, strictChecking));
	}

	TEST_METHOD(bracketsNoChange_success) {
		std::string expression = "((a + (b * 3))) + (((5)))";
		int statementNum = 964;

		std::vector<std::shared_ptr<Token>> tokens = tokeniseWrapper(expression);
		TreeBuilder tb = TreeBuilder(statementNum);
		std::shared_ptr<Node> result = tb.buildTree(tokens);

		std::vector<std::shared_ptr<Node>> empty = {};
		std::vector<std::shared_ptr<Node>> multiplyChildren = {
			std::make_shared<Node>(statementNum, "b", empty),
			std::make_shared<Node>(statementNum, "3", empty)
		};
		std::vector<std::shared_ptr<Node>> plusLeftChildren = {
		  std::make_shared<Node>(statementNum, "a", empty),
		  std::make_shared<Node>(statementNum, "*", multiplyChildren)
		};
		Node plusRight = Node(statementNum, "+", {
		  std::make_shared<Node>(statementNum, "+", plusLeftChildren),
		  std::make_shared<Node>(statementNum, "5", empty)
			});

		bool strictChecking = true;
		assert(plusRight.isIdentical(result, strictChecking));
	}

	TEST_METHOD(bracketsChangeOrder_success) {
		std::string expression = "((a + b) * 3 + 5)";
		int statementNum = 321;

		std::vector<std::shared_ptr<Token>> tokens = tokeniseWrapper(expression);
		TreeBuilder tb = TreeBuilder(statementNum);
		std::shared_ptr<Node> result = tb.buildTree(tokens);

		std::vector<std::shared_ptr<Node>> empty = {};
		std::vector<std::shared_ptr<Node>> plusLeftChildren = {
		  std::make_shared<Node>(statementNum, "a", empty),
		  std::make_shared<Node>(statementNum, "b", empty)
		};
		std::vector<std::shared_ptr<Node>> multiplyChildren = {
			std::make_shared<Node>(statementNum, "+", plusLeftChildren),
			std::make_shared<Node>(statementNum, "3", empty)
		};
		Node plusRight = Node(statementNum, "+", {
		  std::make_shared<Node>(statementNum, "*", multiplyChildren),
		  std::make_shared<Node>(statementNum, "5", empty)
			});

		bool strictChecking = true;
		assert(plusRight.isIdentical(result, strictChecking));
	}

	TEST_METHOD(invalidOp_failure) {
		std::string expression = "a == b";
		int statementNum = 61;

		std::vector<std::shared_ptr<Token>> tokens = tokeniseWrapper(expression);
		TreeBuilder tb = TreeBuilder(statementNum);
		try {
			std::shared_ptr<Node> result = tb.buildTree(tokens);
		}
		catch (std::invalid_argument e) {
			assert(true);
			return;
		}
		assert(false);
	}

	TEST_METHOD(missingOp_failure) {
		std::string expression = "a b";
		int statementNum = 84;

		std::vector<std::shared_ptr<Token>> tokens = tokeniseWrapper(expression);
		TreeBuilder tb = TreeBuilder(statementNum);
		try {
			std::shared_ptr<Node> result = tb.buildTree(tokens);
		}
		catch (std::invalid_argument e) {
			assert(true);
			return;
		}
		assert(false);
	}

	TEST_METHOD(missingOperatee_failure) {
		std::string expression = "+ b";
		int statementNum = 458;

		std::vector<std::shared_ptr<Token>> tokens = tokeniseWrapper(expression);
		TreeBuilder tb = TreeBuilder(statementNum);
		try {
			std::shared_ptr<Node> result = tb.buildTree(tokens);
		}
		catch (std::invalid_argument e) {
			assert(true);
			return;
		}
		assert(false);
	}

	TEST_METHOD(wrongOrder_failure) {
		std::string expression = "a b +";
		int statementNum = 61231;

		std::vector<std::shared_ptr<Token>> tokens = tokeniseWrapper(expression);
		TreeBuilder tb = TreeBuilder(statementNum);
		try {
			std::shared_ptr<Node> result = tb.buildTree(tokens);
		}
		catch (std::invalid_argument e) {
			assert(true);
			return;
		}
		assert(false);
	}

	TEST_METHOD(mismatchParentheses_failure_1) {
		std::string expression = "(a + b";
		int statementNum = 6123;

		std::vector<std::shared_ptr<Token>> tokens = tokeniseWrapper(expression);
		TreeBuilder tb = TreeBuilder(statementNum);
		try {
			std::shared_ptr<Node> result = tb.buildTree(tokens);
		}
		catch (std::invalid_argument e) {
			assert(true);
			return;
		}
		assert(false);
	}

	TEST_METHOD(mismatchParentheses_failure_2) {
		std::string expression = "a + b)";
		int statementNum = 61;

		std::vector<std::shared_ptr<Token>> tokens = tokeniseWrapper(expression);
		TreeBuilder tb = TreeBuilder(statementNum);
		try {
			std::shared_ptr<Node> result = tb.buildTree(tokens);
		}
		catch (std::invalid_argument e) {
			assert(true);
			return;
		}
		assert(false);
	}

	TEST_METHOD(mismatchParentheses_failure_3) {
		std::string expression = ")a + b(";
		int statementNum = 5132;

		std::vector<std::shared_ptr<Token>> tokens = tokeniseWrapper(expression);
		TreeBuilder tb = TreeBuilder(statementNum);
		try {
			std::shared_ptr<Node> result = tb.buildTree(tokens);
		}
		catch (std::invalid_argument e) {
			assert(true);
			return;
		}
		assert(false);
	}

	TEST_METHOD(parenthesesSideBySide) {
		std::string expression = "(a + b) (a * b)";
		int statementNum = 1231;

		std::vector<std::shared_ptr<Token>> tokens = tokeniseWrapper(expression);
		TreeBuilder tb = TreeBuilder(statementNum);
		try {
			std::shared_ptr<Node> result = tb.buildTree(tokens);
		}
		catch (std::invalid_argument e) {
			assert(true);
			return;
		}
		assert(false);
	}

	TEST_METHOD(parenthesesContentsEmpty_failure1) {
		std::string expression = "()a + b";
		int statementNum = 1231;

		std::vector<std::shared_ptr<Token>> tokens = tokeniseWrapper(expression);
		TreeBuilder tb = TreeBuilder(statementNum);
		try {
			std::shared_ptr<Node> result = tb.buildTree(tokens);
		}
		catch (std::invalid_argument e) {
			assert(true);
			return;
		}
		assert(false);
	}

	TEST_METHOD(parenthesesContentsEmpty_failure2) {
		std::string expression = "a + () b";
		int statementNum = 1231;

		std::vector<std::shared_ptr<Token>> tokens = tokeniseWrapper(expression);
		TreeBuilder tb = TreeBuilder(statementNum);
		try {
			std::shared_ptr<Node> result = tb.buildTree(tokens);
		}
		catch (std::invalid_argument e) {
			assert(true);
			return;
		}
		assert(false);
	}

	TEST_METHOD(parenthesesContentsEmpty_failure3) {
		std::string expression = "a + b ()";
		int statementNum = 1231;

		std::vector<std::shared_ptr<Token>> tokens = tokeniseWrapper(expression);
		TreeBuilder tb = TreeBuilder(statementNum);
		try {
			std::shared_ptr<Node> result = tb.buildTree(tokens);
		}
		catch (std::invalid_argument e) {
			assert(true);
			return;
		}
		assert(false);
	}
	};
}