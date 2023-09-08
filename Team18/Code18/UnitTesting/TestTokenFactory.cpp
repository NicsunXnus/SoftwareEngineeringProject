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
		std::string tokenName = "";
		bool forSimple = "";
		bool forceIdentifier = "";
		std::shared_ptr<Token> (*genToken) (std::string, bool, bool) = TokenFactory::generateToken;

	public:
		TEST_METHOD(CHANGE_THIS_SUCCESS) {
			tokenName = "0";
			forSimple = "CHANGE_THIS";
			forceIdentifier = "CHANGE_THIS";
			//std::shared_ptr<Token> result = genToken(tokenName, forSimple, forceIdentifier);
			std::shared_ptr<IntegerLiteralToken> result = std::make_shared<IntegerLiteralToken>("1");
			std::shared_ptr<IntegerLiteralToken> expected = std::make_shared<IntegerLiteralToken>(tokenName);
			bool isEqual = Token::checkEquality(result, expected);
			assert(isEqual == true); // success
		}
	};
}