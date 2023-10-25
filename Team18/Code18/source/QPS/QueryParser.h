#pragma once
#ifndef QUERYPARSER_H
#define QUERYPARSER_H

#include <vector>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include "QueryValidator.h"
#include "QueryObjects/QueryObject.h"
#include "QueryObjects/DesignObjects.h"
#include "QueryObjects/QueryObjectFactory.h"
#include "QueryObjects/SynonymObjects.h"


using namespace std;
/**
* This class is responsible for the creation of QueryObjects. A concrete class for each Query
* IDEA:
* 1. Split string into individual 'tokens'
* 2. Split into declaration and query clause, assume that input query is valid up till this point, can do semantic checks (absence of either clause)
* 3. Parse the declaration and query clauses. Sends these to a Query Validator object to validate the queries
* 4. Convert to query object
*/
class QueryParser {
public:
	QueryParser();

	/*
	* This function executes the entire parsing process for a pql query
	*
	* @param tokens A tokenized pql query
	* @return A vector of query objects in the select portion of the pql query
	*/
	vector<shared_ptr<QueryObject>> parsePQL(vector<string_view> tokens);

	/*
	* This function splits the string into two groups: the declaration (variable v, assign a etc..) clause and the query (select...) clause
	*/
	tuple<vector<string_view>, vector<string_view>> splitDeclarationQuery(vector<string_view> tokens);


	/*
	* This function parses the declaration clause
	*/
	vector<shared_ptr<QueryObject>> parseDeclaration(vector<string_view> declarations);


	/*
	* This function validates the query clause
	*/
	vector<shared_ptr<QueryObject>> parseQuery(vector<string_view> query);

	unordered_map<string_view, shared_ptr<QueryObject>> getSynonyms() {
		return synonyms;
	}

	int getTupleObjectCount() {
		return synonyms_in_select;
	}

	vector<shared_ptr<QueryObject>> getSelectClauseQueryObject(vector<shared_ptr<QueryObject>> queryObjects) {
		auto start = queryObjects.begin();
		auto end = start + synonyms_in_select;
		vector<shared_ptr<QueryObject>> selectClauseQueryObjects(start, end);

		return selectClauseQueryObjects;
	}

	vector<shared_ptr<QueryObject>> getNonSelectClauseQueryObject(vector<shared_ptr<QueryObject>> queryObjects) {
		auto start = queryObjects.begin() + synonyms_in_select;
		auto end = queryObjects.end();
		vector<shared_ptr<QueryObject>> selectClauseQueryObjects(start, end);

		return selectClauseQueryObjects;
	}


private:
	// A query validator object
	std::shared_ptr<QueryValidator> validator{ make_shared<QueryValidator>() };

	// Synonyms declared in the query's declaration statements
	unordered_map<string_view, shared_ptr<QueryObject>> synonyms;

	// A mapping between the synonyms declared in the declarations statements to its design-entity
	unordered_map <string_view, ENTITY> synonymToEntity;

	// Valid relational references
	std::unordered_set<string_view> relationalReferences
	{ "Follows"sv, "Follows*"sv, "Parent"sv, "Parent*"sv, "Uses"sv, "Modifies"sv, "Calls"sv, "Calls*"sv, "Next"sv, "Next*"sv, "Affects"sv };

	// Is set to true if the query contains a semantic error
	vector<shared_ptr<SemanticErrorException>> semanticErrors;

	// The number of synonyms in the select tuple of the query
	int synonyms_in_select{ 0 }; 

	int SUCH_THAT_CLAUSE_TOKEN_COUNT{ 6 };
	int MIN_PATTERN_CLAUSE_TOKEN_COUNT{ 6 };
	int MAX_PATTERN_CLAUSE_TOKEN_COUNT{ 8 };
	int ATTR_REF_TOKEN_COUNT{ 3 }; // e.g., 'p', '.', 'procName'
	int MIN_WITH_CLAUSE_TOKEN_COUNT{ 3 }; // e.g., '"ident"', '=', '15'
	int WITH_CLAUSE_ONE_ATTR_REF_TOKEN_COUNT{ 5 }; // e.g., 'a', '.', 'procName', '=', '15'
	int MAX_WITH_CLAUSE_TOKEN_COUNT{ 7 }; // e.g., 'a', '.', 'procName', '=', 'b', '.', 'varName'


	/*
	* Helper function splits the declarations into each individual declaration (up till ";")
	*/
	vector<vector<string_view>> splitDeclarations(vector<string_view> declarations);

	// Helper function to check if Select keyword is present
	bool hasSelect(std::vector<string_view>& query, int& index);

	// Helper function to check if the token is a BOOLEAN word
	bool isBoolean(std::vector<string_view>& query, int index);

	// Helper function to check if pattern keyword is present
	bool hasPattern(std::vector<string_view>& query, int index);

	// Helper function to check if a synonym is declared
	bool isDeclared(std::string_view synonym);

	// Helper function to check if the such that keywords are present
	bool hasSuchThat(std::vector<string_view>& query, int index);

	// Creates a boolean object if there hasn't been a synonym named BOOLEAN declared. Return the synonym object otherwise
	shared_ptr<QueryObject> createBooleanObject(std::vector<string_view>& query, int& index);

	// Creates a such that clause query object, and increments the index by the number of tokens the clause has
	shared_ptr<QueryObject> createClauseObj(std::vector<string_view>& query, int& index);

	// Creates a pattern clause query object 
	shared_ptr<QueryObject> createPatternObject(std::vector<string_view>& query, int& index, int tokenCount, bool isIfPattern);

	// Creates an attribute reference query object
	shared_ptr<QueryObject> createAttrRefObject(std::vector<string_view>& query, int& index);

	// Creates an attribute reference query object from when parsing the tuple
	shared_ptr<QueryObject> createAttrRefObjectInTuple(string_view synonym, string_view attrName);

	// Returns a vector of declaration query objects or with clause objects specified in the tuple
	std::vector<shared_ptr<QueryObject>> createTupleObjects(std::vector<string_view>& query, int& index, int tokenCount);

	// Checks whether there is a with keyword in the query
	bool QueryParser::hasWith(std::vector<string_view>& query, int index);

	// Creates a comparison clause query object
	shared_ptr<QueryObject> QueryParser::createComparisonObject(std::vector<string_view>& query, 
		int& index, int tokenCount, bool is1stArgAttrRef);

	// Stores semantic errors to be thrown once syntax validation is complete
	void storeSemanticError(shared_ptr<SemanticErrorException> semanticError);
};

#endif