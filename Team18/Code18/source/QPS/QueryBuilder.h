#ifndef QUERYBUILDER_H
#define QUERYBUILDER_H

#include <iostream>
#include <string_view>
#include <vector>
#include "QueryResultsTable.h"
#include "QueryObjects/QueryObject.h"

/**
* This class a query builder object
*/
class QueryBuilder {
public:
	/**
	 * Constructor for the QueryBuilder object
	 */
	QueryBuilder() {};

	/**
	 * Destructor for the QueryBuilder object
	 */
	~QueryBuilder() {};	

	/**
	 * Builds the query results given a list of query objects
	 */
	vector<shared_ptr<QueryResultsTable>> buildQuery(vector<shared_ptr<QueryObject>> queryObjects);
};
#endif
