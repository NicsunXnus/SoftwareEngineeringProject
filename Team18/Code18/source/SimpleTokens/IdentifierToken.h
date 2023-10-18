#ifndef IDENTIFIERTOKEN_H
#define IDENTIFIERTOKEN_H

#include <iostream>
#include <string_view>

#include "Token.h"

/**
 * This class represents tokens objects that stem from user-defined names.
 */
class IdentifierToken : public Token {
 private:
 public:
  IdentifierToken(std::string_view tokenName) : Token{tokenName} {};
};
#endif
