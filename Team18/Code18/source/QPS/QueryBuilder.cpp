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

  cout << "Number of groups: " << groups.size() << endl;
  cout << "groups: " << endl;
  for (const auto& group : groups) {
    cout << "group: " << endl;
    for (const auto& clause : group->getClauses()) {
      cout << clause->getQueryObjectName() << endl;
    }
  }

  unordered_set<string> selectSynonyms;
  for (const auto& queryObject : selectClauseQueryObjects) {
    cout << "seelctqueryObject: " << queryObject->getQueryObjectName() << endl;
    auto synonyms = queryObject->getSynonyms();
    cout << "synonyms: in this selectqueryobj" << endl;
    cout << "num of syns: " << synonyms->size() << endl;
    for (const auto& synonym : *synonyms) {
      cout << synonym << endl;
    }
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
    cout << "groupSynonyms: " << endl;
    for (const auto& synonym : groupSynonyms) {
      cout << synonym << endl;
    }
    cout << "selectSynonyms: " << endl;
    for (const auto& synonym : selectSynonyms) {
      cout << synonym << endl;
    }
    cout << "hasRelevantSynonyms: " << hasRelevantSynonyms << endl;
    shared_ptr<QueryResultsTable> groupTable =
        buildGroupQuery(group, hasRelevantSynonyms);

    /*cout << "groupTable: " << endl;
    groupTable->printTable();*/

    // if groupTable is empty and insignificant, break out of loop.
    // lazy evaluation
    if (groupTable->isEmpty() && !groupTable->getSignificant()) {
      return {QueryResultsTable::createEmptyTable()};
    }

    // initialisation: if finalTable is empty, just set it to groupTable
    if (finalTable->isEmpty()) {
      finalTable = groupTable;
      continue;
    }
    cout << "starting cross product" << endl;
    // no common synonyms. must cross product
    finalTable = finalTable->crossProductSet(groupTable);
    if (finalTable->isEmpty() && !finalTable->getSignificant()) {
      return {QueryResultsTable::createEmptyTable()};
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
      return QueryResultsTable::createEmptyTable();
    }

    // if this group is has no relevant synonyms, we keep checking if it
    // becomes empty at any point in time.
    if (!hasRelevantSynonyms) {
      cout << "here!" << endl;
      if (table->isEmpty() && !table->getSignificant()) {
        // can early terminate. the whole query will be false
        return QueryResultsTable::createEmptyTable(false);
      } else {
        cout << "here!" << endl;
        // significant. cannot early terminate.
        // need to complete evaluation of this group
        // before we can determine if the whole query is false
        // But, whether empty or not, rows are irrelevant. just set as empty
        table = QueryResultsTable::createEmptyTable(true);
      }
    }
    cout << "groupTable: " << endl;
    groupTable->printTable();
    cout << "table: " << endl;
    table->printTable();
    groupTable = groupTable->innerJoinSet(table);

    // if groupTable is empty, just set it to table. initialise
    if (groupTable->isEmpty()) {
      groupTable = table;
      continue;
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
