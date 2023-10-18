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
  KeywordToken(std::string_view tokenName) : Token{tokenName} {};
};

/**
 * This class represents the token for the "call" keyword, used in SIMPLE
 */
class CallKeywordToken : public KeywordToken {
 public:
  CallKeywordToken() : KeywordToken{"call"sv} {};
};

/**
 * This class represents the token for the "print" keyword, used in SIMPLE
 */
class PrintKeywordToken : public KeywordToken {
 public:
  PrintKeywordToken() : KeywordToken{"print"sv} {};
};

/**
 * This class represents the token for the "read" keyword, used in SIMPLE
 */
class ReadKeywordToken : public KeywordToken {
 public:
  ReadKeywordToken() : KeywordToken{"read"sv} {};
};

#endif
