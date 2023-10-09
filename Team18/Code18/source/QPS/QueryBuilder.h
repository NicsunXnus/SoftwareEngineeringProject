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
	vector <shared_ptr<QueryObject>> selectClauseQueryObjects;
	vector <shared_ptr<QueryObject>> nonSelectClauseQueryObjects;
	shared_ptr<DataAccessLayer> dataAccessLayer;

public:
	/**
	 * Constructor for the QueryBuilder object
	 */
	QueryBuilder(vector<shared_ptr<QueryObject>> selectClauseQueryObjects, vector<shared_ptr<QueryObject>> nonSelectClauseQueryObjects, shared_ptr<DataAccessLayer> dataAccessLayer)
		: selectClauseQueryObjects(selectClauseQueryObjects), nonSelectClauseQueryObjects(nonSelectClauseQueryObjects), dataAccessLayer(dataAccessLayer) {

	}

	/**
	 * Destructor for the QueryBuilder object
	 */
	~QueryBuilder() {};	

	/**
	 * Builds the query results given a list of query objects excluding select clause
	 */

	vector<shared_ptr<QueryResultsTable>> buildQuery();

	/**
	 * Builds the query results given a list of query objects only for select clause
	 */

	vector<shared_ptr<QueryResultsTable>> buildQuerySelectClause();

};
#endif
