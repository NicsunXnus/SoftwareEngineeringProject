#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <string>
#include <string_view>
#include <vector>

#include "../TokenClasses/Token.h"

using namespace std::string_view_literals;

/**
* This class represents the tokenizer that tokenizes the pql
*/
class PQLTokenizer {
public:
	PQLTokenizer() {};

	std::vector<Token> tokenize(std::string_view pql);
};

#endif
