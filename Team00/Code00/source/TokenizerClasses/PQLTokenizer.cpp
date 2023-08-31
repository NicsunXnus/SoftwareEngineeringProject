#include <memory>
#include <string.h>
#include "PQLTokenizer.h"
/**
* Splits a pql query into tokens.
* 
* Returns a std::vector of tokens.
*/
std::vector<Token> tokenize(std::string_view pql) {

	std::vector<std::shared_ptr<Token>> tokens;
	int startIndex{ 0 };
	bool isWord{ false };

	for (int i = 0; i < pql.length(); ++i) {
		if (pql[i] == ';') {
			tokens.push_back(std::make_shared<SemicolonSepToken>());
			continue;
		}

		if (pql[i] == ',') {
			tokens.push_back(std::make_shared<)
		}
	}

}

