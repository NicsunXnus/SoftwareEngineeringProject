#ifndef PQLTOKENIZER_H
#define PQLTOKENIZER_H

#include <algorithm>
#include <string>
#include <string_view>
#include <vector>
#include <sstream>
#include <unordered_set>

using namespace std::string_view_literals;

inline std::unordered_set<char> TOKENIZED_CHARS{ ';', ',', '(', ')', '_', '.' , '='};

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
			if (pql[i] == '<') { // tokenizing tuple
				if (isWord) {
					tokens.push_back(pql.substr(startIndex, i - startIndex));
					isWord = false;
				}
				std::vector<std::string> tupleTokens{ tokenizeTuple(pql, i) };
				tokens.insert(tokens.end(), tupleTokens.begin(), tupleTokens.end());

				continue;
			}
			else if (pql[i] == '"') { // opening quotes
				if (isWord) {
					tokens.push_back(pql.substr(startIndex, i - startIndex));
					isWord = false;
				}
				std::string quoteToken{ tokenizeQuotes(pql, i) };
				tokens.emplace_back(quoteToken);

				continue;
			}

			if (TOKENIZED_CHARS.find(pql[i]) != TOKENIZED_CHARS.end()) {
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
	// removes whitespace within quotes
	static std::string tokenizeQuotes(std::string pql, int& index) {
		if (pql[index] != '"') {
			throw std::runtime_error("This should not be called");
		}
		std::stringstream ss;

		ss.str(std::string()); // empties the stream
		ss << pql[index];
		index++;
		
		while (index < pql.size() && pql[index] != '"') {
			if (!isspace(pql[index]) && pql[index] != '\b') {
				ss << pql[index];
			}
			index++;
		}

		if (index == pql.size()) {
			// reached end of pql without finding closing quotes
			// invalid argument, just return incomplete quote
			return ss.str();
		}

		// add the closing quotes
		ss << pql[index];

		return ss.str();
	}

	static std::vector<std::string> tokenizeTuple(std::string pql, int& index) {
		std::vector<std::string> tokens;
		int startIndex{ 0 };
		bool isIdent{ false };

		// push '<' token into tokens
		tokens.emplace_back(pql.substr(index, 1));
		++index;

		while (index < static_cast<int>(pql.size()) && pql[index] != '>') {
			// for now, support only tuples of synonyms
			if (TOKENIZED_CHARS.find(pql[index]) != TOKENIZED_CHARS.end()) {
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

		}

		if (isIdent) {
			tokens.emplace_back(pql.substr(startIndex, index - startIndex));
		}
		
		// pql[index] == '>', at end of tuple
		if (pql[index] == '>') {
			tokens.emplace_back(pql.substr(index, 1));
		}

		return tokens;
	}


};


#endif
