#ifndef QUERYBUILDER_H
#define QUERYBUILDER_H

#include <iostream>
#include <string_view>
#include <vector>
#include "QueryResultsTable.h"
#include "QueryObjects/QueryObject.h"
#include "DataAccessLayer.h"

/**
* This class builds the query by calling the PKB and splitting the query into the select clause, and non-select clauses
*/
using namespace std;

class QueryBuilder {
private:
	vector <shared_ptr<QueryObject>> selectClauseQueryObjects;
	vector <shared_ptr<QueryObject>> nonSelectClauseQueryObjects;
	shared_ptr<DataAccessLayer> dataAccessLayer;
	inline static bool isOptimisedSwitch = false;
	// Stores the count of the header in a group of clauses
	inline static map<string, int> countHeadersStore;
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

	bool getOptimisedSwitch() {
		return isOptimisedSwitch;
	}

	void setOptimisedSwitch() {
		isOptimisedSwitch = !isOptimisedSwitch;
	}

	static void updateCountHeaderStore(string header) {
		countHeadersStore[header]++;
	}

	static void resetCountHeaderStore() {
		countHeadersStore.clear();
	}

	int static getCount(string header) {
		return countHeadersStore[header];
	}
};
#endif
