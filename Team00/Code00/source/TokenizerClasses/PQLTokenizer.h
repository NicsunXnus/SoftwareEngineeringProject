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
std::vector<std::shared_ptr<Token>> tokenize(std::string_view pql) {

	std::vector<std::shared_ptr<Token>> tokens;
	int startIndex{ 0 };
	bool isWord{ false };

	for (int i = 0; i < pql.length(); ++i) {
		if (pql[i] == ';') {
			if (isWord) {
				tokens.push_back(tokenizerGenerator(pql.substr(startIndex, i)));
				isWord = false;
			}
			tokens.push_back(std::make_shared<SemicolonSepToken>());
			continue;
		}

		if (pql[i] == ',') {
			if (isWord) {
				tokens.push_back(tokenizerGenerator(pql.substr(startIndex, i)));
				isWord = false;
			}
			tokens.push_back(std::make_shared<CommaSepToken>());
			continue;
		}

		if (pql[i] == ' ') {
			isWord = false;
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
		tokens.push_back(tokenizerGenerator(pql.substr(startIndex, pql.length())));
	}

	return tokens;
}

#endif
