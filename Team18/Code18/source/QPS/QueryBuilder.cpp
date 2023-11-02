#pragma once

#include "QueryBuilder.h"
#include "QueryObjects/DesignObjects.h"
#include "QueryObjects/ClauseObject.h"
#include "QueryObjects/PatternClauseObject.h"
#include "OptimisedSortingClauses.h"
//Takes in a vector of Query objects and then returns a vector of QueryResultsTable. This vector will
//be passed to ResultHandler to process (inner join etc) the various tables of the clauses
// In the future, optimization of queries, sorting etc will be done here before passing to results handler

vector<shared_ptr<QueryResultsTable>> QueryBuilder::buildQuery() {

	vector<shared_ptr<QueryResultsTable>> queryResultsTables;

	for (shared_ptr<QueryObject> obj : nonSelectClauseQueryObjects) {
		shared_ptr<QueryResultsTable> table = obj->callAndProcess(dataAccessLayer);
		queryResultsTables.push_back(table);
	}

	// Activate Optimisation
	QueryBuilder::setOptimisedSwitch();
	if (QueryBuilder::getOptimisedSwitch()) { // Trigger sorting of clauses
		moveEmptyTablesToFront(queryResultsTables);
		vector<shared_ptr<GroupClause>> groups = groupSimilarTables(queryResultsTables);
		mergeSimilarGroups(groups);
		optimiseTablePositions(groups);
		queryResultsTables = revert1DTables(groups);
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
