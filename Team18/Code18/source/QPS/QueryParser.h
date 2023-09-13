#pragma once
#ifndef QUERYPARSER_H
#define QUERYPARSER_H

#include <vector>
#include <iostream>
#include <unordered_map>
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
	unordered_map<string_view, shared_ptr<QueryObject>> synonyms;
	QueryParser();
	
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
private:
	/*
	* Helper function splits the declarations into each individual declaration (up till ";")
	*/
	vector<vector<string_view>> splitDeclarations(vector<string_view> declarations);
	/*
	* Helper function to check if Select keyword is present
	*/
	bool hasSelect(std::vector<string_view> query, int index);

	/*
	* Helper function to check if a synonym is declared
	*/
	bool isDeclared(std::vector<string_view> query, int index);

	/*
	* Helper function to check if such that is present
	*/
	bool hasSuchThat(std::vector<string_view> query, int index);

	/*
	* Helper function to check if a such that clause is present
	*/
	bool hasRelationalReference(std::vector<string_view> query, int index);
};

#endif