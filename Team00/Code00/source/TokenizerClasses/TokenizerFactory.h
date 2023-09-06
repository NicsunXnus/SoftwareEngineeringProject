#ifndef TOKENIZERFACTORY_H
#define TOKENIZERFACTORY_H

#include <string>
#include <string_view>
#include <vector>
#include <memory>

#include "../TokenClasses/IdentifierToken.h"
#include "../TokenClasses/KeywordToken.h"
#include "../TokenClasses/LiteralToken.h"
#include "../TokenClasses/OperatorToken.h"
#include "../TokenClasses/SeparatorToken.h"
#include "../TokenClasses/UnderscoreToken.h"

using namespace std::string_view_literals;

inline std::shared_ptr<Token> tokenizerGenerator(std::string_view tokenName) {
	if (tokenName == "assign"sv) {
		return std::make_shared<AssignKeywordToken>();
	}
	else if (tokenName == "call"sv) {
		return std::make_shared<CallKeywordToken>();
	}
	else if (tokenName == "constant"sv) {
		return std::make_shared<ConstantKeywordToken>();
	}
	else if (tokenName == "else"sv) {
		return std::make_shared<ElseKeywordToken>();
	}
	else if (tokenName == "if"sv) {
		return std::make_shared<IfKeywordToken>();
	}
	else if (tokenName == "print"sv) {
		return std::make_shared<PrintKeywordToken>();
	}
	else if (tokenName == "procedure"sv) {
		return std::make_shared<ProcedureKeywordToken>();
	}
	else if (tokenName == "read"sv) {
		return std::make_shared<ReadKeywordToken>();
	}
	else if (tokenName == "stmt"sv) {
		return std::make_shared<StmtKeywordToken>();
	}
	else if (tokenName == "then"sv) {
		return std::make_shared<ThenKeywordToken>();
	}
	else if (tokenName == "variable"sv) {
		return std::make_shared<VariableKeywordToken>();
	}
	else if (tokenName == "while"sv) {
		return std::make_shared<WhileKeywordToken>();
	}
	else if (tokenName == "="sv) {
		return std::make_shared<EqualsOpToken>();
	}
	else if (tokenName == "/"sv) {
		return std::make_shared<DivideOpToken>();
	}
	else if (tokenName == "-"sv) {
		return std::make_shared<MinusOpToken>();
	}
	else if (tokenName == "%"sv) {
		return std::make_shared<ModuloOpToken>();
	}
	else if (tokenName == "*"sv) {
		return std::make_shared<MultiplyOpToken>();
	}
	else if (tokenName == "+"sv) {
		return std::make_shared<PlusOpToken>();
	}
	else if (tokenName == "&&"sv) {
		return std::make_shared<AndOpToken>();
	}
	else if (tokenName == "!"sv) {
		return std::make_shared<NotOpToken>();
	}
	else if (tokenName == "||"sv) {
		return std::make_shared<OrOpToken>();
	}
	else if (tokenName == ">"sv) {
		return std::make_shared<MoreThanOpToken>();
	}
	else if (tokenName == ">="sv) {
		return std::make_shared<MoreThanEqualOpToken>();
	}
	else if (tokenName == "<"sv) {
		return std::make_shared<LessThanOpToken>();
	}
	else if (tokenName == "<="sv) {
		return std::make_shared<LessThanEqualOpToken>();
	}
	else if (tokenName == "!="sv) {
		return std::make_shared<InequalityOpToken>();
	}
	else if (tokenName == "=="sv) {
		return std::make_shared<EqualityOpToken>();
	}
	else if (tokenName == "}"sv) {
		return std::make_shared<CurlyCloseSepToken>();
	}
	else if (tokenName == "{"sv) {
		return std::make_shared<CurlyOpenSepToken>();
	}
	else if (tokenName == "\""sv) {
		return std::make_shared<DoubleQuoSepToken>();
	}
	else if (tokenName == ")"sv) {
		return std::make_shared<ParenCloseSepToken>();
	}
	else if (tokenName == "("sv) {
		return std::make_shared<ParenOpenSepToken>();
	}
	else if (tokenName == ";"sv) {
		return std::make_shared<SemicolonSepToken>();
	}
	else if (tokenName == ","sv) {
		return std::make_shared<CommaSepToken>();
	}
	else if (tokenName == "_"sv) {
		return std::make_shared<UnderscoreToken>();
	}
	else if (tokenName == "_"sv) {
		return std::make_shared<UnderscoreToken>();
	}
	else if (isdigit(tokenName[0])) {
		return std::make_shared<IntegerLiteralToken>(tokenName);
	}
	else {
		return std::make_shared<IdentifierToken>(tokenName);
	}

}


#endif
