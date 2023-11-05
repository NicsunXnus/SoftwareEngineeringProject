#include <string>
#include <string_view>
#include <unordered_map>

#include "TokenFactory.h"
#include "IdentifierToken.h"
#include "LiteralToken.h"
#include "OperatorToken.h"
#include "SeparatorToken.h"
#include "../../HelperFunctions.h"
#include "../../ExceptionMessages.h"
#include "../../Constants/SPConstants.h"

static const unordered_map<string_view, shared_ptr<Token>> arithmeticOpTokens = {
	{OPEN_BRACKET_STRING_VIEW, make_shared<ParenOpenSepToken>()},
	{CLOSE_BRACKET_STRING_VIEW, make_shared<ParenCloseSepToken>()},
	{PLUS_STRING_VIEW, make_shared<PlusOpToken>()},
	{MINUS_STRING_VIEW, make_shared<MinusOpToken>()},
	{MULTIPLY_STRING_VIEW, make_shared<MultiplyOpToken>()},
	{DIVIDE_STRING_VIEW, make_shared<DivideOpToken>()},
	{MODULO_STRING_VIEW, make_shared<ModuloOpToken>()},
};

shared_ptr<Token> TokenFactory::generateArithmeticOpToken(string_view tokenName) {
	return arithmeticOpTokens.at(tokenName);
}

shared_ptr<Token> TokenFactory::generateIdentifier(string_view tokenName) {
	return make_shared<IdentifierToken>(tokenName);
}

shared_ptr<Token> TokenFactory::generateIntLiteral(string_view number) {
	return make_shared<IntegerLiteralToken>(number);
}

shared_ptr<Token> TokenFactory::generateTokenForSimple(string tokenName, bool forceIdentifier) {
	if (tokenName.empty()) {
		throw invalid_argument(ExceptionMessages::invalidToken);
	}
	tokenName = trimWhitespaces(tokenName);
	// Prioritises creating an identifier if given tokenName is a valid name
	if (forceIdentifier && isValidName(tokenName)) {
		return generateIdentifier(tokenName);
	}
	if (arithmeticOpTokens.find(tokenName) != arithmeticOpTokens.end()) {
		return generateArithmeticOpToken(tokenName);
	}
	if (isValidName(tokenName)) {
		return generateIdentifier(tokenName);
	}
	if (isValidNumber(tokenName)) {
		return generateIntLiteral(tokenName);
	}
	throw invalid_argument(ExceptionMessages::invalidToken);
}

shared_ptr<Token> TokenFactory::generateTokenForSimple(string_view tokenName, bool forceIdentifier) {
	return generateTokenForSimple(string(tokenName), forceIdentifier);
}

