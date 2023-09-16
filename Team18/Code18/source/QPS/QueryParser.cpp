#pragma once
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

#include "QueryParser.h"
#include "QueryObjects/ClauseArg.h"
#include "QueryObjects/ClauseObject.h"
#include "QueryObjects/InvalidQueryObject.h"



using namespace std;

QueryParser::QueryParser() {

};

/*
	* This function splits the string into two groups: the declaration (variable v, assign a etc..) clause and the query (select...) clause
*/
tuple<vector<string_view>, vector<string_view>> QueryParser::splitDeclarationQuery(vector<string_view> words) {
	vector<string_view> declarations;
	vector<string_view> query;
	int indexSemiColon = 0;
	for (int i = words.size() - 1; i >= 0; --i) { // get index of the split, first semicolon from the back
		string_view current = words[i];
		if (current == ";") {
			indexSemiColon = i;
			break;
		}
	}
	if (indexSemiColon == 0) {
		throw runtime_error("No Declaration clause found");
	}
	for (int i = 0; i < words.size(); ++i) {
		string_view current = words[i];
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
vector<shared_ptr<QueryObject>> QueryParser::validateDeclaration(vector<string_view> declarations) {
	vector<shared_ptr<QueryObject>> converted;
	vector<vector<string_view>> splittedDeclarations = splitDeclarations(declarations);
	int totalDeclarations = splittedDeclarations.size();
	for (const vector<string_view>declaration : splittedDeclarations) {
		int size = declaration.size();
		if (size < 2) { // declarations must contain at least 2 items; design-entity synonym
			throw runtime_error("Invalid declaration");
		}
		// first token must be a design entity, then a synonym, exception thrown by createDesignFactory otherwise
		// get the respective factory required to create QueryObjects
		shared_ptr<QueryObjectFactory> designFactory = QueryObjectFactory::createFactory(declaration[0]);
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
				std::string entityType(declaration[0].begin(), declaration[0].end());
				synonymToEntity[currentDeclaration] = EntityEnumToString(entityType);
			}
			wasDeclaration = true;
			converted.push_back(queryObject);
		}
	}
	
	
	return converted;

}

vector<shared_ptr<QueryObject>> QueryParser::validateQuery(vector<string_view> query) {
	vector<shared_ptr<QueryObject>> result;
	

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
	shared_ptr<QueryObject> selectQuery{ synonyms.find(query[currentWordIndex])->second };
	result.push_back(selectQuery);
	currentWordIndex++;

	// return select query if query size == 2
	if (query.size() == currentWordIndex) {
		return result;
	}

	// loop to check for such that clauses and pattern clauses
	while (currentWordIndex < static_cast<int>(query.size())) {
		bool isSuchThat{ hasSuchThat(query, currentWordIndex) }; // checks the current clause is a such that clause
		bool isPattern{ hasPattern(query, currentWordIndex) }; // checks the current clause is a patter clause
		
		if (isSuchThat) {
			currentWordIndex += 2;

			if (!hasRelationalReference(query, currentWordIndex)) {
				throw runtime_error("such that clause has invalid syntax");
			}

			// Construct query object
			shared_ptr<QueryObject> suchThatClauseObj{ createClauseObj(query, currentWordIndex) };
			result.push_back(suchThatClauseObj);

		} else if (isPattern) {
			currentWordIndex += 1;
		}
	}
	return result;
}

bool QueryParser::isDeclared(std::vector<string_view>& query, int index) {
	if (index >= static_cast<int>(query.size())) {
		return false;
	}
	string_view synonym = query[index];
	return this->synonyms.find(synonym) != this->synonyms.end();
}

bool QueryParser::hasSelect(std::vector<string_view>& query, int index) {
	return index < static_cast<int>(query.size()) && query[index] == "Select"sv;
}

bool QueryParser::hasPattern(std::vector<string_view>& query, int index) {
	return index < static_cast<int>(query.size()) && query[index] == "pattern"sv;
}

bool QueryParser::hasSuchThat(std::vector<string_view>& query, int index) {
	return index < static_cast<int>(query.size() - 1) && query[index] == "such"sv && query[index+1] == "that"sv;
}

bool QueryParser::hasRelationalReference(std::vector<string_view>& query, int index) {
	// 6 here is the expected number of tokens we generate from tokenizing a relational reference clause
	// Eg., "Uses" "(" "a" "," "b" ")"
	if (index > static_cast<int>(query.size()) - SUCH_THAT_CLAUSE_TOKEN_COUNT) {
		return false;
	}

	bool hasRelationalKeyword{ this->relationalReferences.find(query[index]) != this->relationalReferences.end()};
	bool hasOpenBracket{ query[index + 1] == "("sv };
	bool hasComma{ query[index + 3] == ","sv };
	bool hasCloseBracket{ query[index + 5] == ")"sv };

	bool hasRR{ hasRelationalKeyword && hasOpenBracket && hasComma && hasCloseBracket };
	return hasRR;
}

shared_ptr<QueryObject> QueryParser::createClauseObj(std::vector<string_view>& query, int index) {
	string_view relationalReference{ query[index] };

	shared_ptr<QueryObjectFactory> clauseFactory{ QueryObjectFactory::createFactory(relationalReference) };

	// create a vector of args for the clause objects
	std::vector<shared_ptr<ClauseArg>> argVector;
	
	// create clauseArg object for first argument
	string_view arg1Name{ query[index + 2] };
	shared_ptr<SynonymObject> synonym1;
	if (SynonymObject::isValid(arg1Name)) {
		if (synonyms.find(arg1Name) == synonyms.end()) { // argument is an undeclared synonym
			throw std::runtime_error("Semantic error: Synonym in clause is undeclared");
		}
		shared_ptr<SynonymObject> synonym1 = make_shared<SynonymObject>(arg1Name, synonymToEntity[arg1Name]);
	}
	argVector.push_back(make_shared<ClauseArg>(arg1Name, synonym1));

	// create clauseArg object for second argument
	string_view arg2Name{ query[index + 4] };
	shared_ptr<SynonymObject> synonym2;
	if (SynonymObject::isValid(arg2Name)) {
		if (synonyms.find(arg2Name) == synonyms.end()) { // argument is an undeclared synonym
			throw std::runtime_error("Semantic error: Synonym in clause is undeclared");
		}
		shared_ptr<SynonymObject> synonym2 = make_shared<SynonymObject>(arg2Name, synonymToEntity[arg2Name]);
	}
	argVector.push_back(make_shared<ClauseArg>(arg2Name, synonym2));

	try {
		return clauseFactory->create(relationalReference, argVector);
	} catch (const std::exception& e) {
		std::cout << e.what() << '\n';
		return make_shared<InvalidQueryObject>("Error in creating clause object"sv);
	} 
}



