#ifndef QUERYBUILDER_H
#define QUERYBUILDER_H

#include <iostream>
#include <string_view>
#include <vector>
#include "QueryResultsTable.h"
#include "QueryObjects/QueryObject.h"
#include "DataAccessLayer.h"

/**
* This class a query builder object
*/
using namespace std;

class QueryBuilder {
private:
	vector <shared_ptr<QueryObject>> queryObjects;
	unordered_map<string_view, shared_ptr<QueryObject>> synonyms;
public:
	/**
	 * Constructor for the QueryBuilder object
	 */
	QueryBuilder(vector<shared_ptr<QueryObject>> queryObjects, unordered_map<string_view, shared_ptr<QueryObject>> synonyms) :queryObjects(queryObjects), synonyms(synonyms) {

	}

	/**
	 * Destructor for the QueryBuilder object
	 */
	~QueryBuilder() {};

	/**
	 * Builds the query results given a list of query objects
	 */
	vector<shared_ptr<QueryResultsTable>> buildQuery();
	  
	
};
#endif
