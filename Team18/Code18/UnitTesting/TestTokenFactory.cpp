#include "stdafx.h"
#include "CppUnitTest.h"

#include "../source/SimpleTokens/TokenFactory.h"

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
	std::shared_ptr<Token> result_normal = genToken(tokenName, true);
	std::shared_ptr<Token> result_inverted = genToken(tokenName, false);

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

		// Relational
		TEST_METHOD(relationalOperators_equality) {
			std::string tokenName = "==";
			std::shared_ptr<EqualityOpToken> expected = std::make_shared<EqualityOpToken>();
			bool isEqual = combineAnd(permutateForceIdentifier(tokenName, expected));
			assert(isEqual == true);
		}
		
		TEST_METHOD(relationalOperators_inequality) {
			std::string tokenName = "!=";
			std::shared_ptr<InequalityOpToken> expected = std::make_shared<InequalityOpToken>();
			bool isEqual = combineAnd(permutateForceIdentifier(tokenName, expected));
			assert(isEqual == true);
		}

		TEST_METHOD(relationalOperators_lessThanEqual) {
			std::string tokenName = "<=";
			std::shared_ptr<LessThanEqualOpToken> expected = std::make_shared<LessThanEqualOpToken>();
			bool isEqual = combineAnd(permutateForceIdentifier(tokenName, expected));
			assert(isEqual == true);
		}

		TEST_METHOD(relationalOperators_lessThan) {
			std::string tokenName = "<";
			std::shared_ptr<LessThanOpToken> expected = std::make_shared<LessThanOpToken>();
			bool isEqual = combineAnd(permutateForceIdentifier(tokenName, expected));
			assert(isEqual == true);
		}

		TEST_METHOD(relationalOperators_MoreThanEqual) {
			std::string tokenName = ">=";
			std::shared_ptr<MoreThanEqualOpToken> expected = std::make_shared<MoreThanEqualOpToken>();
			bool isEqual = combineAnd(permutateForceIdentifier(tokenName, expected));
			assert(isEqual == true);
		}

		TEST_METHOD(relationalOperators_MoreThan) {
			std::string tokenName = ">";
			std::shared_ptr<MoreThanOpToken> expected = std::make_shared<MoreThanOpToken>();
			bool isEqual = combineAnd(permutateForceIdentifier(tokenName, expected));
			assert(isEqual == true);
		}

		// Boolean
		TEST_METHOD(booleanOperators_not) {
			std::string tokenName = "!";
			std::shared_ptr<NotOpToken> expected = std::make_shared<NotOpToken>();
			bool isEqual = combineAnd(permutateForceIdentifier(tokenName, expected));
			assert(isEqual == true);
		}

		TEST_METHOD(booleanOperators_or) {
			std::string tokenName = "||";
			std::shared_ptr<OrOpToken> expected = std::make_shared<OrOpToken>();
			bool isEqual = combineAnd(permutateForceIdentifier(tokenName, expected));
			assert(isEqual == true);
		}

		TEST_METHOD(booleanOperators_and) {
			std::string tokenName = "&&";
			std::shared_ptr<AndOpToken> expected = std::make_shared<AndOpToken>();
			bool isEqual = combineAnd(permutateForceIdentifier(tokenName, expected));
			assert(isEqual == true);
		}

		TEST_METHOD(otherOperators_equals) {
			// Equals
			std::string tokenName = "=";
			std::shared_ptr<EqualsOpToken> expected = std::make_shared<EqualsOpToken>();
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
		TEST_METHOD(commonKeywords_read) {
			bool forceIdentifier = false;
			std::string tokenName = "read";
			std::shared_ptr<ReadKeywordToken> expected = std::make_shared<ReadKeywordToken>();
			std::shared_ptr<Token> generated = genToken(tokenName, forceIdentifier);
			assert(Token::checkEquality(expected, generated));
		}

		TEST_METHOD(commonKeywords_print) {
			bool forceIdentifier = false;
			std::string tokenName = "print";
			std::shared_ptr<PrintKeywordToken> expected = std::make_shared<PrintKeywordToken>();
			std::shared_ptr<Token> generated = genToken(tokenName, forceIdentifier);
			assert(Token::checkEquality(expected, generated));
		}

		TEST_METHOD(commonKeywords_call) {
			bool forceIdentifier = false;
			std::string tokenName = "call";
			std::shared_ptr<CallKeywordToken> expected = std::make_shared<CallKeywordToken>();
			std::shared_ptr<Token> generated = genToken(tokenName, forceIdentifier);
			assert(Token::checkEquality(expected, generated));
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
		// There should never be an Identifier with "" as its tokenName so such a token is good for testing
		std::shared_ptr<IdentifierToken> nonsense = std::make_shared<IdentifierToken>("");
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