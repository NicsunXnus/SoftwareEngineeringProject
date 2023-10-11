#pragma once
#ifndef QUERYPARSER_H
#define QUERYPARSER_H

#include <vector>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
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
* 3. Within the declaration and query clause, check for validity. If both are valid, input query must be valid
* 4. Convert to query object
*/
class QueryParser  {
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
	* This function validates the declaration clause
	*/
	vector<shared_ptr<QueryObject>> validateDeclaration(vector<string_view> declarations);


	/*
	* This function validates the query clause
	*/
	vector<shared_ptr<QueryObject>> validateQuery(vector<string_view> query);

	unordered_map<string_view, shared_ptr<QueryObject>> getSynonyms() {
		return synonyms;
	}

	int getTupleObjectCount() {
		return QUERY_OBJECTS_IN_TUPLE_COUNT;
	}

	vector<shared_ptr<QueryObject>> getSelectClauseQueryObject(vector<shared_ptr<QueryObject>> queryObjects) {
		auto start = queryObjects.begin();
		auto end = start + QUERY_OBJECTS_IN_TUPLE_COUNT;
		vector<shared_ptr<QueryObject>> selectClauseQueryObjects(start, end);

		return selectClauseQueryObjects;
	}

	vector<shared_ptr<QueryObject>> getNonSelectClauseQueryObject(vector<shared_ptr<QueryObject>> queryObjects) {
		auto start = queryObjects.begin() + QUERY_OBJECTS_IN_TUPLE_COUNT;
		auto end = queryObjects.end();
		vector<shared_ptr<QueryObject>> selectClauseQueryObjects(start, end);

		return selectClauseQueryObjects;
	}


private:
	// Synonyms declared in the query's declaration statements
	unordered_map<string_view, shared_ptr<QueryObject>> synonyms;

	// A mapping between the synonyms declared in the declarations statements to its design-entity
	unordered_map <string_view, ENTITY> synonymToEntity;

	// Valid relational references
	std::unordered_set<string_view> relationalReferences
		{"Follows"sv, "Follows*"sv, "Parent"sv, "Parent*"sv, "Uses"sv, "Modifies"sv, "Calls"sv, "Calls*"sv, "Next"sv, "Next*"sv, "Affects"sv};

	// Is set to true if the query contains a semantic error
	vector<shared_ptr<SemanticErrorException>> semanticErrors;

	int SUCH_THAT_CLAUSE_TOKEN_COUNT{ 6 };
	int MIN_PATTERN_CLAUSE_TOKEN_COUNT{ 6 };
	int MAX_PATTERN_CLAUSE_TOKEN_COUNT{ 8 };
	int ATTR_REF_TOKEN_COUNT{ 3 }; // e.g., 'p', '.', 'procName'
	int QUERY_OBJECTS_IN_TUPLE_COUNT{ 1 }; // The number of query objects in the select tuple of the query


	/*
	* Helper function splits the declarations into each individual declaration (up till ";")
	*/
	vector<vector<string_view>> splitDeclarations(vector<string_view> declarations);

	// Helper function to check if Select keyword is present
	bool hasSelect(std::vector<string_view>& query, int& index);

	// Helper function to check if pattern keyword is present
	bool hasPattern(std::vector<string_view>& query, int index);

	// Helper function to check if a synonym is declared
	bool isDeclared(std::vector<string_view>& query, int index);

	// Helper function to check if such that is present
	bool hasSuchThat(std::vector<string_view>& query, int index);

	// Helper function to check if a such that clause is present
	bool hasRelationalReference(std::vector<string_view>& query, int index);

	// Creates a such that clause query object, and increments the index by the number of tokens the clause has
	shared_ptr<QueryObject> createClauseObj(std::vector<string_view>& query, int& index);

	// Helper function to check if a pattern clause is present
	bool hasPatternClause(std::vector<string_view>& query, int index, int& tokenCount);

	// Creates a pattern clause query object 
	shared_ptr<QueryObject> createPatternObject(std::vector<string_view>& query, int& index, int tokenCount);

	/*
	* Helper function to check if select clause is a tuple, and gets the number of tokens until the tuple bracket closes
	* Does not check for validity of inputs, only checks if there is a correct sequence of tokens for a valid tuple
	*/ 
	bool isSelectTuple(std::vector<string_view>& query, int index, int& tokenCount);

	// Helper function to check if select clause has the structure of an elem (synonym or attrRef) and gets the respective token count
	bool isSelectElem(std::vector<string_view>& query, int index, int& tokenCount);

	// Helper function to check if index has the structure of a attrRef
	bool isAttrRef(std::vector<string_view>& query, int index, int& tokenCount);

	// Creates an attribute reference query object
	shared_ptr<QueryObject> createAttrRefObject(std::vector<string_view>& query, int& index);

	// Creates an attribute reference query object from when parsing the tuple
	shared_ptr<QueryObject> createAttrRefObjectInTuple(string_view synonym, string_view attrName);

	// Returns a vector of declaration query objects or with clause objects specified in the tuple
	std::vector<shared_ptr<QueryObject>> createTupleObjects(std::vector<string_view>& query, int& index, int tokenCount);

	// Stores semantic errors to be thrown once syntax validation is complete
	void storeSemanticError(shared_ptr<SemanticErrorException> semanticError);
};

#endif