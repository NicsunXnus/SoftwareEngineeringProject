#ifndef OPERATORTOKEN_H
#define OPERATORTOKEN_H

#include <iostream>
#include <string_view>

#include "Token.h"
#include "../../Constants/SPConstants.h"

using namespace std;

/**
 * This class represents tokens objects that are operators between expressions
 */
class OperatorToken : public Token {
 public:
  OperatorToken(string_view tokenName) : Token{tokenName} {};
};

// === ARITHMETIC OPERATORS ===

/**
 * This class represents tokens objects that are arithmetic operators between
 * expressions
 */
class ArithmeticOpToken : public OperatorToken {
 public:
  ArithmeticOpToken(string_view tokenName) : OperatorToken{tokenName} {};
};

/**
 * This class represents a "+" symbol.
 */
class PlusOpToken : public ArithmeticOpToken {
 public:
  PlusOpToken() : ArithmeticOpToken{PLUS_STRING_VIEW} {};
};

/**
 * This class represents a "-" symbol.
 */
class MinusOpToken : public ArithmeticOpToken {
 public:
  MinusOpToken() : ArithmeticOpToken{MINUS_STRING_VIEW} {};
};

/**
 * This class represents a "/" symbol.
 */
class DivideOpToken : public ArithmeticOpToken {
 public:
  DivideOpToken() : ArithmeticOpToken{DIVIDE_STRING_VIEW} {};
};

/**
 * This class represents a "*" symbol.
 */
class MultiplyOpToken : public ArithmeticOpToken {
 public:
  MultiplyOpToken() : ArithmeticOpToken{MULTIPLY_STRING_VIEW} {};
};

/**
 * This class represents a "%" symbol.
 */
class ModuloOpToken : public ArithmeticOpToken {
 public:
  ModuloOpToken() : ArithmeticOpToken{MODULO_STRING_VIEW} {};
};

#endif
