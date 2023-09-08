#include "stdafx.h"
#include "CppUnitTest.h"

#include "../source/TokenizerClasses/SimpleTokenizer.h"
#include "../source/TokenizerClasses/TokenFactory.h"
#include "../source/AST/ASTBuilder.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std::string_view_literals;

namespace UnitTesting
{
	TEST_CLASS(Test_TokenFactory_generateToken) {
	private:
		// Some variables to make the code cleaner in the test methods
		std::string tokenName = "";
		bool forSimple = false;
		bool forceIdentifier = false;
		std::shared_ptr<Token> (*genToken) (std::string, bool, bool) = TokenFactory::generateToken;

		// Tests all combinations of forSimple and forceIdentifier with a given tokenName, and an expected value of T.
		template <typename T>
		auto testAllCombinations(std::string tokenName, T expected) {
			std::shared_ptr<Token> result_simple_nonIden = genToken(tokenName, forSimple, forceIdentifier);
			std::shared_ptr<Token> result_pql_nonIden = genToken(tokenName, !forSimple, forceIdentifier);
			std::shared_ptr<Token> result_simple_iden = genToken(tokenName, forSimple, !forceIdentifier);
			std::shared_ptr<Token> result_pql_iden = genToken(tokenName, !forSimple, !forceIdentifier);

			bool isEqual = Token::checkEquality(result_simple_nonIden, expected);
			isEqual = isEqual && Token::checkEquality(result_pql_nonIden, expected);
			isEqual = isEqual && Token::checkEquality(result_simple_iden, expected);
			isEqual = isEqual && Token::checkEquality(result_pql_iden, expected);
			return isEqual;
		};

	public:
		TEST_METHOD(integerLiterals_success) {
			tokenName = "0";
			std::shared_ptr<IntegerLiteralToken> expected = std::make_shared<IntegerLiteralToken>(tokenName);
			bool isEqual = testAllCombinations(tokenName, expected);
			assert(isEqual == true); // success

			tokenName = "123";
			bool isEqual = testAllCombinations(tokenName, expected);
			assert(isEqual = true); // success
		}

		TEST_METHOD(underscore_success) {
			tokenName = "_";
			std::shared_ptr<UnderscoreToken> expected = std::make_shared<UnderscoreToken>(tokenName);
			bool isEqual = testAllCombinations(tokenName, expected);
			assert(isEqual = true);
		}

		TEST_METHOD(arithmeticOperators_success) {
			// Arithmetic
			tokenName = "+";
			std::shared_ptr<PlusOpToken> expected = std::make_shared<PlusOpToken>(tokenName);
			bool isEqual = testAllCombinations(tokenName, expected);
			assert(isEqual = true);

			tokenName = "-";
			std::shared_ptr<MinusOpToken> expected = std::make_shared<MinusOpToken>(tokenName);
			bool isEqual = testAllCombinations(tokenName, expected);
			assert(isEqual = true);

			tokenName = "*";
			std::shared_ptr<MultiplyOpToken> expected = std::make_shared<MultiplyOpToken>(tokenName);
			bool isEqual = testAllCombinations(tokenName, expected);
			assert(isEqual = true);

			tokenName = "/";
			std::shared_ptr<DivideOpToken> expected = std::make_shared<DivideOpToken>(tokenName);
			bool isEqual = testAllCombinations(tokenName, expected);
			assert(isEqual = true);

			tokenName = "%";
			std::shared_ptr<ModuloOpToken> expected = std::make_shared<ModuloOpToken>(tokenName);
			bool isEqual = testAllCombinations(tokenName, expected);
			assert(isEqual = true);
		}

		TEST_METHOD(relationalOperators_success) {
			// Relational
			tokenName = "==";
			std::shared_ptr<EqualityOpToken> expected = std::make_shared<EqualityOpToken>(tokenName);
			bool isEqual = testAllCombinations(tokenName, expected);
			assert(isEqual = true);

			tokenName = "!=";
			std::shared_ptr<InequalityOpToken> expected = std::make_shared<InequalityOpToken>(tokenName);
			bool isEqual = testAllCombinations(tokenName, expected);
			assert(isEqual = true);

			tokenName = "<=";
			std::shared_ptr<LessThanEqualOpToken> expected = std::make_shared<LessThanEqualOpToken>(tokenName);
			bool isEqual = testAllCombinations(tokenName, expected);
			assert(isEqual = true);

			tokenName = "<";
			std::shared_ptr<LessThanOpToken> expected = std::make_shared<LessThanOpToken>(tokenName);
			bool isEqual = testAllCombinations(tokenName, expected);
			assert(isEqual = true);

			tokenName = ">=";
			std::shared_ptr<MoreThanEqualOpToken> expected = std::make_shared<MoreThanEqualOpToken>(tokenName);
			bool isEqual = testAllCombinations(tokenName, expected);
			assert(isEqual = true);

			tokenName = ">";
			std::shared_ptr<MoreThanOpToken> expected = std::make_shared<MoreThanOpToken>(tokenName);
			bool isEqual = testAllCombinations(tokenName, expected);
			assert(isEqual = true);
		}

		TEST_METHOD(booleanOperators_success) {
			// Boolean
			tokenName = "!";
			std::shared_ptr<NotOpToken> expected = std::make_shared<NotOpToken>(tokenName);
			bool isEqual = testAllCombinations(tokenName, expected);
			assert(isEqual = true);

			tokenName = "||";
			std::shared_ptr<OrOpToken> expected = std::make_shared<OrOpToken>(tokenName);
			bool isEqual = testAllCombinations(tokenName, expected);
			assert(isEqual = true);

			tokenName = "&&";
			std::shared_ptr<AndOpToken> expected = std::make_shared<AndOpToken>(tokenName);
			bool isEqual = testAllCombinations(tokenName, expected);
			assert(isEqual = true);
		}

		TEST_METHOD(otherOperators_success) {
			// Equals
			tokenName = "=";
			std::shared_ptr<EqualsOpToken> expected = std::make_shared<EqualsOpToken>(tokenName);
			bool isEqual = testAllCombinations(tokenName, expected);
			assert(isEqual = true);
		}

		TEST_METHOD(separators_success) {
			tokenName = "(";
			std::shared_ptr<ParenOpenSepToken> expected = std::make_shared<ParenOpenSepToken>(tokenName);
			bool isEqual = testAllCombinations(tokenName, expected);
			assert(isEqual = true);

			tokenName = "}";
			std::shared_ptr<CurlyCloseSepToken> expected = std::make_shared<CurlyCloseSepToken>(tokenName);
			bool isEqual = testAllCombinations(tokenName, expected);
			assert(isEqual = true);

			tokenName = ";";
			std::shared_ptr<SemicolonSepToken> expected = std::make_shared<SemicolonSepToken>(tokenName);
			bool isEqual = testAllCombinations(tokenName, expected);
			assert(isEqual = true);

		}

		TEST_METHOD(simpleKeywords_success) {

		}

		TEST_METHOD(pqlKeywords_success) {

		}

		TEST_METHOD(commonKeywords_success) {

		}

		TEST_METHOD(forceIdentifier_sucess) {

		}
	};
}