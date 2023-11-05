#include "stdafx.h"
#include "CppUnitTest.h"

#include "../source/SP/SimpleTokens/Token.h"
#include "../source/SP/SimpleTokens/LiteralToken.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

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
			shared_ptr<Token> token = make_shared<Token>("123");
			assert(Token::checkEquality(token, token) == true);
		}
		TEST_METHOD(diffObject_success) {
			shared_ptr<Token> token1 = make_shared<Token>("123");
			shared_ptr<Token> token2 = make_shared<Token>("123");
			assert(Token::checkEquality(token1, token2) == true);
		}
		TEST_METHOD(sameClass_failure) {
			shared_ptr<Token> token1 = make_shared<Token>("abc");
			shared_ptr<Token> token2 = make_shared<Token>("def");
			assert(Token::checkEquality(token1, token2) == false);
		}
		TEST_METHOD(sameTokenName_failure) {
			shared_ptr<IntegerLiteralToken> token1 = make_shared<IntegerLiteralToken>("abc");
			shared_ptr<Token> token2 = make_shared<Token>("abc");
			assert(Token::checkEquality(token1, token2) == false);
		}
		TEST_METHOD(everythingDifferent_failure) {
			shared_ptr<IntegerLiteralToken> token1 = make_shared<IntegerLiteralToken>("abc");
			shared_ptr<Token> token2 = make_shared<Token>("bcd");
			assert(Token::checkEquality(token1, token2) == false);
		}
	};
}