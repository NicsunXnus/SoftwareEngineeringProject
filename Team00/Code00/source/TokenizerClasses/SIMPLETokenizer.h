#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <string>
#include <string_view>
#include <vector>

#include "../TokenClasses/Token.h"

using namespace std::string_view_literals;

/**
* This class represents the tokenizer that tokenizes the SIMPLE program
*/
class SIMPLETokenizer {
public:
	static std::vector<Token> tokenize(std::string_view simple);
	static Token generateToken(std::string input);
};

#endif
