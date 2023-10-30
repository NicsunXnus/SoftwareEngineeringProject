#ifndef OPTIMISEDSORTINGCLAUSES_H
#define OPTIMISEDSORTINGCLAUSES_H

#include <vector>
#include "QueryResultsTable.h"

/**
* An auxiliary class to contain and provide the functions to QueryBuilder for the sorting of the clauses into groups
* for a more efficient processing of the results of the clauses.
*/
using namespace std;

// Stores the count of the header in a group of clauses
map<string, int> countHeadersStore;

void updateCountHeaderStore(string header) {
	countHeadersStore[header]++;
}

void resetCountHeaderStore() {
	countHeadersStore.clear();
}

int getCount(string header) {
	return countHeadersStore[header];
}

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
	int priorityA = -1;
	int priorityB = 0;
	if (a->isEmpty() && !b->isEmpty()) {
		priorityA = 1;
	}
	return priorityA > priorityB;
}

// An auxiliary function to aid in the comparison within the data structure of QueryResultsTable(s) in a vector.
// The table containing the more common header in the group of clauses will be sorted before the other.
bool sortMostCommonHeaderFirst(const std::shared_ptr<QueryResultsTable>& a, const std::shared_ptr<QueryResultsTable>& b) {
	int maxValueA = 0;
	int maxValueB = 0;
	vector<string> headersA = a->getHeaders();
	vector<string> headersB = b->getHeaders();
	for (string header : headersA) {
		if (getCount(header) > maxValueA) {
			maxValueA = getCount(header);
		}
	}
	for (string header : headersB) {
		if (getCount(header) > maxValueB) {
			maxValueB = getCount(header);
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

// An auxiliary function to sort the tables in a vector by the most common header
void sortVectorOfTablesByHeader(vector<shared_ptr<QueryResultsTable>>& tables) {
	for (shared_ptr<QueryResultsTable> table : tables) {
		vector<string> headers = table->getHeaders();
		for (string header : headers) {
			updateCountHeaderStore(header);
		}
	}
	sort(tables.begin(), tables.end(), sortMostCommonHeaderFirst);
	resetCountHeaderStore();
}

// Add all empty tables to the beginning
void optimiseStepA(vector<shared_ptr<QueryResultsTable>>& nonSelectClauseTables) {
	sort(nonSelectClauseTables.begin(), nonSelectClauseTables.end(), sortEmptyFirst);
}

//Declaration to let the function be used in optimisedStepB
void optimiseStepC(vector< vector<shared_ptr<QueryResultsTable>> >& groups);

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
	if (nonEmptyTables.size() <= 1) {
		groups.emplace_back(nonEmptyTables);
		optimiseStepC(groups);
		nonSelectClauseTables = flatten2DArray(groups);
		return;
	}

	// sort the most table with the most common headers at the start
	sortVectorOfTablesByHeader(nonEmptyTables);

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

	optimiseStepC(groups);
	nonSelectClauseTables = flatten2DArray(groups);
}

// Rearrange the clauses in the group such that the clauses with the most common headers are arranged at the front.
// This means a higher chance of "mutual friends" already forming at the beginning, so more likely that an inner join
// will occur than a cross product.
void optimiseStepC(vector< vector<shared_ptr<QueryResultsTable>> >& groups) {
	for (vector<shared_ptr<QueryResultsTable>>& group : groups) {
		if (group.size() <= 1 || group[0]->isEmpty()) continue; //skip for empty table groups  and if group size is not greater than 1
		sortVectorOfTablesByHeader(group);
	}
}

#endif
