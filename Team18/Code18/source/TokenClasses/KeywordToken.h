#ifndef KEYWORDTOKEN_H
#define KEYWORDTOKEN_H

#include <iostream>
#include <string_view>

#include "Token.h"

using namespace std::string_view_literals;

/**
* This class represents tokens objects that stem from reserved keywords 
*/
class KeywordToken : public Token {
private:
public:
	KeywordToken(std::string_view tokenName)
		: Token{ tokenName } {
	};
};

/**
* This class represents the token for the "assign" keyword, used in PQL declarations
*/
class AssignKeywordToken : public KeywordToken {
public:
	AssignKeywordToken()
		: KeywordToken{ "assign"sv } {
	};
};

/**
* This class represents the token for the "call" keyword, used in PQL declarations and in SIMPLE
*/
class CallKeywordToken : public KeywordToken {
public:
	CallKeywordToken()
		: KeywordToken{ "call"sv } {
	};
};

/**
* This class represents the token for the "constant" keyword, used in PQL declarations
*/
class ConstantKeywordToken : public KeywordToken {
public:
	ConstantKeywordToken()
		: KeywordToken{ "constant"sv } {
	};
};

/**
* This class represents the token for the "if" keyword, used in PQL declarations
*/
class IfKeywordToken : public KeywordToken {
public:
	IfKeywordToken()
		: KeywordToken{ "if"sv } {
	};
};

/**
* This class represents the token for the "while" keyword, used in PQL declarations
*/
class WhileKeywordToken : public KeywordToken {
public:
	WhileKeywordToken()
		: KeywordToken{ "while"sv } {
	};
};

/**
* This class represents the token for the "print" keyword, used in PQL declarations and in SIMPLE
*/
class PrintKeywordToken : public KeywordToken {
public:
	PrintKeywordToken()
		: KeywordToken{ "print"sv } {
	};
};

/**
* This class represents the token for the "procedure" keyword, used in PQL declarations
*/
class ProcedureKeywordToken : public KeywordToken {
public:
	ProcedureKeywordToken()
		: KeywordToken{ "procedure"sv } {
	};
};

/**
* This class represents the token for the "read" keyword, used in PQL declarations and in SIMPLE
*/
class ReadKeywordToken : public KeywordToken {
public:
	ReadKeywordToken()
		: KeywordToken{ "read"sv } {
	};
};

/**
* This class represents the token for the "stmt" keyword, used in PQL declarations
*/
class StmtKeywordToken : public KeywordToken {
public:
	StmtKeywordToken()
		: KeywordToken{ "stmt"sv } {
	};
};

/**
* This class represents the token for the "variable" keyword, used in PQL declarations
*/
class VariableKeywordToken : public KeywordToken {
public:
	VariableKeywordToken()
		: KeywordToken{ "variable"sv } {
	};
};

/**
* This class represents the token for the "select" keyword, used in PQL
*/
class SelectKeywordToken : public KeywordToken {
public:
	SelectKeywordToken()
		: KeywordToken{ "select"sv } {
	};
};

/**
* This class represents the token for the "such" keyword, used in PQL
*/
class SuchKeywordToken : public KeywordToken {
public:
	SuchKeywordToken()
		: KeywordToken{ "such"sv } {
	};
};

/**
* This class represents the token for the "that" keyword, used in PQL
*/
class ThatKeywordToken : public KeywordToken {
public:
	ThatKeywordToken()
		: KeywordToken{ "that"sv } {
	};
};

#endif
