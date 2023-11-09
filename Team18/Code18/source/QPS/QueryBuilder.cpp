#pragma once

#include "QueryBuilder.h"

#include "../ThreadPool.h"
#include "OptimisedSortingClauses.h"
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
      QueryResultsTable::createEmptyTable();

  shared_ptr<QueryOptimiser> queryOptimiser =
      make_shared<QueryOptimiser>(nonSelectClauseQueryObjects);
  queryOptimiser->groupQueryObjects();
  queryOptimiser->sortGroups();
  vector<shared_ptr<QueryGroup>> groups = queryOptimiser->getQueryGroups();
  cout << "number of distinct groups: " << groups.size() << endl;

  // evaluate each group (sequentially), in the order 0, 1, 2,..
  for (shared_ptr<QueryGroup> group : groups) {
    // print group
    cout << "groups (after sorting): " << endl;
    for (shared_ptr<QueryObject> obj : group->getClauses()) {
	  cout << obj->getQueryObjectName() << " ";
	}

    shared_ptr<QueryResultsTable> groupTable = buildGroupQuery(group);
    // if finalTable is empty, just set it to groupTable
    if (finalTable->isEmpty()) {
      finalTable = groupTable;
      cout << "finalTable: " << endl;
      finalTable->printTable();
      continue;
    }
    // no common synonyms. must cross product
    finalTable->crossProduct(groupTable);
    cout << "finalTable: " << endl;
    finalTable->printTable();
    if (finalTable->isEmpty() && !finalTable->getSignificant()) {
      return {QueryResultsTable::createEmptyTable()};
    }
  }

  return {finalTable};  // TODO: change to just return finalTable
}

shared_ptr<QueryResultsTable> QueryBuilder::buildGroupQuery(
    shared_ptr<QueryGroup> group) {
  shared_ptr<QRTCache> cache = make_shared<QRTCache>();
  shared_ptr<QueryResultsTable> groupTable = make_shared<QueryResultsTable>();
  vector<shared_ptr<QueryObject>> clauses = group->getClauses();

  // evaluation order: perform inner joins on the tables based on next
  // neighbour with smallest heuristic (basically, in the given order).
  // inner join table onto groupTable.
  // if inner join results in empty+insignificant groupTable, break out of the
  // for loop. this will terminate this group's evaluation, which also
  // terminates future group's evaluation

  // TODO: augmentation: after every group, remove all columns of synonyms not
  // in the select clause. dropColumns() then removeDuplicates().

  vector<bool> usedQueryObjects(clauses.size(), false);
  unordered_set<string> usedSynonyms;

  for (int i = 0; i < clauses.size(); i++) {
    // find the next obj with the smallest heuristic and common synonyms
    int nextObjIndex =
        findNextQueryObjectIndex(clauses, usedQueryObjects, usedSynonyms);
    shared_ptr<QueryObject> nextObj = clauses[nextObjIndex];

    usedQueryObjects[nextObjIndex] = true;
    // TODO: uncomment this!
    // usedSynonyms.insert(nextObj->getSynonyms().begin(),
    // nextObj->getSynonyms().end());

    cout << "query object chosen: " << nextObj->getQueryObjectName() << endl;

    // evaluate/retrieved cached table
    shared_ptr<QueryResultsTable> table;
    if (cache->contains(nextObj->getCacheName())) {
      table = cache->get(nextObj->getCacheName());
      continue;  // cache hit
    } else {
      table = nextObj->callAndProcess(dataAccessLayer);
    }
    if (nextObj->shouldCache()) {
      cache->insert(nextObj->getCacheName(), table);
    }

    cout << "table: " << endl;
    table->printTable();

    // break out of loop if table is empty. lazy evaluation
    if (table->isEmpty() && !table->getSignificant()) {
      return QueryResultsTable::createEmptyTable();
    }

    // if groupTable is empty, just set it to table
    if (groupTable->isEmpty()) {
      groupTable = table;
      cout << "groupTable: " << endl;
      groupTable->printTable();
      continue;
    }
    // TODO: this should be innerJoin only! allow crossproduct now just for
    // sanity testing
    if (groupTable->haveSimilarHeaders(table)) {
      // inner join, given the guarantee of common synonyms
      groupTable = groupTable->innerJoin(table);
    } else {
      // inner join, given the guarantee of common synonyms
      groupTable = groupTable->crossProduct(table);
    }

    cout << "groupTable: " << endl;
    groupTable->printTable();

    // break out of loop if groupTable is empty. lazy evaluation
    if (groupTable->isEmpty() && !groupTable->getSignificant()) {
      return QueryResultsTable::createEmptyTable();
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

  int noSynNext;
  for (int i = 0; i < clauses.size(); i++) {
    if (usedQueryObjects[i]) {
      continue;
    }
    noSynNext = i;
    // check if there are common synonyms with usedSynonyms
    shared_ptr<QueryObject> object = clauses[i];
    // TODO: uncomment this!
    // unordered_set<string> obj_synonyms = object->getSynonyms();
    unordered_set<string> obj_synonyms = {};
    for (string synonym : obj_synonyms) {
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
  }
  return queryResultsTables;
}
