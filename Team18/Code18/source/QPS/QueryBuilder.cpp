#pragma once

#include "QueryBuilder.h"
#include "QueryObjects/DesignObjects.h"
#include "QueryObjects/ClauseObject.h"
#include "QueryObjects/PatternClauseObject.h"
#include "OptimisedSortingClauses.h"
#include "../ThreadPool.h"
#include "QRTCache.h"
//Takes in a vector of Query objects and then returns a vector of QueryResultsTable. This vector will
//be passed to ResultHandler to process (inner join etc) the various tables of the clauses
// In the future, optimization of queries, sorting etc will be done here before passing to results handler

// tables must be non-empty
shared_ptr<QueryResultsTable> joinIntermediateTables(vector<shared_ptr<QueryResultsTable>> tables) {
	shared_ptr<QueryResultsTable> intermediateTable = tables[0];
	tables.erase(tables.begin());
	shared_ptr<QueryResultsTable> currTable;
	for (shared_ptr<QueryResultsTable> table : tables) {
		currTable = table;
		// for empty but significant tables
		if (currTable->isEmpty()) {
			if (currTable->getSignificant()) {
				continue; // just keep current table
			}
			else { // no need to evaluate the rest of the query
				return QueryResultsTable::createEmptyTable();
			}

		}
		if (intermediateTable->haveSimilarHeaders(currTable)) {
			//do inner join
			intermediateTable = intermediateTable->innerJoinSet(currTable);
		}
		else {
			//do cross product
			intermediateTable = intermediateTable->crossProductSet(currTable);
		}
	}
	return intermediateTable;
}

shared_ptr<QueryResultsTable> merge(vector<shared_ptr<QueryResultsTable>>::iterator left, vector<shared_ptr<QueryResultsTable>>::iterator right)
{
	if (left == right) return 0;
	if (left + 1 == right) return *left;
	int midPosition = (right - left) / 2;
	vector<shared_ptr<QueryResultsTable>>::iterator mid = left + midPosition;
	shared_ptr<QueryResultsTable> tab1 = merge(left, mid);
	shared_ptr<QueryResultsTable> tab2 = merge(mid, right);
	return joinIntermediateTables({ tab1, tab2 });
}

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

	ThreadPool outerPool;



	// Activate Optimisation
	//QueryBuilder::setOptimisedSwitch();
	if (QueryBuilder::getOptimisedSwitch()) { // Trigger sorting of clauses
		//moveEmptyTablesToFront(queryResultsTables);
		//vector<shared_ptr<GroupClause>> groups = groupSimilarTables(queryResultsTables);
		//mergeSimilarGroups(groups);
		//optimiseTablePositions(groups);
		/*ThreadPool pool;
		for (shared_ptr<GroupClause> group : groups) {
			pool.addTask(&GroupClause::reduceToOne, group);
		}
		pool.wait();
		queryResultsTables = revert1DTables(groups);*/
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
