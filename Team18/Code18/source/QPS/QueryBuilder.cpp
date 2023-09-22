#pragma once

#include "QueryBuilder.h"
#include "QueryObjects/DesignObjects.h"
#include "QueryObjects/ClauseObject.h"
#include "QueryObjects/PatternClauseObject.h"
using namespace std;

//Takes in a Query object and then returns a vector of QueryResultsTable. This vector will
//be pased to ResultHandler to process the various tables of the clauses

vector<shared_ptr<QueryResultsTable>> QueryBuilder::buildQuery() {

	vector<shared_ptr<QueryResultsTable>> queryResultsTables;
	for (shared_ptr<QueryObject> obj : queryObjects) {
		shared_ptr<QueryResultsTable> table = obj->callAndProcess(dataAccessLayer, synonyms);
		queryResultsTables.push_back(table);
	}
	return queryResultsTables;
}
