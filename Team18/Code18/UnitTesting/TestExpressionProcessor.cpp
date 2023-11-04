#include "stdafx.h"
#include "CppUnitTest.h"

#include "../source/SP/SimpleProcessor/ExpressionProcessor.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ExpressionProcessor_Test
{
	TEST_CLASS(nodifyArithmeticExpression_Test) {
	private:
		bool nodifyArithExpWrapper(std::string input, std::shared_ptr<Node> expected) {
			std::shared_ptr<Node> result;
			try {
				ExpressionProcessor ep = ExpressionProcessor();
				result = ep.nodifyArithmeticExpression(input);
			}
			catch (std::invalid_argument e) {
				return false;
			}
			return result->isIdentical(expected);
		}

		std::shared_ptr<Node> dummy = nullptr;
	public:
		TEST_METHOD(empty_failure) {
			std::string input = "";
			bool result = nodifyArithExpWrapper(input, dummy);
			assert(result == false);
		}

		TEST_METHOD(plus_success) {
			std::string input = "1 + 2";
			std::vector<std::shared_ptr<Node>> empty = {};
			std::vector<std::shared_ptr<Node>> children = {
				std::make_shared<Node>(-1, "1", empty),
				std::make_shared<Node>(-1, "2", empty)
			};
			std::shared_ptr<Node> expected = std::make_shared<Node>(-1, "+", children);
			bool result = nodifyArithExpWrapper(input, expected);
			assert(result);
		}

		TEST_METHOD(minus_success) {
			std::string input = "1 - 2";
			std::vector<std::shared_ptr<Node>> empty = {};
			std::vector<std::shared_ptr<Node>> children = {
				std::make_shared<Node>(-1, "1", empty),
				std::make_shared<Node>(-1, "2", empty)
			};
			std::shared_ptr<Node> expected = std::make_shared<Node>(-1, "-", children);
			bool result = nodifyArithExpWrapper(input, expected);
			assert(result);
		}

		TEST_METHOD(times_success) {
			std::string input = "1 * 2";
			std::vector<std::shared_ptr<Node>> empty = {};
			std::vector<std::shared_ptr<Node>> children = {
				std::make_shared<Node>(-1, "1", empty),
				std::make_shared<Node>(-1, "2", empty)
			};
			std::shared_ptr<Node> expected = std::make_shared<Node>(-1, "*", children);
			bool result = nodifyArithExpWrapper(input, expected);
			assert(result);
		}

		TEST_METHOD(divide_success) {
			std::string input = "1 / 2";
			std::vector<std::shared_ptr<Node>> empty = {};
			std::vector<std::shared_ptr<Node>> children = {
				std::make_shared<Node>(-1, "1", empty),
				std::make_shared<Node>(-1, "2", empty)
			};
			std::shared_ptr<Node> expected = std::make_shared<Node>(-1, "/", children);
			bool result = nodifyArithExpWrapper(input, expected);
			assert(result);
		}

		TEST_METHOD(modulo_success) {
			std::string input = "1 % 2";
			std::vector<std::shared_ptr<Node>> empty = {};
			std::vector<std::shared_ptr<Node>> children = {
				std::make_shared<Node>(-1, "1", empty),
				std::make_shared<Node>(-1, "2", empty)
			};
			std::shared_ptr<Node> expected = std::make_shared<Node>(-1, "%", children);
			bool result = nodifyArithExpWrapper(input, expected);
			assert(result);
		}

		TEST_METHOD(whitespaces_success) {
			std::string input = " \t\f\v\n\r\b  \t\f\v\n\r\b  1 \t\f\v\n\r\b +  \t\f\v\n\r\b2 \t\f\v\n\r\b";
			std::vector<std::shared_ptr<Node>> empty = {};
			std::vector<std::shared_ptr<Node>> children = {
				std::make_shared<Node>(-1, "1", empty),
				std::make_shared<Node>(-1, "2", empty)
			};
			std::shared_ptr<Node> expected = std::make_shared<Node>(-1, "+", children);
			bool result = nodifyArithExpWrapper(input, expected);
			assert(result);
		}

		TEST_METHOD(parentheses_success) {
			std::string input = "(1 + 2)";
			std::vector<std::shared_ptr<Node>> empty = {};
			std::vector<std::shared_ptr<Node>> children = {
				std::make_shared<Node>(-1, "1", empty),
				std::make_shared<Node>(-1, "2", empty)
			};
			std::shared_ptr<Node> expected = std::make_shared<Node>(-1, "+", children);
			bool result = nodifyArithExpWrapper(input, expected);
			assert(result);
		}

		TEST_METHOD(badSyntax_failure) {
			std::string input = ")1 + * + 2(";
			bool result = nodifyArithExpWrapper(input, dummy);
			assert(result == false);
		}

		TEST_METHOD(invalidIntegerLiteral_failure) {
			std::string input = ")01 + * + 2(";
			bool result = nodifyArithExpWrapper(input, dummy);
			assert(result == false);
		}

		TEST_METHOD(invalidName_failure) {
			std::string input = ")0a + * + 2(";
			bool result = nodifyArithExpWrapper(input, dummy);
			assert(result == false);
		}

		TEST_METHOD(invalidOperation_failure) {
			std::string input = "1 || 2";
			bool result = nodifyArithExpWrapper(input, dummy);
			assert(result == false);
		}
	};
	TEST_CLASS(processConditionalExpression_Test) {
	private:
		bool nodifyCondExpWrapper(std::string input, std::shared_ptr<Node> expected) {
			std::shared_ptr<Node> result;
			try {
				ExpressionProcessor ep = ExpressionProcessor();
				result = ep.nodifyConditionalExpression(input);
			}
			catch (std::invalid_argument e) {
				return false;
			}
			return result->isIdentical(expected);
		}

		std::shared_ptr<Node> dummy = nullptr;
	public:
		TEST_METHOD(empty_failure) {
			std::string input = "";
			bool result = nodifyCondExpWrapper(input, dummy);
			assert(result == false);
		}

		TEST_METHOD(brackets_success) {
			std::string input = "(((b1 + 2))) >= (3)";
			std::vector<std::shared_ptr<Node>> empty = {};
			std::vector<std::shared_ptr<Node>> children = {
				std::make_shared<Node>(-1, "b1", empty),
				std::make_shared<Node>(-1, "2", empty)
			};
			std::vector<std::shared_ptr<Node>> moreThanChildren = {
				std::make_shared<Node>(-1, "+", children),
				std::make_shared<Node>(-1, "3", empty)
			};
			std::shared_ptr<Node> expected = std::make_shared<Node>(-1, ">=", moreThanChildren);
			bool result = nodifyCondExpWrapper(input, expected);
			assert(result);
		}

		TEST_METHOD(or_success) {
			std::string input = "(1>2) || (1<2)";
			std::vector<std::shared_ptr<Node>> empty = {};
			std::vector<std::shared_ptr<Node>> children = {
				std::make_shared<Node>(-1, "1", empty),
				std::make_shared<Node>(-1, "2", empty)
			};

			std::vector<std::shared_ptr<Node>> orChild = {
				std::make_shared<Node>(-1, ">", children),
				std::make_shared<Node>(-1, "<", children)
			};
			std::shared_ptr<Node> expected = std::make_shared<Node>(-1, "||", orChild);
			bool result = nodifyCondExpWrapper(input, expected);
			assert(result);
		}

		TEST_METHOD(and_success) {
			std::string input = "(1>2) && (1<2)";
			std::vector<std::shared_ptr<Node>> empty = {};
			std::vector<std::shared_ptr<Node>> children = {
				std::make_shared<Node>(-1, "1", empty),
				std::make_shared<Node>(-1, "2", empty)
			};
			std::vector<std::shared_ptr<Node>> andChild = {
				std::make_shared<Node>(-1, ">", children),
				std::make_shared<Node>(-1, "<", children)
			};
			std::shared_ptr<Node> expected = std::make_shared<Node>(-1, "&&", andChild);
			bool result = nodifyCondExpWrapper(input, expected);
			assert(result);
		}

		TEST_METHOD(not_success) {
			std::string input = "(!(1>2)) || (1<2)";
			std::vector<std::shared_ptr<Node>> empty = {};
			std::vector<std::shared_ptr<Node>> children = {
				std::make_shared<Node>(-1, "1", empty),
				std::make_shared<Node>(-1, "2", empty)
			};
			std::vector<std::shared_ptr<Node>> notChild = { std::make_shared<Node>(-1, ">", children) };
			std::vector<std::shared_ptr<Node>> orChild = {
				std::make_shared<Node>(-1, "!", notChild),
				std::make_shared<Node>(-1, "<", children)
			};
			std::shared_ptr<Node> expected = std::make_shared<Node>(-1, "||", orChild);
			bool result = nodifyCondExpWrapper(input, expected);
			assert(result);
		}

		TEST_METHOD(equality_success) {
			std::string input = "1 == 2";
			std::vector<std::shared_ptr<Node>> empty = {};
			std::vector<std::shared_ptr<Node>> children = {
				std::make_shared<Node>(-1, "1", empty),
				std::make_shared<Node>(-1, "2", empty)
			};
			std::shared_ptr<Node> expected = std::make_shared<Node>(-1, "==", children);
			bool result = nodifyCondExpWrapper(input, expected);
			assert(result);
		}

		TEST_METHOD(inequality_success) {
			std::string input = "1 != 2";
			std::vector<std::shared_ptr<Node>> empty = {};
			std::vector<std::shared_ptr<Node>> children = {
				std::make_shared<Node>(-1, "1", empty),
				std::make_shared<Node>(-1, "2", empty)
			};
			std::shared_ptr<Node> expected = std::make_shared<Node>(-1, "!=", children);
			bool result = nodifyCondExpWrapper(input, expected);
			assert(result);
		}

		TEST_METHOD(moreThan_success) {
			std::string input = "1 > 2";
			std::vector<std::shared_ptr<Node>> empty = {};
			std::vector<std::shared_ptr<Node>> children = {
				std::make_shared<Node>(-1, "1", empty),
				std::make_shared<Node>(-1, "2", empty)
			};
			std::shared_ptr<Node> expected = std::make_shared<Node>(-1, ">", children);
			bool result = nodifyCondExpWrapper(input, expected);
			assert(result);
		}

		TEST_METHOD(moreThanEquals_success) {
			std::string input = "1 >= 2";
			std::vector<std::shared_ptr<Node>> empty = {};
			std::vector<std::shared_ptr<Node>> children = {
				std::make_shared<Node>(-1, "1", empty),
				std::make_shared<Node>(-1, "2", empty)
			};
			std::shared_ptr<Node> expected = std::make_shared<Node>(-1, ">=", children);
			bool result = nodifyCondExpWrapper(input, expected);
			assert(result);
		}

		TEST_METHOD(lessThan_success) {
			std::string input = "1 < 2";
			std::vector<std::shared_ptr<Node>> empty = {};
			std::vector<std::shared_ptr<Node>> children = {
				std::make_shared<Node>(-1, "1", empty),
				std::make_shared<Node>(-1, "2", empty)
			};
			std::shared_ptr<Node> expected = std::make_shared<Node>(-1, "<", children);
			bool result = nodifyCondExpWrapper(input, expected);
			assert(result);
		}

		TEST_METHOD(lessThanEquals_success) {
			std::string input = "1 <= 2";
			std::vector<std::shared_ptr<Node>> empty = {};
			std::vector<std::shared_ptr<Node>> children = {
				std::make_shared<Node>(-1, "1", empty),
				std::make_shared<Node>(-1, "2", empty)
			};
			std::shared_ptr<Node> expected = std::make_shared<Node>(-1, "<=", children);
			bool result = nodifyCondExpWrapper(input, expected);
			assert(result);
		}

		TEST_METHOD(whitespaces_success) {
			std::string input = " \t\f\v\n\r\b  \t\f\v\n\r\b  1 \t\f\v\n\r\b +  \t\f\v\n\r\b2 \t\f\v\n\r\b< \t\f\v\n\r\b3 \t\f\v\n\r\b";
			std::vector<std::shared_ptr<Node>> empty = {};
			std::vector<std::shared_ptr<Node>> children = {
				std::make_shared<Node>(-1, "1", empty),
				std::make_shared<Node>(-1, "2", empty)
			};
			std::vector<std::shared_ptr<Node>> lessThanChildren = {
				std::make_shared<Node>(-1, "+", children),
				std::make_shared<Node>(-1, "3", empty)
			};
			std::shared_ptr<Node> expected = std::make_shared<Node>(-1, "<", lessThanChildren);
			bool result = nodifyCondExpWrapper(input, expected);
			assert(result);
		}

		TEST_METHOD(parenthesis_success) {
			std::string input = "(1 + 2) < 3";
			std::vector<std::shared_ptr<Node>> empty = {};
			std::vector<std::shared_ptr<Node>> children = {
				std::make_shared<Node>(-1, "1", empty),
				std::make_shared<Node>(-1, "2", empty)
			};
			std::vector<std::shared_ptr<Node>> lessThanChildren = {
				std::make_shared<Node>(-1, "+", children),
				std::make_shared<Node>(-1, "3", empty)
			};
			std::shared_ptr<Node> expected = std::make_shared<Node>(-1, "<", lessThanChildren);
			bool result = nodifyCondExpWrapper(input, expected);
			assert(result);
		}

		TEST_METHOD(badParentheses_failure_1) {
			std::string input = "(1 + 2 < 3)";
			bool result = nodifyCondExpWrapper(input, dummy);
			assert(result == false);
		}

		TEST_METHOD(badParentheses_failure_2) {
			std::string input = "((1 < 3) || (2 > 4))";
			bool result = nodifyCondExpWrapper(input, dummy);
			assert(result == false);
		}

		TEST_METHOD(badSyntax_success) {
			std::string input = ")1 + * + 2(";
			bool result = nodifyCondExpWrapper(input, dummy);
			assert(result == false);
		}

		TEST_METHOD(invalidIntegerLiteral_failure) {
			std::string input = "01 + 2 < 3";
			bool result = nodifyCondExpWrapper(input, dummy);
			assert(result == false);
		}

		TEST_METHOD(invalidName_failure) {
			std::string input = "0a + 2 < 3";
			bool result = nodifyCondExpWrapper(input, dummy);
			assert(result == false);
		}

		TEST_METHOD(everyOp_success) {
			std::string left = "(a + b * c > 6) || (a - b * c < 1)";
			std::string middle = "(a - b / c >= 2) || (a * b / c <= 3)";
			std::string right = "(a % b - c == 4) || (a % b % c != 5)";
			std::string expression = "(" + left + ") || ((!(" + middle + ")) && (" + right + "))";
			int statementNum = -1;

			std::vector<std::shared_ptr<Node>> empty = {};
			std::vector<std::shared_ptr<Node>> b_Times_C = {
					std::make_shared<Node>(statementNum, "b", empty),
					std::make_shared<Node>(statementNum, "c", empty)
			};
			std::vector<std::shared_ptr<Node>> a_Plus_BtimesC = {
					std::make_shared<Node>(statementNum, "a", empty),
					std::make_shared<Node>(statementNum, "*", b_Times_C)
			};
			std::vector<std::shared_ptr<Node>> aPlusBtimesC_MoreThan_Zero = {
					std::make_shared<Node>(statementNum, "+", a_Plus_BtimesC),
					std::make_shared<Node>(statementNum, "6", empty)
			};
			std::vector<std::shared_ptr<Node>> a_Minus_BtimesC = {
					std::make_shared<Node>(statementNum, "a", empty),
					std::make_shared<Node>(statementNum, "*", b_Times_C)
			};
			std::vector<std::shared_ptr<Node>> aMinusBtimesC_LessThan_One = {
					std::make_shared<Node>(statementNum, "-", a_Minus_BtimesC),
					std::make_shared<Node>(statementNum, "1", empty)
			};
			std::vector<std::shared_ptr<Node>> left_or_ = {
					std::make_shared<Node>(statementNum, ">", aPlusBtimesC_MoreThan_Zero),
					std::make_shared<Node>(statementNum, "<", aMinusBtimesC_LessThan_One)
			};

			std::vector<std::shared_ptr<Node>> b_Divide_C = {
					std::make_shared<Node>(statementNum, "b", empty),
					std::make_shared<Node>(statementNum, "c", empty)
			};
			std::vector<std::shared_ptr<Node>> a_Minus_BdivideC = {
					std::make_shared<Node>(statementNum, "a", empty),
					std::make_shared<Node>(statementNum, "/", b_Divide_C)
			};
			std::vector<std::shared_ptr<Node>> aMinusBdivideC_MoreThanEq_Two = {
					std::make_shared<Node>(statementNum, "-", a_Minus_BdivideC),
					std::make_shared<Node>(statementNum, "2", empty)
			};
			std::vector<std::shared_ptr<Node>> a_Times_B = {
					std::make_shared<Node>(statementNum, "a", empty),
					std::make_shared<Node>(statementNum, "b", empty)
			};
			std::vector<std::shared_ptr<Node>> aTimesB_divide_C = {
					std::make_shared<Node>(statementNum, "*", a_Times_B),
					std::make_shared<Node>(statementNum, "c", empty)
			};
			std::vector<std::shared_ptr<Node>> aTimesBdivideC_LessThanEq_3 = {
					std::make_shared<Node>(statementNum, "/", aTimesB_divide_C),
					std::make_shared<Node>(statementNum, "3", empty)
			};
			std::vector<std::shared_ptr<Node>> middle_or_ = {
					std::make_shared<Node>(statementNum, ">=", aMinusBdivideC_MoreThanEq_Two),
					std::make_shared<Node>(statementNum, "<=", aTimesBdivideC_LessThanEq_3)
			};

			std::vector<std::shared_ptr<Node>> a_Mod_B = {
					std::make_shared<Node>(statementNum, "a", empty),
					std::make_shared<Node>(statementNum, "b", empty)
			};
			std::vector<std::shared_ptr<Node>> aModB_Minus_C = {
					std::make_shared<Node>(statementNum, "%", a_Mod_B),
					std::make_shared<Node>(statementNum, "c", empty)
			};
			std::vector<std::shared_ptr<Node>> aModBminusC_eq_4 = {
					std::make_shared<Node>(statementNum, "-", aModB_Minus_C),
					std::make_shared<Node>(statementNum, "4", empty)
			};
			std::vector<std::shared_ptr<Node>> aModB_mod_C = {
					std::make_shared<Node>(statementNum, "%", a_Mod_B),
					std::make_shared<Node>(statementNum, "c", empty)
			};
			std::vector<std::shared_ptr<Node>> aModBmodC_ineq_5 = {
					std::make_shared<Node>(statementNum, "%", aModB_mod_C),
					std::make_shared<Node>(statementNum, "5", empty)
			};
			std::vector<std::shared_ptr<Node>> right_or_ = {
					std::make_shared<Node>(statementNum, "==", aModBminusC_eq_4),
					std::make_shared<Node>(statementNum, "!=", aModBmodC_ineq_5)
			};

			std::vector<std::shared_ptr<Node>> notOp = {
					std::make_shared<Node>(statementNum, "||", middle_or_)
			};
			std::vector<std::shared_ptr<Node>> andOp = {
					std::make_shared<Node>(statementNum, "!", notOp),
					std::make_shared<Node>(statementNum, "||", right_or_),
			};
			std::vector<std::shared_ptr<Node>> outerOrOp = {
					std::make_shared<Node>(statementNum, "||", left_or_),
					std::make_shared<Node>(statementNum, "&&", andOp),
			};

			std::shared_ptr<Node> expr = std::make_shared<Node>(statementNum, "||", outerOrOp);

			bool result = nodifyCondExpWrapper(expression, expr);
			assert(result);
		}
	};
}
