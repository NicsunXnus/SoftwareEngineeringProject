#ifndef PQLTOKENIZER_H
#define PQLTOKENIZER_H

#include <algorithm>
#include <string>
#include <string_view>
#include <vector>
#include <sstream>
#include <unordered_set>

using namespace std::string_view_literals;

/**
* This class encapsulates a tokenizer that tokenizes PQL queries
*/
class PQLTokenizer {

public:
	/**
	* Splits a pql query into tokens.
	*
	* Returns a std::vector of tokens.
	*/
	static std::vector<std::string> tokenize(std::string pql) {

		std::vector<std::string> tokens;
		int startIndex{ 0 };
		bool isWord{ false };
		bool isWithinQuotes{ false };
		std::unordered_set<char> operators{ '+', '-', '*', '/', '%' };
		std::stringstream ss;

		for (int i = 0; i < pql.length(); ++i) {
			if (pql[i] == '<') {
				if (isWord) {
					tokens.push_back(pql.substr(startIndex, i - startIndex));
					isWord = false;
				}
				std::vector tupleTokens{ tokenizeTuple(pql, i) };
				tokens.insert(tokens.end(), tupleTokens.begin(), tupleTokens.end());
			}
			else if (!isWithinQuotes && pql[i] == '"') { // opening quotes
				if (isWord) {
					tokens.push_back(pql.substr(startIndex, i - startIndex));
					isWord = false;
				}
				ss.str(std::string()); // empties the stream
				ss << pql[i];
				isWithinQuotes = true;
				continue;
			}
			else if (isWithinQuotes) {
				if (pql[i] == '"') { // closing quotes
					ss << pql[i];
					std::string s = ss.str();
					tokens.push_back(s);
					isWithinQuotes = false;
				}
				else if (!isspace(pql[i]) && pql[i] != '\b') {
					ss << pql[i];
				}
				startIndex = i;
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

private:
	static std::vector<std::string> tokenizeTuple(std::string pql, int& index) {
		std::vector<std::string> tokens;
		int startIndex{ 0 };
		bool isIdent{ false };

		// push '<' token into tokens
		tokens.emplace_back(pql.substr(index, 1));
		++index;

		while (pql[index] != '>' && index < static_cast<int>(pql.size())) {
			// for now, support only tuples of synonyms
			if (pql[index] == ',') {
				if (isIdent) {
					tokens.emplace_back(pql.substr(startIndex, index - startIndex));
					isIdent = false;
				}
				tokens.emplace_back(pql.substr(index, 1)); // appends the corresponding symbol starting at index i to the tokens vector
				startIndex = index;
				++index;
				continue;
			}

			if (isspace(pql[index]) || pql[index] == '\b') {
				if (isIdent) {
					tokens.emplace_back(pql.substr(startIndex, index - startIndex));
					isIdent = false;
				}
				startIndex = index;
				++index;
				continue;
			}

			// index is on a word
			if (!isIdent) {
				startIndex = index;
				isIdent = true;
				++index;
				continue;
			} else {
				++index;
				continue;
			}

			// unidentified char encountered. add into tokens, let parser throw the syntax error
			std::cout << "unidentified char encountered while tokenizing tuples\n";
			if (isIdent) {
				tokens.emplace_back(pql.substr(startIndex, index - startIndex));
				isIdent = false;
			}
			tokens.emplace_back(pql.substr(index, 1));
			++index;
		}

		if (isIdent) {
			tokens.emplace_back(pql.substr(startIndex, index - startIndex));
		}
		
		// pql[index] == '>', at end of tuple
		if (pql[index] == '>') {
			tokens.emplace_back(pql.substr(index, 1));
			++index;
		}

		return tokens;
	}
};


#endif
