#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <string>
#include <string_view>
#include <vector>

#include "TokenizerFactory.h"

using namespace std::string_view_literals;

/**
* Splits a SIMPLE source codeinto tokens.
*
* Returns a std::vector of tokens.
*/
std::vector<std::shared_ptr<Token>> tokenize(std::string_view pql) {

}

#endif
