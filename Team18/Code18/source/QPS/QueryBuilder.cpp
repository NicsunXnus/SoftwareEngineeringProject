#pragma once

#include "QueryBuilder.h"
#include "QueryObjects/DesignObjects.h"
#include "QueryObjects/ClauseObject.h"
#include "QueryObjects/PatternClauseObject.h"
using namespace std;

// A set to contain only unique select clauses arguments.
set<string> getHeadersOfTableAsSet(vector<shared_ptr<QueryResultsTable>> selectClauseTables) {
	set<string> result;
	for (shared_ptr<QueryResultsTable> table : selectClauseTables) {
		result.insert(table->getHeaders()[0]);
	}
	return result;
}

// An auxiliary function that aids in the comparison of QueryResultsTable(s) in a vector.
// Empty tables are given higher priority and sorted before non-empty tables.
bool sortEmptyFirst(const std::shared_ptr<QueryResultsTable>& a, const std::shared_ptr<QueryResultsTable>& b) {
	if (b->isEmpty() && !a->isEmpty())
		return false;
	return true;
}

// An auxiliary function to aid in the comparison within the data structure of QueryResultsTable(s) in a vector.
// The table containing the more common header in the group of clauses will be sorted before the other.
bool sortMostCommonHeaderFirst(const std::shared_ptr<QueryResultsTable>& a, const std::shared_ptr<QueryResultsTable>& b) {
	int maxValueA = 0;
	int maxValueB = 0;
	vector<string> headersA = a->getHeaders();
	vector<string> headersB = b->getHeaders();
	for (string header : headersA) {
		if (QueryBuilder::getCount(header) > maxValueA) {
			maxValueA = QueryBuilder::getCount(header);
		}
	}
	for (string header : headersB) {
		if (QueryBuilder::getCount(header) > maxValueB) {
			maxValueB = QueryBuilder::getCount(header);
		}
	}
	return maxValueA > maxValueB;
}

// An auxiliary function to flatten vector< vector<shared_ptr<QueryResultsTable>> >
vector<shared_ptr<QueryResultsTable>> flatten2DArray(vector< vector<shared_ptr<QueryResultsTable>> > v2d) {
	vector<shared_ptr<QueryResultsTable>> v1d;
	for (const auto& row : v2d) {
		v1d.insert(v1d.end(), row.begin(), row.end());
	}
	return v1d;
}

// 1. Removes columns that the select clauses do not ask for
// 2. Add all empty tables to the beginning
//void optimiseStepA(vector<shared_ptr<QueryResultsTable>> selectClauseTables, vector<shared_ptr<QueryResultsTable>>& nonSelectClauseTables) {
void optimiseStepA(vector<shared_ptr<QueryResultsTable>>& nonSelectClauseTables) {
	/*set<string> selectClauses = getHeadersOfTableAsSet(selectClauseTables);
	for (shared_ptr<QueryResultsTable> table : nonSelectClauseTables) {
		if (!table->isEmpty()) {
			vector< map<string, vector<string>> > columns = table->getColumns();
			vector< map<string, vector<string>> > result;
			for (map<string, vector<string>> column : columns) {
				if (find(selectClauses.begin(), selectClauses.end(), column.begin()->first) != selectClauses.end()) {
					result.emplace_back(column);
				}
			}
			table->setColumns(result);
		}
	}*/
	//End of step 1
	sort(nonSelectClauseTables.begin(), nonSelectClauseTables.end(), sortEmptyFirst);
	//End of step 2			
}

// Rearrange the clauses in the group such that the clauses with the most common headers are arranged at the front.
// This means a higher chance of "mutual friends" already forming at the beginning, so more likely that an inner join
// will occur than a cross product.
void optimiseStepC(vector< vector<shared_ptr<QueryResultsTable>> >& groups) {
	for (vector<shared_ptr<QueryResultsTable>>& group : groups) {
		if (group.size() == 0 || group[0]->isEmpty()) continue; //skip for empty table groups
		for (shared_ptr<QueryResultsTable> table : group) {
			vector<string> headers = table->getHeaders();
			for (string header : headers) {
				QueryBuilder::updateCountHeaderStore(header);
			}
		}
		sort(group.begin(), group.end(), sortMostCommonHeaderFirst);
		QueryBuilder::resetCountHeaderStore();
	}
}

