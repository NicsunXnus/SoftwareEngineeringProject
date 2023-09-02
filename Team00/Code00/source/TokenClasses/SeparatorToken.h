#ifndef SEPARATORTOKEN_H
#define SEPARATORTOKEN_H

#include <iostream>
#include <string_view>

#include "Token.h"

/**
* This class represents tokens objects that stem from user-defined names.
*/
class SeparatorToken : public Token {
private:
public:
	SeparatorToken(std::string_view tokenName)
		: Token{ tokenName } {
	};
};

/**
* This class represents a "}" symbol.
*/
class CurlyCloseSepToken : public SeparatorToken {
public:
	CurlyCloseSepToken()
		: SeparatorToken{ "}"sv } {};
};

/**
* This class represents a "{" symbol.
*/
class CurlyOpenSepToken : public SeparatorToken {
public:
	CurlyOpenSepToken()
		: SeparatorToken{ "{"sv } {};
};

/**
* This class represents a '"' symbol.
*/
class DoubleQuoSepToken : public SeparatorToken {
public:
	DoubleQuoSepToken()
		: SeparatorToken{ "\""sv} {};
};

/**
* This class represents a ")" symbol.
*/
class ParenCloseSepToken : public SeparatorToken {
public:
	ParenCloseSepToken()
		: SeparatorToken{ ")"sv } {};
};

/**
* This class represents a "(" symbol.
*/
class ParenOpenSepToken : public SeparatorToken {
public:
	ParenOpenSepToken()
		: SeparatorToken{ "("sv } {};
};

/**
* This class represents a ";" symbol.
*/
class SemicolonSepToken : public SeparatorToken {
public:
	SemicolonSepToken()
		: SeparatorToken{ ";"sv } {};
};

/**
* This class represents a "," symbol.
*/
class CommaSepToken : public SeparatorToken {
public:
	CommaSepToken()
		: SeparatorToken{ ","sv } {};
};

#endif
