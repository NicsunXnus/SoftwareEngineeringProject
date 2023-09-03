#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "QueryParser.h"


QueryParser:: QueryParser() {

};
/*
	* This function splits the tokens into two groups: the declaration (variable v, assign a etc..) clause and the query (select...) clause
*/
std::tuple<std::vector<std::shared_ptr<Token>>, std::vector<std::shared_ptr<Token>>> splitTokens(std::vector<std::shared_ptr<Token>> tokens) {
	std::vector<std::shared_ptr<Token>> declarations;
	std::vector<std::shared_ptr<Token>> query;
	int indexSemiColon = 0;
	for (int i = tokens.size() - 1; i >= 0; --i) { // get index of the split
		std::shared_ptr<Token> currentPtr = tokens[i];
		std::string tokenName = currentPtr->getTokenName();
		if (tokenName == ";") {
			indexSemiColon = i;
			break;
		}
	}
	for (int i = 0; i < tokens.size(); ++i) {
		std::shared_ptr<Token> currentPtr = tokens[i];
		std::string tokenName = currentPtr->getTokenName();
		if (tokenName == ";" || tokenName == ",") { // do not push, useless later on
			continue;
		}
	
		if (i > indexSemiColon) {
			query.push_back(currentPtr);
		} else {
			declarations.push_back(currentPtr);
		}
		
	}
	return std::make_tuple(declarations, query);

}

// Need function to link declarations with query

/*std::vector<QueryObject> QueryParser::generateQueryObjects(std::vector<Token> tokens) {

}*/


