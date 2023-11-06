#ifndef LITERALTOKEN_H
#define LITERALTOKEN_H

#include <iostream>
#include <string_view>

#include "Token.h"

using namespace std;

/**
 * This class represents tokens objects that are constants or literals
 */
class LiteralToken : public Token {
 private:
 public:
  LiteralToken(string_view tokenName) : Token{tokenName} {};
};

/**
 * This class represents the token for the integer literals, used in PQL
 * declarations and in SIMPLE
 */
class IntegerLiteralToken : public LiteralToken {
 public:
  IntegerLiteralToken(string_view tokenName) : LiteralToken{tokenName} {};
};

#endif
