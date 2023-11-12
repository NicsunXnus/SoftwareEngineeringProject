#pragma once

#include "QueryBuilder.h"

#include "../ThreadPool.h"
#include "QRTCache.h"
#include "QueryObjects/ClauseObject.h"
#include "QueryObjects/DesignObjects.h"
#include "QueryObjects/PatternClauseObject.h"
#include "QueryOptimiser.h"

// Takes in a vector of Query objects and then returns a vector of
// QueryResultsTable. This vector will be passed to ResultHandler to process
// (inner join etc) the various tables of the clauses

vector<shared_ptr<QueryResultsTable>> QueryBuilder::buildQuery() {
  shared_ptr<QueryResultsTable> finalTable =
      QueryResultsTable::createEmptyTable(false);

  if (nonSelectClauseQueryObjects.empty()) return {};

  shared_ptr<QueryOptimiser> queryOptimiser =
      make_shared<QueryOptimiser>(nonSelectClauseQueryObjects);
  queryOptimiser->groupQueryObjects();
  queryOptimiser->sortGroups();
  vector<shared_ptr<QueryGroup>> groups = queryOptimiser->getQueryGroups();

  unordered_set<string> selectSynonyms;
  for (const auto& queryObject : selectClauseQueryObjects) {
    auto synonyms = queryObject->getSynonyms();
    selectSynonyms.insert(synonyms->begin(), synonyms->end());
  }

  //  evaluate each group (sequentially), in the order 0, 1, 2,..
  for (shared_ptr<QueryGroup> group : groups) {
    // check if a group has relevant synonyms
    unordered_set<string> groupSynonyms;
    for (const auto& queryObject : group->getClauses()) {
      auto synonyms = queryObject->getSynonyms();
      groupSynonyms.insert(synonyms->begin(), synonyms->end());
    }
    bool hasRelevantSynonyms = has_intersection(selectSynonyms, groupSynonyms);
    shared_ptr<QueryResultsTable> groupTable =
        buildGroupQuery(group, hasRelevantSynonyms);

    // if groupTable is empty and insignificant, break out of loop.
    // lazy evaluation
    if (groupTable->isEmpty() && !groupTable->getSignificant()) {
      return {QueryResultsTable::createEmptyTable(false)};
    }

    // if this group is has no relevant synonyms, just return empty table.
    // significant if groupTable non-empty, vice-versa.
    if (!hasRelevantSynonyms) {
      if (groupTable->isEmpty() && !groupTable->getSignificant()) {
        return {QueryResultsTable::createEmptyTable(false)};
      } else if (groupTable->isEmpty() && groupTable->getSignificant()) {
        groupTable = QueryResultsTable::createEmptyTable(true);
      } else {
        groupTable = QueryResultsTable::createEmptyTable(true);
      }
    }

    // initialisation: if finalTable is empty, just set it to groupTable
    if (finalTable->isEmpty()) {
      finalTable = groupTable;
    } else {
      // no common synonyms. must cross product
      finalTable = finalTable->crossProductSet(groupTable);
    }
  }

  return {finalTable};
}

shared_ptr<QueryResultsTable> QueryBuilder::buildGroupQuery(
    shared_ptr<QueryGroup> group, bool hasRelevantSynonyms) {
  shared_ptr<QRTCache> cache = make_shared<QRTCache>();
  shared_ptr<QueryResultsTable> groupTable =
      QueryResultsTable::createEmptyTable(false);
  vector<shared_ptr<QueryObject>> clauses = group->getClauses();

  // evaluation order: perform inner joins on the tables based on next
  // neighbour with smallest heuristic (basically, in the given order).
  // inner join table onto groupTable.
  // if inner join results in empty+insignificant groupTable, break out of the
  // for loop. this will terminate this group's evaluation, which also
  // terminates future group's evaluation

  vector<bool> usedQueryObjects(clauses.size(), false);
  unordered_set<string> usedSynonyms;

  for (int i = 0; i < clauses.size(); i++) {
    // find the next obj with the smallest heuristic and common synonyms
    int nextObjIndex =
        findNextQueryObjectIndex(clauses, usedQueryObjects, usedSynonyms);
    shared_ptr<QueryObject> nextObj = clauses[nextObjIndex];

    usedQueryObjects[nextObjIndex] = true;
    shared_ptr<unordered_set<string>> thisSynonyms = nextObj->getSynonyms();
    usedSynonyms.insert(thisSynonyms->begin(), thisSynonyms->end());

    // evaluate/retrieved cached table
    shared_ptr<QueryResultsTable> table;
    if (cache->contains(nextObj->getCacheName())) {
      table = cache->get(nextObj->getCacheName());
    } else {
      table = nextObj->callAndProcess(dataAccessLayer);
    }
    if (nextObj->shouldCache()) {
      cache->insert(nextObj->getCacheName(), table);
    }
    // break out of loop if table is empty. lazy evaluation
    if (table->isEmpty() && !table->getSignificant()) {
      return QueryResultsTable::createEmptyTable(false);
    }

    // if groupTable is empty, just set it to table. initialise
    if (groupTable->isEmpty()) {
      groupTable = table;
    } else {
      groupTable = groupTable->innerJoinSet(table);
    }

    // break out of loop if groupTable is empty. lazy evaluation
    if (groupTable->isEmpty() && !groupTable->getSignificant()) {
      return QueryResultsTable::createEmptyTable(false);
    }
  }
  return groupTable;
}

// choose the next obj in group with smallest heuristic
// (which group should already be ordered in),
// and also common synonyms with our current groupTable
int QueryBuilder::findNextQueryObjectIndex(
    vector<shared_ptr<QueryObject>> clauses, vector<bool> usedQueryObjects,
    unordered_set<string> usedSynonyms) {
  int noSynNext = -1;
  for (int i = 0; i < clauses.size(); i++) {
    if (usedQueryObjects[i]) {
      continue;
    }
    if (noSynNext == -1) noSynNext = i;
    // check if there are common synonyms with usedSynonyms
    shared_ptr<QueryObject> object = clauses[i];
    shared_ptr<unordered_set<string>> obj_synonyms = object->getSynonyms();
    for (string synonym : *obj_synonyms) {
      if (usedSynonyms.count(synonym)) {  // found common synonym
        return i;                         // optimalNext
      }
    }
  }
  // in case no clauses left with common synonyms. ideally should not happen.
  return noSynNext;
}

vector<shared_ptr<QueryResultsTable>> QueryBuilder::buildQuerySelectClause() {
  vector<shared_ptr<QueryResultsTable>> queryResultsTables;
  for (shared_ptr<QueryObject> obj : selectClauseQueryObjects) {
    shared_ptr<QueryResultsTable> table = obj->callAndProcess(dataAccessLayer);
    queryResultsTables.push_back(table);
    unordered_set<string> headers = table->getHeaders();
  }
  return queryResultsTables;
}
