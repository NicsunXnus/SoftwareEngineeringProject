#ifndef PQLTOKENIZER_H
#define PQLTOKENIZER_H

#include <string>
#include <string_view>
#include <vector>
#include <sstream>
#include <unordered_set>

using namespace std::string_view_literals;

/**
* Splits a pql query into tokens.
*
* Returns a std::vector of tokens.
*/
static std::vector<std::string_view> tokenize(std::string_view pql) {

	std::vector<std::string_view> tokens;
	int startIndex{ 0 };
	bool isWord{ false };
	bool isWithinQuotes{ false };
	std::unordered_set<char> operators { '+', '-', '*', '/', '%'};
	std::stringstream ss;

	for (int i = 0; i < pql.length(); ++i) {
		if (!isWithinQuotes && pql[i] == '"') { // opening quotes
			if (isWord) {
				tokens.push_back(pql.substr(startIndex, i - startIndex));
				isWord = false;
			}
			ss.clear();
			isWithinQuotes = true;
			startIndex = i;
			continue;
		} else if (isWithinQuotes) {
			if (!isspace(pql[i]) && pql[i] != '\b') {
				ss << pql[i];
			} else if (pql[i] == '"') { // closing quotes
				tokens.push_back(ss.str());
				isWithinQuotes = false;
			}
			continue;
		}

		if (pql[i] == ';' || pql[i] == ',' || pql[i] == '(' || pql[i] == ')' || pql[i] == '_') {
			if (isWord) {
				tokens.push_back(pql.substr(startIndex, i - startIndex));
				isWord = false;
			}
			tokens.push_back(pql.substr(i, 1)); // appends the corresponding symbol starting at index i to the tokens vector
			startIndex = i;
			continue;
		}

		if (isspace(pql[i]) || pql[i] == '\b') {
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
