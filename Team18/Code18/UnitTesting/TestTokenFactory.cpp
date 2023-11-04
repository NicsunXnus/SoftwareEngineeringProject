#include "stdafx.h"
#include "CppUnitTest.h"

#include "../source/SP/SimpleTokens/Token.h"
#include "../source/SP/SimpleTokens/LiteralToken.h"
#include "../source/SP/SimpleTokens/IdentifierToken.h"
#include "../source/SP/SimpleTokens/OperatorToken.h"
#include "../source/SP/SimpleTokens/SeparatorToken.h"
#include "../source/SP/SimpleTokens/TokenFactory.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std::string_view_literals;

bool combineAnd(std::vector<bool> results) {
	bool output = true;
	for (bool b : results) {
		output = output && b;
	}
	return output;
}
bool combineOr(std::vector<bool> results) {
	bool output = false;
	for (bool b : results) {
		output = output || b;
	}
	return output;
}

std::shared_ptr<Token>(*genToken) (std::string, bool) = TokenFactory::generateTokenForSimple;

template <typename T>
std::vector<bool> permutateForceIdentifier(std::string tokenName, T expected) {
	std::shared_ptr<Token> result_normal;
	std::shared_ptr<Token> result_inverted;
	try {
		result_normal = genToken(tokenName, true);
		result_inverted = genToken(tokenName, false);
	}
	catch (std::invalid_argument e) {
		return std::vector<bool>(false);
	}

	std::vector<bool> output;
	output.push_back(Token::checkEquality(result_normal, expected));
	output.push_back(Token::checkEquality(result_inverted, expected));
	return output;
}

namespace TokenFactory_Test
{
	TEST_CLASS(generateToken_successes) {
	public:
		TEST_METHOD(integerLiterals_singleDigit) {
			std::string tokenName = "0";
			std::shared_ptr<IntegerLiteralToken> expected = std::make_shared<IntegerLiteralToken>(tokenName);
			bool isEqual = combineAnd(permutateForceIdentifier(tokenName, expected));
			assert(isEqual == true); // success
		}

		TEST_METHOD(integerLiterals_multiDigit) {
			std::string tokenName = "123";
			std::shared_ptr<IntegerLiteralToken> expected = std::make_shared<IntegerLiteralToken>(tokenName);
			bool isEqual = combineAnd(permutateForceIdentifier(tokenName, expected));
			assert(isEqual == true); // success
		}

		// Arithmetic
		TEST_METHOD(arithmeticOperators_plus) {
			std::string tokenName = "+";
			std::shared_ptr<PlusOpToken> expected = std::make_shared<PlusOpToken>();
			bool isEqual = combineAnd(permutateForceIdentifier(tokenName, expected));
			assert(isEqual == true);
		}

		TEST_METHOD(arithmeticOperators_minus) {
			std::string tokenName = "-";
			std::shared_ptr<MinusOpToken> expected = std::make_shared<MinusOpToken>();
			bool isEqual = combineAnd(permutateForceIdentifier(tokenName, expected));
			assert(isEqual == true);
		}

		TEST_METHOD(arithmeticOperators_multiply) {
			std::string tokenName = "*";
			std::shared_ptr<MultiplyOpToken> expected = std::make_shared<MultiplyOpToken>();
			bool isEqual = combineAnd(permutateForceIdentifier(tokenName, expected));
			assert(isEqual == true);
		}

		TEST_METHOD(arithmeticOperators_divide) {
			std::string tokenName = "/";
			std::shared_ptr<DivideOpToken> expected = std::make_shared<DivideOpToken>();
			bool isEqual = combineAnd(permutateForceIdentifier(tokenName, expected));
			assert(isEqual == true);
		}

		TEST_METHOD(arithmeticOperators_modulo) {
			std::string tokenName = "%";
			std::shared_ptr<ModuloOpToken> expected = std::make_shared<ModuloOpToken>();
			bool isEqual = combineAnd(permutateForceIdentifier(tokenName, expected));
			assert(isEqual == true);
		}

		// Separators
		TEST_METHOD(separators_parenOpen) {
			std::string tokenName = "(";
			std::shared_ptr<ParenOpenSepToken> expected = std::make_shared<ParenOpenSepToken>();
			bool isEqual = combineAnd(permutateForceIdentifier(tokenName, expected));
			assert(isEqual == true);
		}

		TEST_METHOD(separators_parenClose) {
			std::string tokenName = ")";
			std::shared_ptr<ParenCloseSepToken> expected = std::make_shared<ParenCloseSepToken>();
			bool isEqual = combineAnd(permutateForceIdentifier(tokenName, expected));
			assert(isEqual == true);
		}

		TEST_METHOD(identifiers_standardName) {
			std::string tokenName = "length";
			std::shared_ptr<IdentifierToken> expected = std::make_shared<IdentifierToken>(tokenName);
			bool isEqual = combineAnd(permutateForceIdentifier(tokenName, expected));
			assert(isEqual == true);
		}

		TEST_METHOD(identifiers_longName) {
			std::string tokenName = "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz";
			std::shared_ptr<IdentifierToken> expected = std::make_shared<IdentifierToken>(tokenName);
			bool isEqual = combineAnd(permutateForceIdentifier(tokenName, expected));
			assert(isEqual == true);
		}

		TEST_METHOD(identifiers_alphanumericName) {
			std::string tokenName = "a1b2c3d4";
			std::shared_ptr<IdentifierToken> expected = std::make_shared<IdentifierToken>(tokenName);
			bool isEqual = combineAnd(permutateForceIdentifier(tokenName, expected));
			assert(isEqual == true);
		}

		TEST_METHOD(identifiers_forceIdentifier) {
			bool forceIdentifier = true;
			std::string tokenName = "read";
			std::shared_ptr<IdentifierToken> expected = std::make_shared<IdentifierToken>(tokenName);
			std::shared_ptr<Token> generated = genToken(tokenName, forceIdentifier);
			assert(Token::checkEquality(expected, generated));
		}
	};

