#include "stdafx.h"
#include "CppUnitTest.h"

#include "../source/SP/SimpleTokens/Token.h"
#include "../source/SP/SimpleTokens/LiteralToken.h"
#include "../source/SP/SimpleTokens/IdentifierToken.h"
#include "../source/SP/SimpleTokens/OperatorToken.h"
#include "../source/SP/SimpleTokens/SeparatorToken.h"
#include "../source/SP/SimpleTokens/TokenFactory.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace string_view_literals;

bool combineAnd(vector<bool> results) {
	bool output = true;
	for (bool b : results) {
		output = output && b;
	}
	return output;
}
bool combineOr(vector<bool> results) {
	bool output = false;
	for (bool b : results) {
		output = output || b;
	}
	return output;
}

shared_ptr<Token>(*genToken) (string, bool) = TokenFactory::generateTokenForSimple;

template <typename T>
vector<bool> permutateForceIdentifier(string tokenName, T expected) {
	shared_ptr<Token> result_normal;
	shared_ptr<Token> result_inverted;
	try {
		result_normal = genToken(tokenName, true);
		result_inverted = genToken(tokenName, false);
	}
	catch (invalid_argument e) {
		return vector<bool>(false);
	}

	vector<bool> output;
	output.push_back(Token::checkEquality(result_normal, expected));
	output.push_back(Token::checkEquality(result_inverted, expected));
	return output;
}

namespace TokenFactory_Test
{
	TEST_CLASS(generateToken_successes) {
	public:
		TEST_METHOD(integerLiterals_singleDigit) {
			string tokenName = "0";
			shared_ptr<IntegerLiteralToken> expected = make_shared<IntegerLiteralToken>(tokenName);
			bool isEqual = combineAnd(permutateForceIdentifier(tokenName, expected));
			assert(isEqual == true); // success
		}

		TEST_METHOD(integerLiterals_multiDigit) {
			string tokenName = "123";
			shared_ptr<IntegerLiteralToken> expected = make_shared<IntegerLiteralToken>(tokenName);
			bool isEqual = combineAnd(permutateForceIdentifier(tokenName, expected));
			assert(isEqual == true); // success
		}

		// Arithmetic
		TEST_METHOD(arithmeticOperators_plus) {
			string tokenName = "+";
			shared_ptr<PlusOpToken> expected = make_shared<PlusOpToken>();
			bool isEqual = combineAnd(permutateForceIdentifier(tokenName, expected));
			assert(isEqual == true);
		}

		TEST_METHOD(arithmeticOperators_minus) {
			string tokenName = "-";
			shared_ptr<MinusOpToken> expected = make_shared<MinusOpToken>();
			bool isEqual = combineAnd(permutateForceIdentifier(tokenName, expected));
			assert(isEqual == true);
		}

		TEST_METHOD(arithmeticOperators_multiply) {
			string tokenName = "*";
			shared_ptr<MultiplyOpToken> expected = make_shared<MultiplyOpToken>();
			bool isEqual = combineAnd(permutateForceIdentifier(tokenName, expected));
			assert(isEqual == true);
		}

		TEST_METHOD(arithmeticOperators_divide) {
			string tokenName = "/";
			shared_ptr<DivideOpToken> expected = make_shared<DivideOpToken>();
			bool isEqual = combineAnd(permutateForceIdentifier(tokenName, expected));
			assert(isEqual == true);
		}

		TEST_METHOD(arithmeticOperators_modulo) {
			string tokenName = "%";
			shared_ptr<ModuloOpToken> expected = make_shared<ModuloOpToken>();
			bool isEqual = combineAnd(permutateForceIdentifier(tokenName, expected));
			assert(isEqual == true);
		}

		// Separators
		TEST_METHOD(separators_parenOpen) {
			string tokenName = "(";
			shared_ptr<ParenOpenSepToken> expected = make_shared<ParenOpenSepToken>();
			bool isEqual = combineAnd(permutateForceIdentifier(tokenName, expected));
			assert(isEqual == true);
		}

		TEST_METHOD(separators_parenClose) {
			string tokenName = ")";
			shared_ptr<ParenCloseSepToken> expected = make_shared<ParenCloseSepToken>();
			bool isEqual = combineAnd(permutateForceIdentifier(tokenName, expected));
			assert(isEqual == true);
		}

		TEST_METHOD(identifiers_standardName) {
			string tokenName = "length";
			shared_ptr<IdentifierToken> expected = make_shared<IdentifierToken>(tokenName);
			bool isEqual = combineAnd(permutateForceIdentifier(tokenName, expected));
			assert(isEqual == true);
		}

		TEST_METHOD(identifiers_longName) {
			string tokenName = "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz";
			shared_ptr<IdentifierToken> expected = make_shared<IdentifierToken>(tokenName);
			bool isEqual = combineAnd(permutateForceIdentifier(tokenName, expected));
			assert(isEqual == true);
		}

		TEST_METHOD(identifiers_alphanumericName) {
			string tokenName = "a1b2c3d4";
			shared_ptr<IdentifierToken> expected = make_shared<IdentifierToken>(tokenName);
			bool isEqual = combineAnd(permutateForceIdentifier(tokenName, expected));
			assert(isEqual == true);
		}

		TEST_METHOD(identifiers_forceIdentifier) {
			bool forceIdentifier = true;
			string tokenName = "read";
			shared_ptr<IdentifierToken> expected = make_shared<IdentifierToken>(tokenName);
			shared_ptr<Token> generated = genToken(tokenName, forceIdentifier);
			assert(Token::checkEquality(expected, generated));
		}
	};

	// Most of the failures come from invalid name being passed into TokenFactory::generateToken,
	// which will throw an invalid_argument.
	TEST_CLASS(generateToken_failures) {
	private:
		// There should never be an Token with "" as its tokenName so such a token is good for testing
		shared_ptr<Token> nonsense = make_shared<Token>("");
	public:

		TEST_METHOD(integerLiterals_negativeNumbers) {
			string tokenName = "-1";
			bool isEqual = combineOr(permutateForceIdentifier(tokenName, nonsense));
			assert(isEqual == false);
		}

		TEST_METHOD(unrecognisedCharacter_1) {
			string tokenName = "^";
			bool isEqual = combineOr(permutateForceIdentifier(tokenName, nonsense));
			assert(isEqual == false);
		}

		TEST_METHOD(unrecognisedCharacter_2) {
			string tokenName = "é";
			bool isEqual = combineOr(permutateForceIdentifier(tokenName, nonsense));
			assert(isEqual == false);
		}

		TEST_METHOD(unrecognisedCharacter_3) {
			string tokenName = " ";
			bool isEqual = combineOr(permutateForceIdentifier(tokenName, nonsense));
			assert(isEqual == false);
		}

		TEST_METHOD(invalidNameIdentifier_1) {
			string tokenName = "0digitInfront";
			bool isEqual = combineOr(permutateForceIdentifier(tokenName, nonsense));
			assert(isEqual == false);
		}

		TEST_METHOD(invalidNameIdentifier_2) {
			string tokenName = "_underscore";
			bool isEqual = combineOr(permutateForceIdentifier(tokenName, nonsense));
			assert(isEqual == false);
		}

		TEST_METHOD(invalidNameIdentifier_3) {
			string tokenName = ";invalidCharacter";
			bool isEqual = combineOr(permutateForceIdentifier(tokenName, nonsense));
			assert(isEqual == false);
		}

		TEST_METHOD(emptyToken) {
			string tokenName = "";
			bool isEqual = combineOr(permutateForceIdentifier(tokenName, nonsense));
			assert(isEqual == false);
		}
	};
}