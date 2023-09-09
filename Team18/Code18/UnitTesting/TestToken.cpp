#include "stdafx.h"
#include "CppUnitTest.h"

#include "../source/TokenClasses/Token.h"
#include "../source/TokenClasses/LiteralToken.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std::string_view_literals;

namespace Token_Test
{
	TEST_CLASS(getName_Test) {
	public:
		TEST_METHOD(parentClass_success) {
			Token token = Token("abc");
			assert(token.getName() == "abc");
		}
		TEST_METHOD(childClass_success) {
			// Okay since there is no input validation for these constructors
			LiteralToken token = LiteralToken("abc"); 
			assert(token.getName() == "abc");
		}
		TEST_METHOD(grandchildClass_success) {
			// Okay since there is no input validation for these constructors
			IntegerLiteralToken token = IntegerLiteralToken("abc");
			assert(token.getName() == "abc");
		}
		TEST_METHOD(upcasting_success) {
			// Okay since there is no input validation for these constructors
			LiteralToken tokenLiteral = IntegerLiteralToken("abc");
			assert(tokenLiteral.getName() == "abc");

			// Okay since there is no input validation for these constructors
			Token tokenParent = LiteralToken("abc");
			assert(tokenParent.getName() == "abc");

			// Okay since there is no input validation for these constructors
			Token tokenGrandparent = IntegerLiteralToken("abc");
			assert(tokenGrandparent.getName() == "abc");
		}
	};

	TEST_CLASS(equality_Test) {
	public:
		TEST_METHOD(equalityOp) {
			Token token1 = Token("123");
			Token token2 = Token("123");
			assert(token1 == token2);
		}
		TEST_METHOD(sameObject_success) {
			Token token = Token("123");
			assert(Token::checkEquality(&token, &token) == true);
		}
		TEST_METHOD(diffObject_success) {
			Token token1 = Token("123");
			Token token2 = Token("123");
			assert(Token::checkEquality(&token1, &token2) == true);
		}
		TEST_METHOD(sameClass_failure) {
			Token token1 = Token("abc");
			Token token2 = Token("bcd");
			assert(Token::checkEquality(&token1, &token2) == false);
		}
		TEST_METHOD(sameTokenName_failure) {
			IntegerLiteralToken token1 = IntegerLiteralToken("abc");
			Token token2 = Token("abc");
			assert(Token::checkEquality(&token1, &token2) == false);
		}
		TEST_METHOD(everythingDifferent_failure) {
			IntegerLiteralToken token1 = IntegerLiteralToken("abc");
			Token token2 = Token("bcd");
			assert(Token::checkEquality(&token1, &token2) == false);
		}
	};
}