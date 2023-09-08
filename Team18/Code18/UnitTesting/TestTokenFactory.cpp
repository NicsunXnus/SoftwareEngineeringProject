#include "stdafx.h"
#include "CppUnitTest.h"

#include "../source/TokenizerClasses/SimpleTokenizer.h"
#include "../source/TokenizerClasses/TokenFactory.h"
#include "../source/AST/ASTBuilder.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std::string_view_literals;

namespace UnitTesting
{
	TEST_CLASS(Test_TokenFactory_isValidName) {
	public:
		TEST_METHOD(lettersOnly_success) {
			bool result = isValidName("AbCd");
			assert(result == true); // success
		}

		TEST_METHOD(alphanumeric_success) {
			bool result = isValidName("a1Bc23D4");
			result = result && isValidName("a41qg650l");
			assert(result == true); // success
		}

		TEST_METHOD(keyword_success) {
			bool result = isValidName("read");
			result = result && isValidName("procedure");
			assert(result == true); // success
		}

		TEST_METHOD(allDigits_failure) {
			bool result = isValidName("0123");
			result = result || isValidName("9");
			result = result || isValidName("10");
			result = result || isValidName("0x1");
			result = result || isValidName("0b1");
			assert(result == false); // failure
		}

		TEST_METHOD(leadingDigit_failure) {
			bool result = isValidName("0read");
			result = result || isValidName("9read");
			result = result || isValidName("10read");
			result = result || isValidName("0x1read");
			result = result || isValidName("0b1read");
			assert(result == false); // failure
		}

		TEST_METHOD(invalidCharacter_failure) {
			bool result = isValidName("b_read"); // invalid character: _
			result = result || isValidName("em@il"); // invalid character: @
			result = result || isValidName("one+two"); // invalid character: +
			result = result || isValidName("wrong*format"); // invalid character: *
			result = result || isValidName("true||false"); // invalid character: |
			assert(result == false); // failure
		}

	};

	TEST_CLASS(Test_TokenFactory_generateToken) {
	private:
		// Some variables to make the code cleaner in the test methods
		std::string tokenName = "";
		bool forSimple = NULL;
		bool forceIdentifier = NULL;
		std::shared_ptr<Token> (*genToken) (std::string, bool, bool) = TokenFactory::generateToken;

	public:
		TEST_METHOD(integerLiterals_success) {
			// lambda function that tests for all combinations of forSimple and forceIdentifier
			auto miniWrapper = [&](std::string tokenName) {
				std::shared_ptr<Token> result_simple_nonIden = genToken(tokenName, forSimple, forceIdentifier);
				std::shared_ptr<Token> result_pql_nonIden = genToken(tokenName, !forSimple, forceIdentifier);
				std::shared_ptr<Token> result_simple_iden = genToken(tokenName, forSimple, !forceIdentifier);
				std::shared_ptr<Token> result_pql_iden = genToken(tokenName, !forSimple, !forceIdentifier);

				std::shared_ptr<IntegerLiteralToken> expected = std::make_shared<IntegerLiteralToken>(tokenName);
				bool isEqual = Token::checkEquality(result_simple_nonIden, expected);
				isEqual = isEqual && Token::checkEquality(result_pql_nonIden, expected);
				isEqual = isEqual && Token::checkEquality(result_simple_iden, expected);
				isEqual = isEqual && Token::checkEquality(result_pql_iden, expected);
				return isEqual;
			};
			tokenName = "0";
			forSimple = false;
			forceIdentifier = false;
			bool isEqual = miniWrapper(tokenName);
			assert(isEqual == true); // success

			tokenName = "123";
			bool isEqual = miniWrapper(tokenName);
			assert(isEqual = true);
		}
		TEST_METHOD(underscore_success) {

		}
		TEST_METHOD(operators_success) {

		}
		TEST_METHOD(separators_success) {

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