#ifndef SEPARATORTOKEN_H
#define SEPARATORTOKEN_H

#include <iostream>
#include <string_view>

#include "Token.h"
#include "../../Constants/SPConstants.h"

using namespace std;

/**
 * This class represents tokens objects that stem from user-defined names.
 */
class SeparatorToken : public Token {
 private:
 public:
  SeparatorToken(string_view tokenName) : Token{tokenName} {};
};

/**
 * This class represents a ")" symbol.
 */
class ParenCloseSepToken : public SeparatorToken {
 public:
  ParenCloseSepToken() : SeparatorToken{CLOSE_BRACKET_STRING_VIEW} {};
};

/**
 * This class represents a "(" symbol.
 */
class ParenOpenSepToken : public SeparatorToken {
 public:
  ParenOpenSepToken() : SeparatorToken{OPEN_BRACKET_STRING_VIEW} {};
};

#endif
