#ifndef TOKENFACTORY_H
#define TOKENFACTORY_H

#include <string>
#include <string_view>
#include <vector>
#include <memory>
#include <unordered_set>

#include "../TokenClasses/IdentifierToken.h"
#include "../TokenClasses/KeywordToken.h"
#include "../TokenClasses/LiteralToken.h"
#include "../TokenClasses/OperatorToken.h"
#include "../TokenClasses/SeparatorToken.h"
#include "../TokenClasses/UnderscoreToken.h"
#include "../HelperFunctions.h"
#include "../ExceptionMessages.h"

using namespace std::string_view_literals;

// Certain keywords or operators that are common in both SIMPLE and PQL
static const std::unordered_set<std::string> common = {
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
	"%"
};

// Certain keywords or operators that will only be found in SIMPLE
static const std::unordered_set<std::string> uniqueSimple = {
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

// Certain keywords or operators that will only be found in PQL
static const std::unordered_set<std::string> uniquePql = {
	"select",
	"stmt",
	"assign",
	"variable",
	"constant",
	"procedure",
	"while",
	"if",
	//"Follows",
	//"Follows*",
	//"Parent",
	//"Parent*",
	//"Modifies",
	//"Uses",
	//"pattern",
	"such",
	"that",
	"_",
	"\"",
	",",
};

/**
* Factory class that produces tokens.
*/
class TokenFactory {
private:
	// These generateXToken all assume that the argument supplied is valid.
	
	// Generates a Token with a name that is common between SIMPLE and PQL
	static std::shared_ptr<Token> generateCommonToken(std::string_view tokenName) {
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
		if (tokenName == ";"sv) {
			return std::make_shared<SemicolonSepToken>();
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
		throw std::invalid_argument(ExceptionMessages::invalidToken);
	}

	// Generates a Token with a name that is unique to SIMPLE
	static std::shared_ptr<Token> generateSimpleToken(std::string_view tokenName) {
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

	// Generates a Token with a name that is unique to PQL
	static std::shared_ptr<Token> generatePqlToken(std::string_view tokenName) {
		if (tokenName == "select"sv) {
			return std::make_shared<SelectKeywordToken>();
		}
		if (tokenName == "stmt"sv) {
			return std::make_shared<StmtKeywordToken>();
		}
		if (tokenName == "assign"sv) {
			return std::make_shared<AssignKeywordToken>();
		}
		if (tokenName == "variable"sv) {
			return std::make_shared<VariableKeywordToken>();
		}
		if (tokenName == "constant"sv) {
			return std::make_shared<ConstantKeywordToken>();
		}
		if (tokenName == "procedure"sv) {
			return std::make_shared<ProcedureKeywordToken>();
		}
		if (tokenName == "while"sv) {
			return std::make_shared<WhileKeywordToken>();
		}
		if (tokenName == "if"sv) {
			return std::make_shared<IfKeywordToken>();
		}
		//if (tokenName == "Follows"sv) {
		//	return NULL;// std::make_shared<>();
		//}
		//if (tokenName == "Follows*"sv) {
		//	return NULL;// std::make_shared<>();
		//}
		//if (tokenName == "Parent"sv) {
		//	return NULL;// std::make_shared<>();
		//}
		//if (tokenName == "Parent*"sv) {
		//	return NULL;// std::make_shared<>();
		//}
		//if (tokenName == "Modifies"sv) {
		//	return NULL;// std::make_shared<>();
		//}
		//if (tokenName == "Uses"sv) {
		//	return NULL;// std::make_shared<>();
		//}
		//if (tokenName == "pattern"sv) {
		//	return NULL;// std::make_shared<>();
		//}
		if (tokenName == "such"sv) {
			return std::make_shared<SuchKeywordToken>();
		}
		if (tokenName == "that"sv) {
			return std::make_shared<ThatKeywordToken>();
		}
		if (tokenName == "_"sv) {
			return std::make_shared<UnderscoreToken>();
		}
		if (tokenName == "\""sv) {
			return std::make_shared<DoubleQuoSepToken>();
		}
		if (tokenName == ","sv) {
			return std::make_shared<CommaSepToken>();
		}
		throw std::invalid_argument(ExceptionMessages::invalidToken);
	}

	// Generates a Identifier Token. REMINDER that this does not validate the argument to ensure that it is a valid name
	static std::shared_ptr<Token> generateIdentifier(std::string_view tokenName) {
		return std::make_shared<IdentifierToken>(tokenName);
	}

	// Generates an Integer Literal Token. REMINDER that this does not validate the argument to ensure that it is a valid number
	static std::shared_ptr<Token> generateIntLiteral(std::string_view number) {
		return std::make_shared<IntegerLiteralToken>(number);
	}
public:
	/// <summary>
	/// Generates tokens based on a token name, whether it is for SIMPLE or not (and hence for PQL) and if the
	/// token should be forced to be an identifier or not.
	/// 
	/// Throws std::invalid_argument exception if anything supplied in the token is invalid.
	/// This can arise due to invalid names for identifiers, or other ways.
	/// </summary>
	/// 
	/// <param name="tokenName">The string to be passed to be turned into a token</param>
	/// <param name="forSimple">Whether the string is coming from a SIMPLE program or not</param>
	/// <param name="forceIdentifier">Whether to force the token to be an IdentifierToken</param>
	/// <returns>a shared pointer to the generated token</returns>
	static std::shared_ptr<Token> generateToken(std::string tokenName, bool forSimple, bool forceIdentifier = false) {
		if (tokenName.empty()) {
			throw std::invalid_argument(ExceptionMessages::invalidToken);
		}
		// Prioritises creating an identifier if given tokenName is a valid name
		if (forceIdentifier && isValidName(tokenName)) {
			return generateIdentifier(tokenName);
		}
		if (common.count(tokenName) != 0) {
			return generateCommonToken(tokenName);
		}
		if (forSimple && uniqueSimple.count(tokenName) != 0) {
			return generateSimpleToken(tokenName);
		}
		if (!forSimple && uniquePql.count(tokenName) != 0) {
			return generatePqlToken(tokenName);
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
	static std::shared_ptr<Token> generateToken(std::string_view tokenName, bool forSimple, bool forceIdentifier = false) {
		return generateToken(std::string(tokenName), forSimple, forceIdentifier);
	}

};

#endif
