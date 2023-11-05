#ifndef IDENTIFIERTOKEN_H
#define IDENTIFIERTOKEN_H

#include <iostream>
#include <string_view>

#include "Token.h"

using namespace std;

/**
 * This class represents tokens objects that stem from user-defined names.
 */
class IdentifierToken : public Token {
 private:
 public:
  IdentifierToken(string_view tokenName) : Token{tokenName} {};
};
#endif
