#include "TokenFactory.h"

using namespace std::string_view_literals;

// Reserved Keywords
static const std::unordered_set<std::string> reservedKeyWords = {
	"read",
	"print",
	"call",
	"(",
	")",
	";",
	"+",
	"-",
	"*",
	"/",
	"%",
	"=",
	"!",
	"||",
	"&&",
	"<",
	"<=",
	">",
	">=",
	"==",
	"!="
};

// Generates a Token with a name that is common between SIMPLE and PQL
std::shared_ptr<Token> TokenFactory::generateReservedKWToken(std::string_view tokenName) {
	if (tokenName == "read"sv) {
		return std::make_shared<ReadKeywordToken>();
	}
	if (tokenName == "print"sv) {
		return std::make_shared<PrintKeywordToken>();
	}
	if (tokenName == "call"sv) {
		return std::make_shared<CallKeywordToken>();
	}
	if (tokenName == "("sv) {
		return std::make_shared<ParenOpenSepToken>();
	}
	if (tokenName == ")"sv) {
		return std::make_shared<ParenCloseSepToken>();
	}
	if (tokenName == "+"sv) {
		return std::make_shared<PlusOpToken>();
	}
	if (tokenName == "-"sv) {
		return std::make_shared<MinusOpToken>();
	}
	if (tokenName == "*"sv) {
		return std::make_shared<MultiplyOpToken>();
	}
	if (tokenName == "/"sv) {
		return std::make_shared<DivideOpToken>();
	}
	if (tokenName == "%"sv) {
		return std::make_shared<ModuloOpToken>();
	}
	if (tokenName == "="sv) {
		return std::make_shared<EqualsOpToken>();
	}
	if (tokenName == "!"sv) {
		return std::make_shared<NotOpToken>();
	}
	if (tokenName == "||"sv) {
		return std::make_shared<OrOpToken>();
	}
	if (tokenName == "&&"sv) {
		return std::make_shared<AndOpToken>();
	}
	if (tokenName == "<"sv) {
		return std::make_shared<LessThanOpToken>();
	}
	if (tokenName == "<="sv) {
		return std::make_shared<LessThanEqualOpToken>();
	}
	if (tokenName == ">"sv) {
		return std::make_shared<MoreThanOpToken>();
	}
	if (tokenName == ">="sv) {
		return std::make_shared<MoreThanEqualOpToken>();
	}
	if (tokenName == "=="sv) {
		return std::make_shared<EqualityOpToken>();
	}
	if (tokenName == "!="sv) {
		return std::make_shared<InequalityOpToken>();
	}
	throw std::invalid_argument(ExceptionMessages::invalidToken);
}

// Generates a Identifier Token. REMINDER that this does not validate the argument to ensure that it is a valid name
std::shared_ptr<Token> TokenFactory::generateIdentifier(std::string_view tokenName) {
	return std::make_shared<IdentifierToken>(tokenName);
}

// Generates an Integer Literal Token. REMINDER that this does not validate the argument to ensure that it is a valid number
std::shared_ptr<Token> TokenFactory::generateIntLiteral(std::string_view number) {
	return std::make_shared<IntegerLiteralToken>(number);
}

std::shared_ptr<Token> TokenFactory::generateTokenForSimple(std::string tokenName, bool forceIdentifier) {
	if (tokenName.empty()) {
		throw std::invalid_argument(ExceptionMessages::invalidToken);
	}
	// Prioritises creating an identifier if given tokenName is a valid name
	if (forceIdentifier && isValidName(tokenName)) {
		return generateIdentifier(tokenName);
	}
	if (reservedKeyWords.count(tokenName) != 0) {
		return generateReservedKWToken(tokenName);
	}
	if (isValidName(tokenName)) {
		return generateIdentifier(tokenName);
	}
	if (isNumber(tokenName)) {
		if (tokenName.size() > 1 && tokenName[0] == '0') {
			throw std::invalid_argument(ExceptionMessages::invalidToken);
		}
		return generateIntLiteral(tokenName);
	}
	throw std::invalid_argument(ExceptionMessages::invalidToken);
}

// Overloaded method to take in string views instead
std::shared_ptr<Token> TokenFactory::generateTokenForSimple(std::string_view tokenName, bool forceIdentifier) {
	return generateTokenForSimple(std::string(tokenName), forceIdentifier);
}

