#pragma once

#include "QueryBuilder.h"
#include "QueryObjects/DesignObjects.h"
#include "QueryObjects/ClauseObject.h"
#include "QueryObjects/PatternClauseObject.h"
#include "OptimisedSortingClauses.h"
#include "QRTCache.h"
//Takes in a vector of Query objects and then returns a vector of QueryResultsTable. This vector will
//be passed to ResultHandler to process (inner join etc) the various tables of the clauses
// In the future, optimization of queries, sorting etc will be done here before passing to results handler

vector<shared_ptr<QueryResultsTable>> QueryBuilder::buildQuery() {

	shared_ptr<QRTCache> cache = make_shared<QRTCache>();

	vector<shared_ptr<QueryResultsTable>> queryResultsTables;

	for (shared_ptr<QueryObject> obj : nonSelectClauseQueryObjects) {
		if (cache->contains(obj->getCacheName())) {
			queryResultsTables.push_back(cache->get(obj->getCacheName()));
			continue; // cache hit
		}
		shared_ptr<QueryResultsTable> table = obj->callAndProcess(dataAccessLayer);
		queryResultsTables.push_back(table);
		if (obj->shouldCache()) {
			cache->insert(obj->getCacheName(), table);
		}
	}
	//// for optimization in future, sort here
	// Activate Optimisation
	QueryBuilder::setOptimisedSwitch();
	if (QueryBuilder::getOptimisedSwitch()) {
		optimiseStepA(queryResultsTables);
		optimiseStepB(queryResultsTables);
	}
	return queryResultsTables;
}

vector<shared_ptr<QueryResultsTable>> QueryBuilder::buildQuerySelectClause() {

	vector<shared_ptr<QueryResultsTable>> queryResultsTables;
	for (shared_ptr<QueryObject> obj : selectClauseQueryObjects) {
		shared_ptr<QueryResultsTable> table = obj->callAndProcess(dataAccessLayer);
		queryResultsTables.push_back(table);
	}
	return queryResultsTables;
}
