#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

#include "QueryParser.h"
#include "QueryObjects/DesignObjects.h"
#include "QueryObjects/DesignObjectsFactory.h"
#include "QueryObjects/SynonymObjects.h"
#include "QueryObjects/ClauseObjectFactory.h"


using namespace std;

QueryParser::QueryParser() {

};

/*
	* This function splits the string into two groups: the declaration (variable v, assign a etc..) clause and the query (select...) clause
*/
tuple<vector<string_view>, vector<string_view>> QueryParser::splitDeclarationQuery(vector<string_view> str) {
	vector<string_view> declarations;
	vector<string_view> query;
	int indexSemiColon = 0;
	for (int i = str.size() - 1; i >= 0; --i) { // get index of the split, first semicolon from the back
		string_view current = str[i];
		if (current == ";") {
			indexSemiColon = i;
			break;
		}
	}
	if (indexSemiColon == 0) {
		throw runtime_error("No Declaration clause found");
	}
	for (int i = 0; i < str.size(); ++i) {
		string_view current = str[i];
		if (i > indexSemiColon) {
			query.push_back(current);
		} else {
			declarations.push_back(current);
		}
		
	}
	if (declarations.size() == 0 || query.size() == 0) {
		throw runtime_error("No Query or Declaration clause found");
	}
	return make_tuple(declarations, query);

}

/*
* This function splits the declarations into each individual declaration (up till ";")
* delcarations = delclaration*
*/
vector<vector<string_view>> QueryParser::splitDeclarations(vector<string_view> declarations) {
	
	vector<vector<string_view>> result;
	for (int i = 0; i < declarations.size(); i++) {
		vector<string_view> currentDeclaration;
		while (i < declarations.size()) {
			string_view currStr = declarations[i];
			if (currStr == ";") {
				break;
			}
			
			currentDeclaration.push_back(currStr);
			
			i += 1;
		}
		result.push_back(currentDeclaration);
	}
	return result;
}

/*
* This function validates and converts the string tokens to QueryObjects
* declaration : design-entity synonym (',' synonym)* ';'
*/
vector<shared_ptr<QueryObject>> QueryParser::validateDeclaration(vector<string_view> declaration) {
	
	int size = declaration.size();
	if (size < 2) {
		throw runtime_error("Invalid declaration");
	}
	// first token must be a design entity, then a synonym, exception thrown by createDesignFactory otherwise
	// get the respective factory required to create QueryObjects
	shared_ptr<DesignObjectFactory> designFactory = DesignObjectFactory::createDesignFactory(declaration[0]);
	vector<shared_ptr<QueryObject>> result;
	// rest of declaration must be the synonyms themselves, convert them to Design query objects
	bool wasDeclaration = false;
	for (int i = 1; i < size; i++) {
		string_view currentDeclaration = declaration[i];
		if (wasDeclaration) {
			wasDeclaration = false;
			if (currentDeclaration != ",") {
				throw runtime_error("Missing comma");
			}
			if (i == size - 1) {
				throw runtime_error("Extra comma");
			}
			continue; // at a ",", continue parsing
		}
		if (!SynonymObject::isValid(currentDeclaration)) { // check if valid synonym
			throw runtime_error("Invalid synonym");
		}
		
		shared_ptr<QueryObject> queryObject = designFactory->create(currentDeclaration);
		if (synonyms.find(currentDeclaration) != synonyms.end()) { // synonym already declared
			throw runtime_error("Repeated synonym declaration");
		}
		else {
			synonyms[currentDeclaration] = queryObject;
		}
		wasDeclaration = true;
		result.push_back(queryObject);
	}
	return result;

}

vector<shared_ptr<QueryObject>> QueryParser::validateQuery(vector<string_view> query) {
	vector<shared_ptr<QueryObject>> result;
	
	//shared_ptr<ClauseFactory> clauseFactory = ClauseObjectFactory::createClauseFactory();
	shared_ptr<QueryObject> selectQuery;

	int currentWordIndex = 0;

	// check 'Select' keyword is present
	if (!hasSelect(query, currentWordIndex)) {
		throw runtime_error("'Select' keyword not present");
	}
	currentWordIndex++;

	// check synonym is present in declaration
	if (!isDeclared(query, currentWordIndex)) {
		throw runtime_error("Synonym not present in select clause, or synonym not declared");
	}
	selectQuery = synonyms.find(query[currentWordIndex])->second;
	result.push_back(selectQuery);
	currentWordIndex++;

	// return select query if query size == 2
	if (query.size() == currentWordIndex) {
		return result;
	}

	return result;

	// check 'such' and 'that' keywords are present
	if (!hasSuchThat(query, currentWordIndex)) {
		throw runtime_error("'such that' keywords not present");
	}
	currentWordIndex += 2;
	
	// check for relational reference
	if (!hasRelationalReference(query, currentWordIndex)) {
		throw runtime_error("error parsing such that clause");
	}
	// substrings should look like Uses ( v , a ) 
	
}

bool QueryParser::isDeclared(std::vector<string_view> query, int index) {
	if (index >= static_cast<int>(query.size())) {
		return false;
	}
	string_view synonym = query[index];
	return this->synonyms.find(synonym) != this->synonyms.end();
}

bool QueryParser::hasSelect(std::vector<string_view> query, int index) {
	return index < static_cast<int>(query.size()) && query[index] == "Select"sv;
}

bool QueryParser::hasSuchThat(std::vector<string_view> query, int index) {
	return index < static_cast<int>(query.size() - 1) && query[index] == "such"sv && query[index+1] == "that"sv;
}

bool QueryParser::hasRelationalReference(std::vector<string_view> query, int index) {
	if (index + 5 >= static_cast<int>(query.size())) {
		return false;
	}
	bool hasRelationalKeyword{};
	bool hasOpenBracket{};
	bool hasArg1{};
	bool hasComma{};
	bool hasArg2{};
	bool hasCloseBracket{};

	return hasRelationalKeyword && hasOpenBracket && hasArg1 && hasComma && hasArg2 && hasCloseBracket;
}





