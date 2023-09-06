#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <string>
#include <string_view>
#include <vector>

#include "TokenizerFactory.h"

using namespace std::string_view_literals;

/**
* Splits a pql query into tokens.
*
* Returns a std::vector of tokens.
*/
std::vector<std::string_view> tokenize(std::string_view pql) {

	std::vector<std::string_view> tokens;
	int startIndex{ 0 };
	bool isWord{ false };

	for (int i = 0; i < pql.length(); ++i) {
		if (pql[i] == ';' || pql[i] == ',') {
			if (isWord) {
				tokens.push_back(pql.substr(startIndex, i - startIndex));
				isWord = false;
			}
			tokens.push_back(pql.substr(i, 1));
			startIndex = i;
			continue;
		}

		if (pql[i] == ' ') {
			if (isWord) {
				tokens.push_back(pql.substr(startIndex, i - startIndex));
				isWord = false;
			}
			startIndex = i;
			continue;
		}

		// index is on a word
		if (!isWord) {
			startIndex = i;
			isWord = true;
		}
	}

	if (isWord) {
		tokens.push_back(pql.substr(startIndex, pql.length() - startIndex));
	}

	return tokens;
}

#endif
