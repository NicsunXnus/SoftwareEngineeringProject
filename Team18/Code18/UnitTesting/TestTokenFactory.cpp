#include "stdafx.h"
#include "CppUnitTest.h"

#include "../source/TokenizerClasses/TokenFactory.h"

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

std::shared_ptr<Token>(*genToken) (std::string, bool, bool) = TokenFactory::generateToken;

template <typename T>
std::vector<bool> allCombinations(std::string tokenName, T expected) {
	std::shared_ptr<Token> result_simple_nonIden = genToken(tokenName, true, false);
	std::shared_ptr<Token> result_pql_nonIden = genToken(tokenName, false, false);
	std::shared_ptr<Token> result_simple_iden = genToken(tokenName, true, true);
	std::shared_ptr<Token> result_pql_iden = genToken(tokenName, false, true);

	std::vector<bool> output;
	output.push_back(Token::checkEquality(result_simple_nonIden, expected));
	output.push_back(Token::checkEquality(result_pql_nonIden, expected));
	output.push_back(Token::checkEquality(result_simple_iden, expected));
	output.push_back(Token::checkEquality(result_pql_iden, expected));
	return output;
};

template <typename T>
std::vector<bool> permutateForceIdentifier(std::string tokenName, bool forSimple, T expected) {
	std::shared_ptr<Token> result_normal = genToken(tokenName, forSimple, true);
	std::shared_ptr<Token> result_inverted = genToken(tokenName, forSimple, false);

	std::vector<bool> output;
	output.push_back(Token::checkEquality(result_normal, expected));
	output.push_back(Token::checkEquality(result_inverted, expected));
	return output;
}

