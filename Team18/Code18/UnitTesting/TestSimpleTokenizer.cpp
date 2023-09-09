#include "stdafx.h"
#include "CppUnitTest.h"

#include "../source/TokenizerClasses/SimpleTokenizer.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace SimpleTokeniser_Test
{
	TEST_CLASS(tokeniseMethod_Test) {
	private:
		bool equalityWrapper(std::vector<std::vector<std::shared_ptr<Token>>> first,
			std::vector<std::vector<std::shared_ptr<Token>>> second) {
			if (!&first || !&second) { // If either are nulls
				return false;
			}
			bool output = true;
			int firstSize = first.size();
			// Check for first dimension same size
			output = output && firstSize == second.size();
			// if no first dimension or not same size, return output
			if (!output || firstSize == 0) return output;
			for (int stmtNum = 0; stmtNum < firstSize; stmtNum++) {
				std::vector<std::shared_ptr<Token>> firstStmt = first[stmtNum];
				std::vector<std::shared_ptr<Token>> secondStmt = second[stmtNum];
				int firstSize = firstStmt.size();
				// Check for second dimension same size
				output = output && firstSize == secondStmt.size();
				// if no second dimension or not same size, return output
				if (!output || firstSize == 0) return output;
				for (int tokenNum = 0; tokenNum < firstSize; tokenNum++) {
					std::shared_ptr<Token> firstToken = firstStmt[tokenNum];
					std::shared_ptr<Token> secondToken = secondStmt[tokenNum];
					output = output && Token::checkEquality(firstToken, secondToken);
				}
			}
			return output;
		}
	public:
		TEST_METHOD(emptyInput_success) {
			std::string input = "";
			std::vector < std::vector<std::shared_ptr<Token>>> output = SimpleTokenizer::tokenize(input);
			std::vector < std::vector<std::shared_ptr<Token>>> expected = {};
			assert(equalityWrapper(output, expected) == true);
		}

		TEST_METHOD(oneStatement_success) {
			std::string input = "x = 1 + 2 - 3 * 4 / 5 % 6;";
			std::vector < std::vector<std::shared_ptr<Token>>> output = SimpleTokenizer::tokenize(input);
			std::vector < std::vector<std::shared_ptr<Token>>> expected = {
				{
					std::make_shared<IdentifierToken>("x"),
					std::make_shared<EqualsOpToken>(),
					std::make_shared<IntegerLiteralToken>("1"),
					std::make_shared<PlusOpToken>(),
					std::make_shared<IntegerLiteralToken>("2"),
					std::make_shared<MinusOpToken>(),
					std::make_shared<IntegerLiteralToken>("3"),
					std::make_shared<MultiplyOpToken>(),
					std::make_shared<IntegerLiteralToken>("4"),
					std::make_shared<DivideOpToken>(),
					std::make_shared<IntegerLiteralToken>("5"),
					std::make_shared<ModuloOpToken>(),
					std::make_shared<IntegerLiteralToken>("6"),
					std::make_shared<SemicolonSepToken>()
				}
			};
			assert(equalityWrapper(output, expected) == true);
		}

		TEST_METHOD(oneStatementBadStyle_success) {
			std::string input = " \v \tx=1\n +\r 2 \f- 3 \b* 4 / \b5 % \v6 ; \n\t ";
			std::vector < std::vector<std::shared_ptr<Token>>> output = SimpleTokenizer::tokenize(input);
			std::vector < std::vector<std::shared_ptr<Token>>> expected = {
				{
					std::make_shared<IdentifierToken>("x"),
					std::make_shared<EqualsOpToken>(),
					std::make_shared<IntegerLiteralToken>("1"),
					std::make_shared<PlusOpToken>(),
					std::make_shared<IntegerLiteralToken>("2"),
					std::make_shared<MinusOpToken>(),
					std::make_shared<IntegerLiteralToken>("3"),
					std::make_shared<MultiplyOpToken>(),
					std::make_shared<IntegerLiteralToken>("4"),
					std::make_shared<DivideOpToken>(),
					std::make_shared<IntegerLiteralToken>("5"),
					std::make_shared<ModuloOpToken>(),
					std::make_shared<IntegerLiteralToken>("6"),
					std::make_shared<SemicolonSepToken>()
				}
			};
			assert(equalityWrapper(output, expected) == true);
		}

		TEST_METHOD(multipleStatements_success) {
			std::string input =
				"read num1;"
				"read num2;"

				"sum = num1 + num2;"
				"ave = sum / 2;"

				"print ave;";
			std::vector < std::vector<std::shared_ptr<Token>>> output = SimpleTokenizer::tokenize(input);
			std::vector < std::vector<std::shared_ptr<Token>>> expected = {
				{
					std::make_shared<ReadKeywordToken>(),
					std::make_shared<IdentifierToken>("num1"),
					std::make_shared<SemicolonSepToken>()
				},
				{
					std::make_shared<ReadKeywordToken>(),
					std::make_shared<IdentifierToken>("num2"),
					std::make_shared<SemicolonSepToken>()
				},
				{
					std::make_shared<IdentifierToken>("sum"),
					std::make_shared<EqualsOpToken>(),
					std::make_shared<IdentifierToken>("num1"),
					std::make_shared<PlusOpToken>(),
					std::make_shared<IdentifierToken>("num2"),
					std::make_shared<SemicolonSepToken>()
				},
				{
					std::make_shared<IdentifierToken>("ave"),
					std::make_shared<EqualsOpToken>(),
					std::make_shared<IdentifierToken>("sum"),
					std::make_shared<DivideOpToken>(),
					std::make_shared<IntegerLiteralToken>("2"),
					std::make_shared<SemicolonSepToken>()
				},
				{
					std::make_shared<PrintKeywordToken>(),
					std::make_shared<IdentifierToken>("ave"),
					std::make_shared<SemicolonSepToken>()
				}

			};
			assert(equalityWrapper(output, expected) == true);
		}

		TEST_METHOD(multipleStatementsBadStyle_success) {
			std::string input =
				"       \f  read    num1  \t\b\r;read    num2;"
				"     \f    sum=num1     +num2  ;\n"
				" ave=    sum/2;\n"
				"    \v    print    ave    \t ;\n\n\n\n\t\b ";
			std::vector < std::vector<std::shared_ptr<Token>>> output = SimpleTokenizer::tokenize(input);
			std::vector < std::vector<std::shared_ptr<Token>>> expected = {
				{
					std::make_shared<ReadKeywordToken>(),
					std::make_shared<IdentifierToken>("num1"),
					std::make_shared<SemicolonSepToken>()
				},
				{
					std::make_shared<ReadKeywordToken>(),
					std::make_shared<IdentifierToken>("num2"),
					std::make_shared<SemicolonSepToken>()
				},
				{
					std::make_shared<IdentifierToken>("sum"),
					std::make_shared<EqualsOpToken>(),
					std::make_shared<IdentifierToken>("num1"),
					std::make_shared<PlusOpToken>(),
					std::make_shared<IdentifierToken>("num2"),
					std::make_shared<SemicolonSepToken>()
				},
				{
					std::make_shared<IdentifierToken>("ave"),
					std::make_shared<EqualsOpToken>(),
					std::make_shared<IdentifierToken>("sum"),
					std::make_shared<DivideOpToken>(),
					std::make_shared<IntegerLiteralToken>("2"),
					std::make_shared<SemicolonSepToken>()
				},
				{
					std::make_shared<PrintKeywordToken>(),
					std::make_shared<IdentifierToken>("ave"),
					std::make_shared<SemicolonSepToken>()
				}
			};
			assert(equalityWrapper(output, expected) == true);
		}

		TEST_METHOD(semicolonsOnly_failure) {
			try {
				std::string input = ";;;";
				std::vector < std::vector<std::shared_ptr<Token>>> output = SimpleTokenizer::tokenize(input);
				assert(false);
			}
			catch (std::invalid_argument e) { assert(true); }
		}

		TEST_METHOD(invalidStatements_failure) {
			try {
				// "123" used as the LHS of an assignment statement
				std::string input = "123 = 456;";
				std::vector < std::vector<std::shared_ptr<Token>>> output = SimpleTokenizer::tokenize(input);
				assert(false);
			}
			catch (std::invalid_argument e) { assert(true); }

			try {
				// "0name" used as the LHS of an assignment statement
				std::string input = "0name = 456;";
				std::vector < std::vector<std::shared_ptr<Token>>> output = SimpleTokenizer::tokenize(input);
				assert(false);
			}
			catch (std::invalid_argument e) { assert(true); }

			try {
				// "@#" used 
				std::string input = "@# = 456;";
				std::vector < std::vector<std::shared_ptr<Token>>> output = SimpleTokenizer::tokenize(input);
				assert(false);
			}
			catch (std::invalid_argument e) { assert(true); }

			try {
				// invalid statement type detected
				std::string input = "1 == 2;";
				std::vector < std::vector<std::shared_ptr<Token>>> output = SimpleTokenizer::tokenize(input);
				assert(false);
			}
			catch (std::invalid_argument e) { assert(true); }

			// TODO: NEED TO REDO THIS WHEN DOING PROCEDURE AND IF WHILE
			try {
				// last character is not a semicolon. Invalid statement.
				std::string input = "x = 1; read y";
				std::vector < std::vector<std::shared_ptr<Token>>> output = SimpleTokenizer::tokenize(input);
				assert(false);
			}
			catch (std::invalid_argument e) { assert(true); }
		}

	};
}