// Group the clauses
// A group of clauses is guaranteed to have a continuous link between all clauses.
// However the order of the clauses is not guaranteed such that any two neighboring clauses
// will share a common header.
void optimiseStepB(vector<shared_ptr<QueryResultsTable>>& nonSelectClauseTables) {
	vector< vector<shared_ptr<QueryResultsTable>> > groups;
	vector<shared_ptr<QueryResultsTable>> emptyTables;
	int indexNonEmpty = 0;
	for (shared_ptr<QueryResultsTable> table : nonSelectClauseTables) {
		if (table->isEmpty()) emptyTables.emplace_back(table);
		else break;
		indexNonEmpty++;
	}
	groups.emplace_back(emptyTables);
	vector<shared_ptr<QueryResultsTable>> nonEmptyTables(nonSelectClauseTables.begin() + indexNonEmpty, nonSelectClauseTables.end());
	if (nonEmptyTables.size() > 1) {
		// sort the most table with the most common headers at the start
		for (shared_ptr<QueryResultsTable> table : nonEmptyTables) {
			vector<string> headers = table->getHeaders();
			for (string header : headers) {
				QueryBuilder::updateCountHeaderStore(header);
			}
		}
		sort(nonEmptyTables.begin(), nonEmptyTables.end(), sortMostCommonHeaderFirst);
		QueryBuilder::resetCountHeaderStore();

		vector<string> headers = nonEmptyTables[0]->getHeaders();

		set<string> firstHeaders;
		copy(headers.begin(), headers.end(), inserter(firstHeaders, firstHeaders.begin()));

		vector<set<string> > groupNames;
		groupNames.emplace_back(firstHeaders);

		vector<shared_ptr<QueryResultsTable>> newGroup;
		newGroup.emplace_back(nonEmptyTables[0]);
		groups.emplace_back(newGroup);
		nonEmptyTables.erase(nonEmptyTables.begin());

		for (shared_ptr<QueryResultsTable> table : nonEmptyTables) {
			vector<string> thisHeaders = table->getHeaders();
			int index = 1; //starts from 1 because 0 is for the emptyTables group
			bool isFound = false;
			for (set<string>& group : groupNames) {
				set<string> intersection;
				set_intersection(group.begin(), group.end(), thisHeaders.begin(), thisHeaders.end(),
					inserter(intersection, intersection.begin()));
				if (intersection.size() > 0) {
					groups[index].emplace_back(table);
					std::set_union(thisHeaders.cbegin(), thisHeaders.cend(),
						group.cbegin(), group.cend(),
						inserter(group, group.begin()));
					isFound = true;
					break;
				}
				index++;
			}
			if (!isFound) {
				set<string> newHeaders;
				copy(thisHeaders.begin(), thisHeaders.end(), inserter(newHeaders, newHeaders.begin()));
				groupNames.emplace_back(newHeaders);
				vector<shared_ptr<QueryResultsTable>> group;
				group.emplace_back(table);
				groups.emplace_back(group);
			}
		}
	}
	else {
		groups.emplace_back(nonEmptyTables);
	}
	optimiseStepC(groups);
	nonSelectClauseTables = flatten2DArray(groups);
}

//Takes in a vector of Query objects and then returns a vector of QueryResultsTable. This vector will
//be passed to ResultHandler to process (inner join etc) the various tables of the clauses
// In the future, optimization of queries, sorting etc will be done here before passing to results handler

vector<shared_ptr<QueryResultsTable>> QueryBuilder::buildQuery() {

	vector<shared_ptr<QueryResultsTable>> queryResultsTables;

	for (shared_ptr<QueryObject> obj : nonSelectClauseQueryObjects) {
		shared_ptr<QueryResultsTable> table = obj->callAndProcess(dataAccessLayer);
		queryResultsTables.push_back(table);
	}
	//// for optimization in future, sort here
	// Activate Optimisation
	QueryBuilder::setOptimisedSwitch();
	if (QueryBuilder::getOptimisedSwitch()) {
		//optimiseStepA(queryResultsTables);
		//optimiseStepB(queryResultsTables);
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