template <typename T>
std::vector<bool> permutateLanguage(std::string tokenName, bool forceIdentifier, T expected) {
	std::shared_ptr<Token> result_normal = genToken(tokenName, true, forceIdentifier);
	std::shared_ptr<Token> result_inverted = genToken(tokenName, false, forceIdentifier);

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
			bool isEqual = combineAnd(allCombinations(tokenName, expected));
			assert(isEqual == true); // success
		}

		TEST_METHOD(integerLiterals_multiDigit) {
			std::string tokenName = "123";
			std::shared_ptr<IntegerLiteralToken> expected = std::make_shared<IntegerLiteralToken>(tokenName);
			bool isEqual = combineAnd(allCombinations(tokenName, expected));
			assert(isEqual == true); // success
		}

		TEST_METHOD(underscore) {
			std::string tokenName = "_";
			std::shared_ptr<UnderscoreToken> expected = std::make_shared<UnderscoreToken>();
			bool isEqual = combineAnd(permutateForceIdentifier(tokenName, false, expected));
			assert(isEqual == true);
		}

		// Arithmetic
		TEST_METHOD(arithmeticOperators_plus) {
			std::string tokenName = "+";
			std::shared_ptr<PlusOpToken> expected = std::make_shared<PlusOpToken>();
			bool isEqual = combineAnd(allCombinations(tokenName, expected));
			assert(isEqual == true);
		}

		TEST_METHOD(arithmeticOperators_minus) {
			std::string tokenName = "-";
			std::shared_ptr<MinusOpToken> expected = std::make_shared<MinusOpToken>();
			bool isEqual = combineAnd(allCombinations(tokenName, expected));
			assert(isEqual == true);
		}

		TEST_METHOD(arithmeticOperators_multiply) {
			std::string tokenName = "*";
			std::shared_ptr<MultiplyOpToken> expected = std::make_shared<MultiplyOpToken>();
			bool isEqual = combineAnd(allCombinations(tokenName, expected));
			assert(isEqual == true);
		}

		TEST_METHOD(arithmeticOperators_divide) {
			std::string tokenName = "/";
			std::shared_ptr<DivideOpToken> expected = std::make_shared<DivideOpToken>();
			bool isEqual = combineAnd(allCombinations(tokenName, expected));
			assert(isEqual == true);
		}

		TEST_METHOD(arithmeticOperators_modulo) {
			std::string tokenName = "%";
			std::shared_ptr<ModuloOpToken> expected = std::make_shared<ModuloOpToken>();
			bool isEqual = combineAnd(allCombinations(tokenName, expected));
			assert(isEqual == true);
		}

		// Relational
		TEST_METHOD(relationalOperators_equality) {
			std::string tokenName = "==";
			bool forSimple = true;
			std::shared_ptr<EqualityOpToken> expected = std::make_shared<EqualityOpToken>();
			bool isEqual = combineAnd(permutateForceIdentifier(tokenName, true, expected));
			assert(isEqual == true);
		}
		
		TEST_METHOD(relationalOperators_inequality) {
			std::string tokenName = "!=";
			bool forSimple = true;
			std::shared_ptr<InequalityOpToken> expected = std::make_shared<InequalityOpToken>();
			bool isEqual = combineAnd(permutateForceIdentifier(tokenName, true, expected));
			assert(isEqual == true);
		}

		TEST_METHOD(relationalOperators_lessThanEqual) {
			std::string tokenName = "<=";
			bool forSimple = true;
			std::shared_ptr<LessThanEqualOpToken> expected = std::make_shared<LessThanEqualOpToken>();
			bool isEqual = combineAnd(permutateForceIdentifier(tokenName, true, expected));
			assert(isEqual == true);
		}

		TEST_METHOD(relationalOperators_lessThan) {
			std::string tokenName = "<";
			bool forSimple = true;
			std::shared_ptr<LessThanOpToken> expected = std::make_shared<LessThanOpToken>();
			bool isEqual = combineAnd(permutateForceIdentifier(tokenName, true, expected));
			assert(isEqual == true);
		}

		TEST_METHOD(relationalOperators_MoreThanEqual) {
			std::string tokenName = ">=";
			bool forSimple = true;
			std::shared_ptr<MoreThanEqualOpToken> expected = std::make_shared<MoreThanEqualOpToken>();
			bool isEqual = combineAnd(permutateForceIdentifier(tokenName, true, expected));
			assert(isEqual == true);
		}

		TEST_METHOD(relationalOperators_MoreThan) {
			std::string tokenName = ">";
			bool forSimple = true;
			std::shared_ptr<MoreThanOpToken> expected = std::make_shared<MoreThanOpToken>();
			bool isEqual = combineAnd(permutateForceIdentifier(tokenName, true, expected));
			assert(isEqual == true);
		}

		// Boolean
		TEST_METHOD(booleanOperators_not) {
			std::string tokenName = "!";
			bool forSimple = true;
			std::shared_ptr<NotOpToken> expected = std::make_shared<NotOpToken>();
			bool isEqual = combineAnd(permutateForceIdentifier(tokenName, true, expected));
			assert(isEqual == true);
		}

		TEST_METHOD(booleanOperators_or) {
			std::string tokenName = "||";
			bool forSimple = true;
			std::shared_ptr<OrOpToken> expected = std::make_shared<OrOpToken>();
			bool isEqual = combineAnd(permutateForceIdentifier(tokenName, true, expected));
			assert(isEqual == true);
		}

		TEST_METHOD(booleanOperators_and) {
			std::string tokenName = "&&";
			bool forSimple = true;
			std::shared_ptr<AndOpToken> expected = std::make_shared<AndOpToken>();
			bool isEqual = combineAnd(permutateForceIdentifier(tokenName, true, expected));
			assert(isEqual == true);
		}

		TEST_METHOD(otherOperators_equals) {
			// Equals
			std::string tokenName = "=";
			bool forSimple = true;
			std::shared_ptr<EqualsOpToken> expected = std::make_shared<EqualsOpToken>();
			bool isEqual = combineAnd(permutateForceIdentifier(tokenName, true, expected));
			assert(isEqual == true);
		}

		// Separators
		TEST_METHOD(separators_parenOpen) {
			std::string tokenName = "(";
			std::shared_ptr<ParenOpenSepToken> expected = std::make_shared<ParenOpenSepToken>();
			bool isEqual = combineAnd(allCombinations(tokenName, expected));
			assert(isEqual == true);
		}

		TEST_METHOD(separators_parenClose) {
			std::string tokenName = ")";
			std::shared_ptr<ParenCloseSepToken> expected = std::make_shared<ParenCloseSepToken>();
			bool isEqual = combineAnd(allCombinations(tokenName, expected));
			assert(isEqual == true);
		}

		TEST_METHOD(separators_semicolon) {
			std::string tokenName = ";";
			std::shared_ptr<SemicolonSepToken> expected = std::make_shared<SemicolonSepToken>();
			bool isEqual = combineAnd(allCombinations(tokenName, expected));
			assert(isEqual == true);
		}

		TEST_METHOD(separators_doubleQuotation) {
			std::string tokenName = "\"";
			std::shared_ptr<DoubleQuoSepToken> expected = std::make_shared<DoubleQuoSepToken>();
			bool isEqual = combineAnd(permutateForceIdentifier(tokenName, false, expected));
			assert(isEqual == true);
		}

		TEST_METHOD(separators_comma) {
			std::string tokenName = ",";
			std::shared_ptr<CommaSepToken> expected = std::make_shared<CommaSepToken>();
			bool isEqual = combineAnd(permutateForceIdentifier(tokenName, false, expected));
			assert(isEqual == true);
		}

		// SIMPLE KEYWORDS
		TEST_METHOD(simpleKeywords_then) {
			bool forSimple = true;
			bool forceIdentifier = false;
			std::string tokenName = "then";
			std::shared_ptr<Token> result = genToken(tokenName, forSimple, forceIdentifier);
			std::shared_ptr<ThenKeywordToken> expected = std::make_shared<ThenKeywordToken>();
			bool isEqual = Token::checkEquality(result, expected);
			assert(isEqual == true);
		}

		TEST_METHOD(simpleKeywords_else) {
			bool forSimple = true;
			bool forceIdentifier = false;
			std::string tokenName = "else";
			std::shared_ptr<Token> result = genToken(tokenName, forSimple, forceIdentifier);
			std::shared_ptr<ElseKeywordToken> expected = std::make_shared<ElseKeywordToken>();
			bool isEqual = Token::checkEquality(result, expected);
			assert(isEqual == true);
		}

		// PQL KEYWORDS
		TEST_METHOD(pqlKeywords_select) {
			bool forSimple = false;
			bool forceIdentifier = false;
			std::string tokenName = "select";
			std::shared_ptr<Token> result = genToken(tokenName, forSimple, forceIdentifier);
			std::shared_ptr<SelectKeywordToken> expected = std::make_shared<SelectKeywordToken>();
			bool isEqual = Token::checkEquality(result, expected);
			assert(isEqual == true);
		}

		TEST_METHOD(pqlKeywords_stmt) {
			bool forSimple = false;
			bool forceIdentifier = false;
			std::string tokenName = "stmt";
			std::shared_ptr<Token> result = genToken(tokenName, forSimple, forceIdentifier);
			std::shared_ptr<StmtKeywordToken> expected = std::make_shared<StmtKeywordToken>();
			bool isEqual = Token::checkEquality(result, expected);
			assert(isEqual == true);
		}

		TEST_METHOD(pqlKeywords_assign) {
			bool forSimple = false;
			bool forceIdentifier = false;
			std::string tokenName = "assign";
			std::shared_ptr<Token> result = genToken(tokenName, forSimple, forceIdentifier);
			std::shared_ptr<AssignKeywordToken> expected = std::make_shared<AssignKeywordToken>();
			bool isEqual = Token::checkEquality(result, expected);
			assert(isEqual == true);
		}

		TEST_METHOD(pqlKeywords_variable) {
			bool forSimple = false;
			bool forceIdentifier = false;
			std::string tokenName = "variable";
			std::shared_ptr<Token> result = genToken(tokenName, forSimple, forceIdentifier);
			std::shared_ptr<VariableKeywordToken> expected = std::make_shared<VariableKeywordToken>();
			bool isEqual = Token::checkEquality(result, expected);
			assert(isEqual == true);
		}

		TEST_METHOD(pqlKeywords_constant) {
			bool forSimple = false;
			bool forceIdentifier = false;
			std::string tokenName = "constant";
			std::shared_ptr<Token> result = genToken(tokenName, forSimple, forceIdentifier);
			std::shared_ptr<ConstantKeywordToken> expected = std::make_shared<ConstantKeywordToken>();
			bool isEqual = Token::checkEquality(result, expected);
			assert(isEqual == true);
		}

		TEST_METHOD(pqlKeywords_such) {
			bool forSimple = false;
			bool forceIdentifier = false;
			std::string tokenName = "such";
			std::shared_ptr<Token> result = genToken(tokenName, forSimple, forceIdentifier);
			std::shared_ptr<SuchKeywordToken> expected = std::make_shared<SuchKeywordToken>();
			bool isEqual = Token::checkEquality(result, expected);
			assert(isEqual == true);
		}

		TEST_METHOD(pqlKeywords_that) {
			bool forSimple = false;
			bool forceIdentifier = false;
			std::string tokenName = "that";
			std::shared_ptr<Token> result = genToken(tokenName, forSimple, forceIdentifier);
			std::shared_ptr<ThatKeywordToken> expected = std::make_shared<ThatKeywordToken>();
			bool isEqual = Token::checkEquality(result, expected);
			assert(isEqual == true);
		}

		//TEST_METHOD(pqlKeywords_follows) {
		//	bool forSimple = false;
		//	bool forceIdentifier = false;
		//	std::string tokenName = "follows";
		//	std::shared_ptr<Token> result = genToken(tokenName, forSimple, forceIdentifier);
		//	std::shared_ptr<ThatKeywordToken> expected = std::make_shared<ThatKeywordToken>();
		//	bool isEqual = Token::checkEquality(result, expected);
		//	assert(isEqual == true);
		//}

		//TEST_METHOD(pqlKeywords_followsT) {
		//	bool forSimple = false;
		//	bool forceIdentifier = false;
		//	std::string tokenName = "follows*";
		//	std::shared_ptr<Token> result = genToken(tokenName, forSimple, forceIdentifier);
		//	std::shared_ptr<ThatKeywordToken> expected = std::make_shared<ThatKeywordToken>();
		//	bool isEqual = Token::checkEquality(result, expected);
		//	assert(isEqual == true);
		//}

		//TEST_METHOD(pqlKeywords_parent) {
		//	bool forSimple = false;
		//	bool forceIdentifier = false;
		//	std::string tokenName = "parent";
		//	std::shared_ptr<Token> result = genToken(tokenName, forSimple, forceIdentifier);
		//	std::shared_ptr<ThatKeywordToken> expected = std::make_shared<ThatKeywordToken>();
		//	bool isEqual = Token::checkEquality(result, expected);
		//	assert(isEqual == true);
		//}

		//TEST_METHOD(pqlKeywords_parentT) {
		//	bool forSimple = false;
		//	bool forceIdentifier = false;
		//	std::string tokenName = "parent*";
		//	std::shared_ptr<Token> result = genToken(tokenName, forSimple, forceIdentifier);
		//	std::shared_ptr<ThatKeywordToken> expected = std::make_shared<ThatKeywordToken>();
		//	bool isEqual = Token::checkEquality(result, expected);
		//	assert(isEqual == true);
		//}

		//TEST_METHOD(pqlKeywords_modifies) {
		//	bool forSimple = false;
		//	bool forceIdentifier = false;
		//	std::string tokenName = "modifies";
		//	std::shared_ptr<Token> result = genToken(tokenName, forSimple, forceIdentifier);
		//	std::shared_ptr<ThatKeywordToken> expected = std::make_shared<ThatKeywordToken>();
		//	bool isEqual = Token::checkEquality(result, expected);
		//	assert(isEqual == true);
		//}

		//TEST_METHOD(pqlKeywords_uses) {
		//	bool forSimple = false;
		//	bool forceIdentifier = false;
		//	std::string tokenName = "uses";
		//	std::shared_ptr<Token> result = genToken(tokenName, forSimple, forceIdentifier);
		//	std::shared_ptr<ThatKeywordToken> expected = std::make_shared<ThatKeywordToken>();
		//	bool isEqual = Token::checkEquality(result, expected);
		//	assert(isEqual == true);
		//}

		//TEST_METHOD(pqlKeywords_pattern) {
		//	bool forSimple = false;
		//	bool forceIdentifier = false;
		//	std::string tokenName = "pattern";
		//	std::shared_ptr<Token> result = genToken(tokenName, forSimple, forceIdentifier);
		//	std::shared_ptr<ThatKeywordToken> expected = std::make_shared<ThatKeywordToken>();
		//	bool isEqual = Token::checkEquality(result, expected);
		//	assert(isEqual == true);
		//}

		TEST_METHOD(commonKeywords_read) {
			bool forceIdentifier = false;
			std::string tokenName = "read";
			std::shared_ptr<ReadKeywordToken> expected = std::make_shared<ReadKeywordToken>();
			bool isEqual = combineAnd(permutateLanguage(tokenName, forceIdentifier, expected));
			assert(isEqual == true);
		}

		TEST_METHOD(commonKeywords_print) {
			bool forceIdentifier = false;
			std::string tokenName = "print";
			std::shared_ptr<PrintKeywordToken> expected = std::make_shared<PrintKeywordToken>();
			bool isEqual = combineAnd(permutateLanguage(tokenName, forceIdentifier, expected));
			assert(isEqual == true);
		}

		TEST_METHOD(commonKeywords_call) {
			bool forceIdentifier = false;
			std::string tokenName = "call";
			std::shared_ptr<CallKeywordToken> expected = std::make_shared<CallKeywordToken>();
			bool isEqual = combineAnd(permutateLanguage(tokenName, forceIdentifier, expected));
			assert(isEqual == true);
		}

		TEST_METHOD(commonKeywords_while) {
			bool forceIdentifier = false;
			std::string tokenName = "while";
			std::shared_ptr<WhileKeywordToken> expected = std::make_shared<WhileKeywordToken>();
			bool isEqual = combineAnd(permutateLanguage(tokenName, forceIdentifier, expected));
			assert(isEqual == true);
		}

		TEST_METHOD(commonKeywords_if) {
			bool forceIdentifier = false;
			std::string tokenName = "if";
			std::shared_ptr<IfKeywordToken> expected = std::make_shared<IfKeywordToken>();
			bool isEqual = combineAnd(permutateLanguage(tokenName, forceIdentifier, expected));
			assert(isEqual == true);
		}

		TEST_METHOD(commonKeywords_procedure) {
			bool forceIdentifier = false;
			std::string tokenName = "procedure";
			std::shared_ptr<ProcedureKeywordToken> expected = std::make_shared<ProcedureKeywordToken>();
			bool isEqual = combineAnd(permutateLanguage(tokenName, forceIdentifier, expected));
			assert(isEqual == true);
		}

		TEST_METHOD(identifiers_standardName) {
			std::string tokenName = "length";
			std::shared_ptr<IdentifierToken> expected = std::make_shared<IdentifierToken>(tokenName);
			bool isEqual = combineAnd(allCombinations(tokenName, expected));
			assert(isEqual == true);
		}

		TEST_METHOD(identifiers_alphanumericName) {
			std::string tokenName = "a1b2c3d4";
			std::shared_ptr<IdentifierToken> expected = std::make_shared<IdentifierToken>(tokenName);
			bool isEqual = combineAnd(allCombinations(tokenName, expected));
			assert(isEqual == true);
		}

		TEST_METHOD(identifiers_forceIdentifier) {
			std::string tokenName = "read";
			std::shared_ptr<IdentifierToken> expected = std::make_shared<IdentifierToken>(tokenName);
			bool isEqual = combineAnd(permutateLanguage(tokenName, true, expected));
			assert(isEqual == true);
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
			try {
				std::string tokenName = "-1";
				combineOr(allCombinations(tokenName, nonsense));
				assert(false);
			}
			catch (std::invalid_argument e) { assert(true); }
		}

		TEST_METHOD(unrecognisedCharacter) {
			try {
				std::string tokenName = "^";
				combineOr(allCombinations(tokenName, nonsense));
				assert(false);
			}
			catch (std::invalid_argument e) { assert(true); }

			try {
				std::string tokenName = "é";
				combineOr(allCombinations(tokenName, nonsense));
				assert(false);
			}
			catch (std::invalid_argument e) { assert(true); }

			try {
				std::string tokenName = " ";
				combineOr(allCombinations(tokenName, nonsense));
				assert(false);
			}
			catch (std::invalid_argument e) { assert(true); }
		}

		TEST_METHOD(invalidNameIdentifier) {
			try {
				std::string tokenName = "0digitInfront";
				combineOr(allCombinations(tokenName, nonsense));
				assert(false);
			}
			catch (std::invalid_argument e) { assert(true); }

			try {
				std::string tokenName = "_underscore";
				combineOr(allCombinations(tokenName, nonsense));
				assert(false);
			}
			catch (std::invalid_argument e) { assert(true); }

			try {
				std::string tokenName = ";invalidCharacter";
				combineOr(allCombinations(tokenName, nonsense));
				assert(false);
			}
			catch (std::invalid_argument e) { assert(true); }
		}
	};
}