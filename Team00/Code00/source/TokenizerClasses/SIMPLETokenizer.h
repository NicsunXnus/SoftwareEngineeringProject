#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <string>
#include <string_view>
#include <vector>

#include "../TokenClasses/Token.h"

using namespace std::string_view_literals;
using namespace Tokens;

/**
* This class represents the tokenizer that tokenizes the pql
*/
class SIMPLETokenizer {
public:
	SIMPLETokenizer() {};

	std::vector<Token> tokenize(std::string_view pql);
};

#endif
