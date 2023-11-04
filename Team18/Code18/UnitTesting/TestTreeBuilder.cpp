#include "stdafx.h"
#include "CppUnitTest.h"
#include "../source/SP/SimpleProcessor/ExpressionProcessor.h"
#include "../source/SP/SimpleProcessor/SimpleProcessor.h"
#include "../source/SP/AST/TreeBuilder.h"
#include "../source/SP/SimpleTokens/TokenFactory.h"
#include "../source/SP/SimpleTokens/IdentifierToken.h"
#include <cassert>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace TreeBuilder_Test
{
	TEST_CLASS(arithmeticExpressions_Test) {
private:
	vector<shared_ptr<Token>> tokeniseWrapper(string expression) {
		vector<shared_ptr<Token>> out;
		for (char c : expression) {
			if (c == ' ') {
				continue;
			}
			string s{ c };
			out.push_back(TokenFactory::generateTokenForSimple(string_view(s)));
		}
		return out;
	}
public:
	TEST_METHOD(PlusMinus_success) {
		string expression = "a + b - 3";
		int statementNum = 5;

		vector<shared_ptr<Token>> tokens = tokeniseWrapper(expression);
		TreeBuilder tb = TreeBuilder(statementNum);
		shared_ptr<Node> result = tb.buildTree(tokens);

		vector<shared_ptr<Node>> empty = {};
		vector<shared_ptr<Node>> plusChildren = {
			make_shared<Node>(statementNum, "a", empty),
			make_shared<Node>(statementNum, "b", empty)
		};
		Node minus = Node(statementNum, "-", {
		  make_shared<Node>(statementNum, "+", plusChildren),
		  make_shared<Node>(statementNum, "3", empty)
			});

		bool strictChecking = true;
		assert(minus.isIdentical(result, strictChecking));
	}

	TEST_METHOD(MultiplyDivideModulo_success) {
		string expression = "a * b / 3 % 5";
		int statementNum = 051;

		vector<shared_ptr<Token>> tokens = tokeniseWrapper(expression);
		TreeBuilder tb = TreeBuilder(statementNum);
		shared_ptr<Node> result = tb.buildTree(tokens);

		vector<shared_ptr<Node>> empty = {};
		vector<shared_ptr<Node>> multiplyChildren = {
			make_shared<Node>(statementNum, "a", empty),
			make_shared<Node>(statementNum, "b", empty)
		};
		vector<shared_ptr<Node>> divideChildren = {
		  make_shared<Node>(statementNum, "*", multiplyChildren),
		  make_shared<Node>(statementNum, "3", empty)
		};
		Node modulo = Node(statementNum, "%", {
		  make_shared<Node>(statementNum, "/", divideChildren),
		  make_shared<Node>(statementNum, "5", empty)
			});

		bool strictChecking = true;
		assert(modulo.isIdentical(result, strictChecking));
	}

	TEST_METHOD(PlusMultiply_success) {
		string expression = "a + b * 3 + 5";
		int statementNum = 510;

		vector<shared_ptr<Token>> tokens = tokeniseWrapper(expression);
		TreeBuilder tb = TreeBuilder(statementNum);
		shared_ptr<Node> result = tb.buildTree(tokens);

		vector<shared_ptr<Node>> empty = {};
		vector<shared_ptr<Node>> multiplyChildren = {
			make_shared<Node>(statementNum, "b", empty),
			make_shared<Node>(statementNum, "3", empty)
		};
		vector<shared_ptr<Node>> plusLeftChildren = {
		  make_shared<Node>(statementNum, "a", empty),
		  make_shared<Node>(statementNum, "*", multiplyChildren)
		};
		Node plusRight = Node(statementNum, "+", {
		  make_shared<Node>(statementNum, "+", plusLeftChildren),
		  make_shared<Node>(statementNum, "5", empty)
			});

		bool strictChecking = true;
		assert(plusRight.isIdentical(result, strictChecking));
	}

	TEST_METHOD(bracketsNoChange_success) {
		string expression = "((a + (b * 3))) + (((5)))";
		int statementNum = 964;

		vector<shared_ptr<Token>> tokens = tokeniseWrapper(expression);
		TreeBuilder tb = TreeBuilder(statementNum);
		shared_ptr<Node> result = tb.buildTree(tokens);

		vector<shared_ptr<Node>> empty = {};
		vector<shared_ptr<Node>> multiplyChildren = {
			make_shared<Node>(statementNum, "b", empty),
			make_shared<Node>(statementNum, "3", empty)
		};
		vector<shared_ptr<Node>> plusLeftChildren = {
		  make_shared<Node>(statementNum, "a", empty),
		  make_shared<Node>(statementNum, "*", multiplyChildren)
		};
		Node plusRight = Node(statementNum, "+", {
		  make_shared<Node>(statementNum, "+", plusLeftChildren),
		  make_shared<Node>(statementNum, "5", empty)
			});

		bool strictChecking = true;
		assert(plusRight.isIdentical(result, strictChecking));
	}

	TEST_METHOD(bracketsChangeOrder_success) {
		string expression = "((a + b) * 3 + 5)";
		int statementNum = 321;

		vector<shared_ptr<Token>> tokens = tokeniseWrapper(expression);
		TreeBuilder tb = TreeBuilder(statementNum);
		shared_ptr<Node> result = tb.buildTree(tokens);

		vector<shared_ptr<Node>> empty = {};
		vector<shared_ptr<Node>> plusLeftChildren = {
		  make_shared<Node>(statementNum, "a", empty),
		  make_shared<Node>(statementNum, "b", empty)
		};
		vector<shared_ptr<Node>> multiplyChildren = {
			make_shared<Node>(statementNum, "+", plusLeftChildren),
			make_shared<Node>(statementNum, "3", empty)
		};
		Node plusRight = Node(statementNum, "+", {
		  make_shared<Node>(statementNum, "*", multiplyChildren),
		  make_shared<Node>(statementNum, "5", empty)
			});

		bool strictChecking = true;
		assert(plusRight.isIdentical(result, strictChecking));
	}

	TEST_METHOD(invalidOp_failure) {
		string expression = "a == b";
		int statementNum = 61;

		vector<shared_ptr<Token>> tokens = {
			make_shared<IdentifierToken>("a"),
			make_shared<Token>("=="),
			make_shared<IdentifierToken>("b"),
		};
		TreeBuilder tb = TreeBuilder(statementNum);
		try {
			shared_ptr<Node> result = tb.buildTree(tokens);
		}
		catch (invalid_argument e) {
			assert(true);
			return;
		}
		assert(false);
	}

	TEST_METHOD(missingOp_failure) {
		string expression = "a b";
		int statementNum = 84;

		vector<shared_ptr<Token>> tokens = tokeniseWrapper(expression);
		TreeBuilder tb = TreeBuilder(statementNum);
		try {
			shared_ptr<Node> result = tb.buildTree(tokens);
		}
		catch (invalid_argument e) {
			assert(true);
			return;
		}
		assert(false);
	}

	TEST_METHOD(missingOperatee_failure) {
		string expression = "+ b";
		int statementNum = 458;

		vector<shared_ptr<Token>> tokens = tokeniseWrapper(expression);
		TreeBuilder tb = TreeBuilder(statementNum);
		try {
			shared_ptr<Node> result = tb.buildTree(tokens);
		}
		catch (invalid_argument e) {
			assert(true);
			return;
		}
		assert(false);
	}

	TEST_METHOD(wrongOrder_failure) {
		string expression = "a b +";
		int statementNum = 61231;

		vector<shared_ptr<Token>> tokens = tokeniseWrapper(expression);
		TreeBuilder tb = TreeBuilder(statementNum);
		try {
			shared_ptr<Node> result = tb.buildTree(tokens);
		}
		catch (invalid_argument e) {
			assert(true);
			return;
		}
		assert(false);
	}

	TEST_METHOD(mismatchParentheses_failure_1) {
		string expression = "(a + b";
		int statementNum = 6123;

		vector<shared_ptr<Token>> tokens = tokeniseWrapper(expression);
		TreeBuilder tb = TreeBuilder(statementNum);
		try {
			shared_ptr<Node> result = tb.buildTree(tokens);
		}
		catch (invalid_argument e) {
			assert(true);
			return;
		}
		assert(false);
	}

	TEST_METHOD(mismatchParentheses_failure_2) {
		string expression = "a + b)";
		int statementNum = 61;

		vector<shared_ptr<Token>> tokens = tokeniseWrapper(expression);
		TreeBuilder tb = TreeBuilder(statementNum);
		try {
			shared_ptr<Node> result = tb.buildTree(tokens);
		}
		catch (invalid_argument e) {
			assert(true);
			return;
		}
		assert(false);
	}

	TEST_METHOD(mismatchParentheses_failure_3) {
		string expression = ")a + b(";
		int statementNum = 5132;

		vector<shared_ptr<Token>> tokens = tokeniseWrapper(expression);
		TreeBuilder tb = TreeBuilder(statementNum);
		try {
			shared_ptr<Node> result = tb.buildTree(tokens);
		}
		catch (invalid_argument e) {
			assert(true);
			return;
		}
		assert(false);
	}

	TEST_METHOD(parenthesesSideBySide) {
		string expression = "(a + b) (a * b)";
		int statementNum = 1231;

		vector<shared_ptr<Token>> tokens = tokeniseWrapper(expression);
		TreeBuilder tb = TreeBuilder(statementNum);
		try {
			shared_ptr<Node> result = tb.buildTree(tokens);
		}
		catch (invalid_argument e) {
			assert(true);
			return;
		}
		assert(false);
	}

	TEST_METHOD(parenthesesContentsEmpty_failure1) {
		string expression = "()a + b";
		int statementNum = 1231;

		vector<shared_ptr<Token>> tokens = tokeniseWrapper(expression);
		TreeBuilder tb = TreeBuilder(statementNum);
		try {
			shared_ptr<Node> result = tb.buildTree(tokens);
		}
		catch (invalid_argument e) {
			assert(true);
			return;
		}
		assert(false);
	}

	TEST_METHOD(parenthesesContentsEmpty_failure2) {
		string expression = "a + () b";
		int statementNum = 1231;

		vector<shared_ptr<Token>> tokens = tokeniseWrapper(expression);
		TreeBuilder tb = TreeBuilder(statementNum);
		try {
			shared_ptr<Node> result = tb.buildTree(tokens);
		}
		catch (invalid_argument e) {
			assert(true);
			return;
		}
		assert(false);
	}

	TEST_METHOD(parenthesesContentsEmpty_failure3) {
		string expression = "a + b ()";
		int statementNum = 1231;

		vector<shared_ptr<Token>> tokens = tokeniseWrapper(expression);
		TreeBuilder tb = TreeBuilder(statementNum);
		try {
			shared_ptr<Node> result = tb.buildTree(tokens);
		}
		catch (invalid_argument e) {
			assert(true);
			return;
		}
		assert(false);
	}
	};
}