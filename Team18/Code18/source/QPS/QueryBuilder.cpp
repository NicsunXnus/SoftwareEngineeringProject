#pragma once

#include "QueryBuilder.h"

#include "../ThreadPool.h"
#include "OptimisedSortingClauses.h"
#include "QRTCache.h"
#include "QueryObjects/ClauseObject.h"
#include "QueryObjects/DesignObjects.h"
#include "QueryObjects/PatternClauseObject.h"

// Takes in a vector of Query objects and then returns a vector of
// QueryResultsTable. This vector will be passed to ResultHandler to process
// (inner join etc) the various tables of the clauses
//  In the future, optimization of queries, sorting etc will be done here before
//  passing to results handler

vector<shared_ptr<QueryResultsTable>> QueryBuilder::buildQuery() {
  shared_ptr<QRTCache> cache = make_shared<QRTCache>();

  vector<shared_ptr<QueryResultsTable>> queryResultsTables;

  for (shared_ptr<QueryObject> obj : nonSelectClauseQueryObjects) {
    if (cache->contains(obj->getCacheName())) {
      queryResultsTables.push_back(cache->get(obj->getCacheName()));
      continue;  // cache hit
    }
    shared_ptr<QueryResultsTable> table = obj->callAndProcess(dataAccessLayer);
    table = table->removeDuplicates();
    queryResultsTables.push_back(table);
    if (obj->shouldCache()) {
      cache->insert(obj->getCacheName(), table);
    }
  }

  // Activate Optimisation
  QueryBuilder::setOptimisedSwitch();
  if (QueryBuilder::getOptimisedSwitch()) {  // Trigger sorting of clauses
    moveEmptyTablesToFront(queryResultsTables);
    vector<shared_ptr<GroupClause>> groups =
        groupSimilarTables(queryResultsTables);
    mergeSimilarGroups(groups);
    optimiseTablePositions(groups);
    ThreadPool pool;
    for (shared_ptr<GroupClause> group : groups) {
      pool.addTask(&GroupClause::reduceToOne, group);
    }
    pool.wait();
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
