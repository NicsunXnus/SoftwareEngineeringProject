#include "SIMPLETokenizer.h"
#include "TokenizerFunctions.h"
#include "../TokenClasses/IdentifierToken.h"
#include "../TokenClasses/KeywordToken.h"
#include "../TokenClasses/LiteralToken.h"
#include "../TokenClasses/OperatorToken.h"
#include "../TokenClasses/SeparatorToken.h"

static bool isNumber(std::string input) {
	for (char const& ch : input) {
		if (std::isdigit(ch) == 0)
			return false;
	}
	return true;
}

static std::shared_ptr<Token> generateToken(std::string input) {
	if (input == "read") {
		return std::make_shared<ReadKeywordToken>();
	}
	else if (input == "print") {
		return std::make_shared<PrintKeywordToken>();
	}
	else if (input == "call") {
		return std::make_shared<CallKeywordToken>();
	}
	else if (input == "if") {
		return std::make_shared<IfKeywordToken>();
	}
	else if (input == "then") {
		return std::make_shared<ThenKeywordToken>();
	}
	else if (input == "else") {
		return std::make_shared<ElseKeywordToken>();
	}
	else if (input == "while") {
		return std::make_shared<WhileKeywordToken>();
	}
	else if (input == "procedure") {
		return std::make_shared<ProcedureKeywordToken>();
	}
	else if (input == "=") {
		return std::make_shared<EqualsOpToken>();
	}
	else if (input == "==") {
		return std::make_shared<EqualityOpToken>();
	}
	else if (input == "!=") {
		return std::make_shared<InequalityOpToken>();
	}
	else if (input == "+") {
		return std::make_shared<PlusOpToken>();
	}
	else if (input == "-") {
		return std::make_shared<MinusOpToken>();
	}
	else if (input == "*") {
		return std::make_shared<MultiplyOpToken>();
	}
	else if (input == "/") {
		return std::make_shared<DivideOpToken>();
	}
	else if (input == "%") {
		return std::make_shared<ModuloOpToken>();
	}
	else if (input == ">=") {
		return std::make_shared<MoreThanEqualOpToken>();
	}
	else if (input == ">") {
		return std::make_shared<MoreThanOpToken>();
	}
	else if (input == "<=") {
		return std::make_shared<LessThanEqualOpToken>();
	}
	else if (input == "<") {
		return std::make_shared<LessThanOpToken>();
	}
	else if (input == "||") {
		return std::make_shared<OrOpToken>();
	}
	else if (input == "&&") {
		return std::make_shared<AndOpToken>();
	}
	else if (input == "!") {
		return std::make_shared<NotOpToken>();
	}
	else if (input == ";") {
		return std::make_shared<SemicolonSepToken>();
	}
	else if (input == "(") {
		return std::make_shared<ParenOpenSepToken>();
	}
	else if (input == ")") {
		return std::make_shared<ParenCloseSepToken>();
	}
	else if (input == "{") {
		return std::make_shared<CurlyOpenSepToken>();
	}
	else if (input == "}") {
		return std::make_shared<CurlyCloseSepToken>();
	}
	else if (isNumber(input)) {
		return std::make_shared<IntegerLiteralToken>(input);
	}
	else {
		// TODO: ACCOUNT FOR BADLY NAMED VARIABLES AS WELL
		return std::make_shared<IdentifierToken>(input);
	}
}

/**
* Splits a SIMPLE query into tokens.
* 
* Returns a std::vector of tokens.
*/
static std::vector<std::shared_ptr<Token>> tokenize(std::string_view simple) {
	//return std::vector<Token>();
	std::vector<std::string> split = TokenizerFunctions::splitString(std::string(simple));
	std::vector<std::shared_ptr<Token>> output;
	for (std::string s : split) {
		//if (s[0] == char("!")) {
		//	output.push_back(generateToken("!"));
		//	s = s.substr(1)
		//}

		// ASSUME THAT THERE IS A SPACE BETWEEN EVERY TOKENISABLE THING
		//  TODO: REMOVE THIS ASSUMPTION
		std::shared_ptr<Token> ptr = generateToken(s);
		output.push_back(ptr);
	}
	return output;
}

