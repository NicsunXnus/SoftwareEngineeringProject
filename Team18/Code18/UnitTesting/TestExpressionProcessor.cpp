#include "stdafx.h"
#include "CppUnitTest.h"

#include "../source/SP/SimpleProcessor/ExpressionProcessor.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ExpressionProcessor_Test
{
	TEST_CLASS(nodifyArithmeticExpression_Test) {
	private:
		bool nodifyArithExpWrapper(string input, shared_ptr<Node> expected) {
			shared_ptr<Node> result;
			try {
				ExpressionProcessor ep = ExpressionProcessor();
				result = ep.nodifyArithmeticExpression(input);
			}
			catch (invalid_argument e) {
				return false;
			}
			return result->isIdentical(expected);
		}

		shared_ptr<Node> dummy = nullptr;
	public:
		TEST_METHOD(empty_failure) {
			string input = "";
			bool result = nodifyArithExpWrapper(input, dummy);
			assert(result == false);
		}

		TEST_METHOD(plus_success) {
			string input = "1 + 2";
			vector<shared_ptr<Node>> empty = {};
			vector<shared_ptr<Node>> children = {
				make_shared<Node>(-1, "1", empty),
				make_shared<Node>(-1, "2", empty)
			};
			shared_ptr<Node> expected = make_shared<Node>(-1, "+", children);
			bool result = nodifyArithExpWrapper(input, expected);
			assert(result);
		}

		TEST_METHOD(minus_success) {
			string input = "1 - 2";
			vector<shared_ptr<Node>> empty = {};
			vector<shared_ptr<Node>> children = {
				make_shared<Node>(-1, "1", empty),
				make_shared<Node>(-1, "2", empty)
			};
			shared_ptr<Node> expected = make_shared<Node>(-1, "-", children);
			bool result = nodifyArithExpWrapper(input, expected);
			assert(result);
		}

		TEST_METHOD(times_success) {
			string input = "1 * 2";
			vector<shared_ptr<Node>> empty = {};
			vector<shared_ptr<Node>> children = {
				make_shared<Node>(-1, "1", empty),
				make_shared<Node>(-1, "2", empty)
			};
			shared_ptr<Node> expected = make_shared<Node>(-1, "*", children);
			bool result = nodifyArithExpWrapper(input, expected);
			assert(result);
		}

		TEST_METHOD(divide_success) {
			string input = "1 / 2";
			vector<shared_ptr<Node>> empty = {};
			vector<shared_ptr<Node>> children = {
				make_shared<Node>(-1, "1", empty),
				make_shared<Node>(-1, "2", empty)
			};
			shared_ptr<Node> expected = make_shared<Node>(-1, "/", children);
			bool result = nodifyArithExpWrapper(input, expected);
			assert(result);
		}

		TEST_METHOD(modulo_success) {
			string input = "1 % 2";
			vector<shared_ptr<Node>> empty = {};
			vector<shared_ptr<Node>> children = {
				make_shared<Node>(-1, "1", empty),
				make_shared<Node>(-1, "2", empty)
			};
			shared_ptr<Node> expected = make_shared<Node>(-1, "%", children);
			bool result = nodifyArithExpWrapper(input, expected);
			assert(result);
		}

		TEST_METHOD(whitespaces_success) {
			string input = " \t\f\v\n\r\b  \t\f\v\n\r\b  1 \t\f\v\n\r\b +  \t\f\v\n\r\b2 \t\f\v\n\r\b";
			vector<shared_ptr<Node>> empty = {};
			vector<shared_ptr<Node>> children = {
				make_shared<Node>(-1, "1", empty),
				make_shared<Node>(-1, "2", empty)
			};
			shared_ptr<Node> expected = make_shared<Node>(-1, "+", children);
			bool result = nodifyArithExpWrapper(input, expected);
			assert(result);
		}

		TEST_METHOD(parentheses_success) {
			string input = "(1 + 2)";
			vector<shared_ptr<Node>> empty = {};
			vector<shared_ptr<Node>> children = {
				make_shared<Node>(-1, "1", empty),
				make_shared<Node>(-1, "2", empty)
			};
			shared_ptr<Node> expected = make_shared<Node>(-1, "+", children);
			bool result = nodifyArithExpWrapper(input, expected);
			assert(result);
		}
		TEST_METHOD(invalidIntegerLiteral_failure) {
			string input = ")01 + * + 2(";
			bool result = nodifyArithExpWrapper(input, dummy);
			assert(result == false);
		}

		TEST_METHOD(invalidName_failure) {
			string input = ")0a + * + 2(";
			bool result = nodifyArithExpWrapper(input, dummy);
			assert(result == false);
		}

		TEST_METHOD(invalidOperation_failure) {
			string input = "1 || 2";
			bool result = nodifyArithExpWrapper(input, dummy);
			assert(result == false);
		}
	};
	TEST_CLASS(processConditionalExpression_Test) {
	private:
		bool nodifyCondExpWrapper(string input, shared_ptr<Node> expected) {
			shared_ptr<Node> result;
			try {
				ExpressionProcessor ep = ExpressionProcessor();
				result = ep.nodifyConditionalExpression(input);
			}
			catch (invalid_argument e) {
				return false;
			}
			return result->isIdentical(expected);
		}

		shared_ptr<Node> dummy = nullptr;
	public:
		TEST_METHOD(empty_failure) {
			string input = "";
			bool result = nodifyCondExpWrapper(input, dummy);
			assert(result == false);
		}

		TEST_METHOD(brackets_success) {
			string input = "(((b1 + 2))) >= (3)";
			vector<shared_ptr<Node>> empty = {};
			vector<shared_ptr<Node>> children = {
				make_shared<Node>(-1, "b1", empty),
				make_shared<Node>(-1, "2", empty)
			};
			vector<shared_ptr<Node>> moreThanChildren = {
				make_shared<Node>(-1, "+", children),
				make_shared<Node>(-1, "3", empty)
			};
			shared_ptr<Node> expected = make_shared<Node>(-1, ">=", moreThanChildren);
			bool result = nodifyCondExpWrapper(input, expected);
			assert(result);
		}

		TEST_METHOD(or_success) {
			string input = "(1>2) || (1<2)";
			vector<shared_ptr<Node>> empty = {};
			vector<shared_ptr<Node>> children = {
				make_shared<Node>(-1, "1", empty),
				make_shared<Node>(-1, "2", empty)
			};

			vector<shared_ptr<Node>> orChild = {
				make_shared<Node>(-1, ">", children),
				make_shared<Node>(-1, "<", children)
			};
			shared_ptr<Node> expected = make_shared<Node>(-1, "||", orChild);
			bool result = nodifyCondExpWrapper(input, expected);
			assert(result);
		}

		TEST_METHOD(and_success) {
			string input = "(1>2) && (1<2)";
			vector<shared_ptr<Node>> empty = {};
			vector<shared_ptr<Node>> children = {
				make_shared<Node>(-1, "1", empty),
				make_shared<Node>(-1, "2", empty)
			};
			vector<shared_ptr<Node>> andChild = {
				make_shared<Node>(-1, ">", children),
				make_shared<Node>(-1, "<", children)
			};
			shared_ptr<Node> expected = make_shared<Node>(-1, "&&", andChild);
			bool result = nodifyCondExpWrapper(input, expected);
			assert(result);
		}

		TEST_METHOD(not_success) {
			string input = "(!(1>2)) || (1<2)";
			vector<shared_ptr<Node>> empty = {};
			vector<shared_ptr<Node>> children = {
				make_shared<Node>(-1, "1", empty),
				make_shared<Node>(-1, "2", empty)
			};
			vector<shared_ptr<Node>> notChild = { make_shared<Node>(-1, ">", children) };
			vector<shared_ptr<Node>> orChild = {
				make_shared<Node>(-1, "!", notChild),
				make_shared<Node>(-1, "<", children)
			};
			shared_ptr<Node> expected = make_shared<Node>(-1, "||", orChild);
			bool result = nodifyCondExpWrapper(input, expected);
			assert(result);
		}

		TEST_METHOD(equality_success) {
			string input = "1 == 2";
			vector<shared_ptr<Node>> empty = {};
			vector<shared_ptr<Node>> children = {
				make_shared<Node>(-1, "1", empty),
				make_shared<Node>(-1, "2", empty)
			};
			shared_ptr<Node> expected = make_shared<Node>(-1, "==", children);
			bool result = nodifyCondExpWrapper(input, expected);
			assert(result);
		}

		TEST_METHOD(inequality_success) {
			string input = "1 != 2";
			vector<shared_ptr<Node>> empty = {};
			vector<shared_ptr<Node>> children = {
				make_shared<Node>(-1, "1", empty),
				make_shared<Node>(-1, "2", empty)
			};
			shared_ptr<Node> expected = make_shared<Node>(-1, "!=", children);
			bool result = nodifyCondExpWrapper(input, expected);
			assert(result);
		}

		TEST_METHOD(moreThan_success) {
			string input = "1 > 2";
			vector<shared_ptr<Node>> empty = {};
			vector<shared_ptr<Node>> children = {
				make_shared<Node>(-1, "1", empty),
				make_shared<Node>(-1, "2", empty)
			};
			shared_ptr<Node> expected = make_shared<Node>(-1, ">", children);
			bool result = nodifyCondExpWrapper(input, expected);
			assert(result);
		}

		TEST_METHOD(moreThanEquals_success) {
			string input = "1 >= 2";
			vector<shared_ptr<Node>> empty = {};
			vector<shared_ptr<Node>> children = {
				make_shared<Node>(-1, "1", empty),
				make_shared<Node>(-1, "2", empty)
			};
			shared_ptr<Node> expected = make_shared<Node>(-1, ">=", children);
			bool result = nodifyCondExpWrapper(input, expected);
			assert(result);
		}

		TEST_METHOD(lessThan_success) {
			string input = "1 < 2";
			vector<shared_ptr<Node>> empty = {};
			vector<shared_ptr<Node>> children = {
				make_shared<Node>(-1, "1", empty),
				make_shared<Node>(-1, "2", empty)
			};
			shared_ptr<Node> expected = make_shared<Node>(-1, "<", children);
			bool result = nodifyCondExpWrapper(input, expected);
			assert(result);
		}

		TEST_METHOD(lessThanEquals_success) {
			string input = "1 <= 2";
			vector<shared_ptr<Node>> empty = {};
			vector<shared_ptr<Node>> children = {
				make_shared<Node>(-1, "1", empty),
				make_shared<Node>(-1, "2", empty)
			};
			shared_ptr<Node> expected = make_shared<Node>(-1, "<=", children);
			bool result = nodifyCondExpWrapper(input, expected);
			assert(result);
		}

		TEST_METHOD(whitespaces_success) {
			string input = " \t\f\v\n\r\b  \t\f\v\n\r\b  1 \t\f\v\n\r\b +  \t\f\v\n\r\b2 \t\f\v\n\r\b< \t\f\v\n\r\b3 \t\f\v\n\r\b";
			vector<shared_ptr<Node>> empty = {};
			vector<shared_ptr<Node>> children = {
				make_shared<Node>(-1, "1", empty),
				make_shared<Node>(-1, "2", empty)
			};
			vector<shared_ptr<Node>> lessThanChildren = {
				make_shared<Node>(-1, "+", children),
				make_shared<Node>(-1, "3", empty)
			};
			shared_ptr<Node> expected = make_shared<Node>(-1, "<", lessThanChildren);
			bool result = nodifyCondExpWrapper(input, expected);
			assert(result);
		}

		TEST_METHOD(parenthesis_success) {
			string input = "(1 + 2) < 3";
			vector<shared_ptr<Node>> empty = {};
			vector<shared_ptr<Node>> children = {
				make_shared<Node>(-1, "1", empty),
				make_shared<Node>(-1, "2", empty)
			};
			vector<shared_ptr<Node>> lessThanChildren = {
				make_shared<Node>(-1, "+", children),
				make_shared<Node>(-1, "3", empty)
			};
			shared_ptr<Node> expected = make_shared<Node>(-1, "<", lessThanChildren);
			bool result = nodifyCondExpWrapper(input, expected);
			assert(result);
		}

		TEST_METHOD(badParentheses_failure_1) {
			string input = "(1 + 2 < 3)";
			bool result = nodifyCondExpWrapper(input, dummy);
			assert(result == false);
		}

		TEST_METHOD(badParentheses_failure_2) {
			string input = "((1 < 3) || (2 > 4))";
			bool result = nodifyCondExpWrapper(input, dummy);
			assert(result == false);
		}

		TEST_METHOD(badSyntax_success) {
			string input = ")1 + * + 2(";
			bool result = nodifyCondExpWrapper(input, dummy);
			assert(result == false);
		}

		TEST_METHOD(invalidIntegerLiteral_failure) {
			string input = "01 + 2 < 3";
			bool result = nodifyCondExpWrapper(input, dummy);
			assert(result == false);
		}

		TEST_METHOD(invalidName_failure) {
			string input = "0a + 2 < 3";
			bool result = nodifyCondExpWrapper(input, dummy);
			assert(result == false);
		}

		TEST_METHOD(everyOp_success) {
			string left = "(a + b * c > 0) || (a - b * c < 1)";
			string middle = "(a - b / c >= 2) || (a * b / c <= 3)";
			string right = "(a % b - c == 4) || (a % b % c != 5)";
			string expression = "(" + left + ") || ((!(" + middle + ")) && (" + right + "))";
			int statementNum = -1;

			vector<shared_ptr<Node>> empty = {};
			vector<shared_ptr<Node>> b_Times_C = {
					make_shared<Node>(statementNum, "b", empty),
					make_shared<Node>(statementNum, "c", empty)
			};
			vector<shared_ptr<Node>> a_Plus_BtimesC = {
					make_shared<Node>(statementNum, "a", empty),
					make_shared<Node>(statementNum, "*", b_Times_C)
			};
			vector<shared_ptr<Node>> aPlusBtimesC_MoreThan_Zero = {
					make_shared<Node>(statementNum, "+", a_Plus_BtimesC),
					make_shared<Node>(statementNum, "0", empty)
			};
			vector<shared_ptr<Node>> a_Minus_BtimesC = {
					make_shared<Node>(statementNum, "a", empty),
					make_shared<Node>(statementNum, "*", b_Times_C)
			};
			vector<shared_ptr<Node>> aMinusBtimesC_LessThan_One = {
					make_shared<Node>(statementNum, "-", a_Minus_BtimesC),
					make_shared<Node>(statementNum, "1", empty)
			};
			vector<shared_ptr<Node>> left_or_ = {
					make_shared<Node>(statementNum, ">", aPlusBtimesC_MoreThan_Zero),
					make_shared<Node>(statementNum, "<", aMinusBtimesC_LessThan_One)
			};

			vector<shared_ptr<Node>> b_Divide_C = {
					make_shared<Node>(statementNum, "b", empty),
					make_shared<Node>(statementNum, "c", empty)
			};
			vector<shared_ptr<Node>> a_Minus_BdivideC = {
					make_shared<Node>(statementNum, "a", empty),
					make_shared<Node>(statementNum, "/", b_Divide_C)
			};
			vector<shared_ptr<Node>> aMinusBdivideC_MoreThanEq_Two = {
					make_shared<Node>(statementNum, "-", a_Minus_BdivideC),
					make_shared<Node>(statementNum, "2", empty)
			};
			vector<shared_ptr<Node>> a_Times_B = {
					make_shared<Node>(statementNum, "a", empty),
					make_shared<Node>(statementNum, "b", empty)
			};
			vector<shared_ptr<Node>> aTimesB_divide_C = {
					make_shared<Node>(statementNum, "*", a_Times_B),
					make_shared<Node>(statementNum, "c", empty)
			};
			vector<shared_ptr<Node>> aTimesBdivideC_LessThanEq_3 = {
					make_shared<Node>(statementNum, "/", aTimesB_divide_C),
					make_shared<Node>(statementNum, "3", empty)
			};
			vector<shared_ptr<Node>> middle_or_ = {
					make_shared<Node>(statementNum, ">=", aMinusBdivideC_MoreThanEq_Two),
					make_shared<Node>(statementNum, "<=", aTimesBdivideC_LessThanEq_3)
			};

			vector<shared_ptr<Node>> a_Mod_B = {
					make_shared<Node>(statementNum, "a", empty),
					make_shared<Node>(statementNum, "b", empty)
			};
			vector<shared_ptr<Node>> aModB_Minus_C = {
					make_shared<Node>(statementNum, "%", a_Mod_B),
					make_shared<Node>(statementNum, "c", empty)
			};
			vector<shared_ptr<Node>> aModBminusC_eq_4 = {
					make_shared<Node>(statementNum, "-", aModB_Minus_C),
					make_shared<Node>(statementNum, "4", empty)
			};
			vector<shared_ptr<Node>> aModB_mod_C = {
					make_shared<Node>(statementNum, "%", a_Mod_B),
					make_shared<Node>(statementNum, "c", empty)
			};
			vector<shared_ptr<Node>> aModBmodC_ineq_5 = {
					make_shared<Node>(statementNum, "%", aModB_mod_C),
					make_shared<Node>(statementNum, "5", empty)
			};
			vector<shared_ptr<Node>> right_or_ = {
					make_shared<Node>(statementNum, "==", aModBminusC_eq_4),
					make_shared<Node>(statementNum, "!=", aModBmodC_ineq_5)
			};

			vector<shared_ptr<Node>> notOp = {
					make_shared<Node>(statementNum, "||", middle_or_)
			};
			vector<shared_ptr<Node>> andOp = {
					make_shared<Node>(statementNum, "!", notOp),
					make_shared<Node>(statementNum, "||", right_or_),
			};
			vector<shared_ptr<Node>> outerOrOp = {
					make_shared<Node>(statementNum, "||", left_or_),
					make_shared<Node>(statementNum, "&&", andOp),
			};

			shared_ptr<Node> expr = make_shared<Node>(statementNum, "||", outerOrOp);

			bool result = nodifyCondExpWrapper(expression, expr);
			assert(result);
		}
	};
}