	// Most of the failures come from invalid name being passed into TokenFactory::generateToken,
	// which will throw an std::invalid_argument.
	TEST_CLASS(generateToken_failures) {
	private:
		// There should never be an Token with "" as its tokenName so such a token is good for testing
		std::shared_ptr<Token> nonsense = std::make_shared<Token>("");
	public:

		TEST_METHOD(integerLiterals_negativeNumbers) {
			std::string tokenName = "-1";
			bool isEqual = combineOr(permutateForceIdentifier(tokenName, nonsense));
			assert(isEqual == false);
		}

		TEST_METHOD(unrecognisedCharacter_1) {
			std::string tokenName = "^";
			bool isEqual = combineOr(permutateForceIdentifier(tokenName, nonsense));
			assert(isEqual == false);
		}

		TEST_METHOD(unrecognisedCharacter_2) {
			std::string tokenName = "é";
			bool isEqual = combineOr(permutateForceIdentifier(tokenName, nonsense));
			assert(isEqual == false);
		}

		TEST_METHOD(unrecognisedCharacter_3) {
			std::string tokenName = " ";
			bool isEqual = combineOr(permutateForceIdentifier(tokenName, nonsense));
			assert(isEqual == false);
		}

		TEST_METHOD(invalidNameIdentifier_1) {
			std::string tokenName = "0digitInfront";
			bool isEqual = combineOr(permutateForceIdentifier(tokenName, nonsense));
			assert(isEqual == false);
		}

		TEST_METHOD(invalidNameIdentifier_2) {
			std::string tokenName = "_underscore";
			bool isEqual = combineOr(permutateForceIdentifier(tokenName, nonsense));
			assert(isEqual == false);
		}

		TEST_METHOD(invalidNameIdentifier_3) {
			std::string tokenName = ";invalidCharacter";
			bool isEqual = combineOr(permutateForceIdentifier(tokenName, nonsense));
			assert(isEqual == false);
		}

		TEST_METHOD(emptyToken) {
			std::string tokenName = "";
			bool isEqual = combineOr(permutateForceIdentifier(tokenName, nonsense));
			assert(isEqual == false);
		}
	};
}