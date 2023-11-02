#ifndef OPTIMISEDSORTINGCLAUSES_H
#define OPTIMISEDSORTINGCLAUSES_H

#include <vector>
#include "GroupClause.h"
#include "QueryResultsTable.h"
#include <ppl.h>
using namespace concurrency;
/**
* An auxiliary class to contain and provide the functions to QueryBuilder for the sorting of the clauses into groups
* for a more efficient processing of the results of the clauses. No contents of the clauses will be modified, only the order.
* The functions used in optimising are prefixed by "optimise" and are designed to be used in their alphabetical order.
* The functions,by the user's needs, can also be used in any order.
*/

bool sortEmptyFirst(const std::shared_ptr<QueryResultsTable>& a, const std::shared_ptr<QueryResultsTable>& b) {
	int priorityA = -1;
	int priorityB = 0;
	if (a->isEmpty() && !b->isEmpty()) {
		priorityA = 1;
	}
	return priorityA > priorityB;
}

bool sortMostUniqueHeadersFirst(const std::shared_ptr<QueryResultsTable>& a, const std::shared_ptr<QueryResultsTable>& b) {
	set<string> setA = a->getHeadersAsSet(); set<string> setB = b->getHeadersAsSet();
	return setA.size() > setB.size();
}

// An auxiliary function to revert a vector of GroupClause back to a vector of QueryResultsTable
vector<shared_ptr<QueryResultsTable>> revert1DTables(vector< shared_ptr<GroupClause> > groups) {
	vector<shared_ptr<QueryResultsTable>> v1d;
	for (shared_ptr<GroupClause> group : groups) {
		vector<shared_ptr<QueryResultsTable>> tables = group->getMembers();
		v1d.insert(v1d.end(), tables.begin(), tables.end());
	}
	return v1d;
}

// 1. Removes columns that the select clauses do not ask for
// 2. Add all empty tables to the beginning
void moveEmptyTablesToFront(vector<shared_ptr<QueryResultsTable>>& nonSelectClauseTables) {
	parallel_buffered_sort(nonSelectClauseTables.begin(), nonSelectClauseTables.end(), sortEmptyFirst);		
}

// Group the clauses. A group of clauses is guaranteed to have a continuous link between all clauses
// However the order of the clauses is not guaranteed such that any two neighboring clauses
// will share a common header.
vector<shared_ptr<GroupClause>> groupSimilarTables(vector<shared_ptr<QueryResultsTable>>& nonSelectClauseTables) {
	vector <shared_ptr<GroupClause>> groups;
	shared_ptr<GroupClause> emptyTables = make_shared<GroupClause>();
	int indexNonEmpty = 0;
	for (shared_ptr<QueryResultsTable> table : nonSelectClauseTables) {
		if (table->isEmpty()) emptyTables->addOne(table);
		else break;
		indexNonEmpty++;
	}
	vector<shared_ptr<QueryResultsTable>> nonEmptyTables;
	if (emptyTables->size() > 0) {
		nonEmptyTables.insert(nonEmptyTables.begin(), nonSelectClauseTables.begin() + indexNonEmpty, nonSelectClauseTables.end());
		groups.emplace_back(emptyTables);
	}
	else {
		nonEmptyTables = nonSelectClauseTables;
	}
	if (nonEmptyTables.size() <= 2) { // trivial if there is not more than 2 tables
		shared_ptr<GroupClause> lessThanTwoNonEmptyTables = make_shared<GroupClause>();
		lessThanTwoNonEmptyTables->addMany(nonEmptyTables);
		groups.emplace_back(lessThanTwoNonEmptyTables);
		return groups;
	}
	parallel_buffered_sort(nonEmptyTables.begin(), nonEmptyTables.end(), sortMostUniqueHeadersFirst);
	shared_ptr<GroupClause> newGroup = make_shared<GroupClause>();
	newGroup->addOne(nonEmptyTables[0]);
	groups.emplace_back(newGroup);
	nonEmptyTables.erase(nonEmptyTables.begin());
	int startIndex = 0;
	if (emptyTables->size() > 0) startIndex = 1;
	for (shared_ptr<QueryResultsTable> table : nonEmptyTables) {
		set<string> thisHeaders = table->getHeadersAsSet();
		int index = startIndex; //starts from 1 because 0 is for the emptyTables group
		bool isFound = false;
		for (shared_ptr<GroupClause> group : groups) {
			if (group->hasEmptyTablesFirst()) continue;
			set<string> intersection;
			set<string> groupHeaders = group->getHeaders();
			set_intersection(groupHeaders.begin(), groupHeaders.end(), thisHeaders.begin(), thisHeaders.end(),
				inserter(intersection, intersection.begin()));
			if (intersection.size() > 0) {
				groups[index]->addOne(table);
				isFound = true;
				break;
			}
			index++;
		}
		if (!isFound) {
			shared_ptr<GroupClause> group = make_shared<GroupClause>();
			group->addOne(table);
			groups.emplace_back(group);
		}
	}
	return groups;
}

// Goes through each group and merge groups that shares same headers
void mergeSimilarGroups(vector< shared_ptr<GroupClause> >& groups) {
	if (groups.size() == 0) return;
	if (groups[0]->hasEmptyTablesFirst() && groups.size() < 3) return;
	if (!groups[0]->hasEmptyTablesFirst() && groups.size() < 2) return;
	int gap = 1;
	int groupSize = groups.size();
	int startIndex = 0;
	if (groups[0]->hasEmptyTablesFirst()) startIndex = 1;
	for (int i = startIndex; i < groupSize; i++) {
		for (int j = i + 1; j < groupSize; j++) {
			if (groups[i]->hasCommonHeaders(groups[j])) {
				groups[i]->merge(groups[j]);
				groups.erase(groups.begin() + j);
				groupSize--;
				i--;
				break;
			}
		}
		i++;
	}
}

void checkForUmbrellaHeadersPresence(vector<shared_ptr<QueryResultsTable>> groupMembers, set<string>& headerContainer, set<string> groupHeaders) {
	for (int i = 1; i < groupMembers.size(); i++) {
		set<string> intersect;
		set<string> thisHeaders = groupMembers[i]->getHeadersAsSet();
		set_intersection(headerContainer.begin(), headerContainer.end(), thisHeaders.begin(), thisHeaders.end(), inserter(intersect, intersect.begin()));
		if (intersect.size() > 0) {
			headerContainer.insert(thisHeaders.begin(), thisHeaders.end());
			if (headerContainer == groupHeaders) break;
		}
		else {
			break;
		}
	}
}

void tableReshuffleToCreateUmbrellasHeader(vector<shared_ptr<QueryResultsTable>>& groupMembers, set<string> groupHeaders) {
	int gap = 1;
	set<string> headerContainer;
	for (int j = 0; j < groupMembers.size(); j++) { // Iterate the same number of times as the number of members unless a table is found to have no shared header
		shared_ptr<QueryResultsTable> currTable = groupMembers[0];
		set<string> currTableHeaders = currTable->getHeadersAsSet();
		copy(currTableHeaders.begin(), currTableHeaders.end(), inserter(headerContainer, headerContainer.begin()));
		if (headerContainer == groupHeaders) break;
		int maximumNonIntersection = -1;
		int minimumIntersectIndex = -1;
		for (int k = j + gap; k < groupMembers.size(); k++) {
			if (!currTable->haveSimilarHeaders(groupMembers[k])) continue;
			int nonIntersectSize = currTable->differenceInHeaders(groupMembers[k]);
			if (maximumNonIntersection < 0) {
				maximumNonIntersection = nonIntersectSize;
				minimumIntersectIndex = k;
				continue;
			}
			if (nonIntersectSize > maximumNonIntersection) {
				maximumNonIntersection = nonIntersectSize;
				minimumIntersectIndex = k;
			}
		}
		gap++;
		if (minimumIntersectIndex < 0) return;
		shared_ptr<QueryResultsTable> temp = groupMembers[minimumIntersectIndex];
		groupMembers.erase(groupMembers.begin() + minimumIntersectIndex);
		groupMembers.insert(groupMembers.begin(), temp);
	}
}

// For every group, this function attempts to sort the tables at the front such that not only do they create a 
// link within themselves (allows inner join) but also a union of them will cover the whole group, which will
// effectively enable inner join for the whole group.
void optimiseTablePositions(vector<shared_ptr<GroupClause>>& groups) {
	if (groups.size() == 0) return;
	int startIndex = 0;
	if (groups[0]->hasEmptyTablesFirst()) startIndex = 1; // start from 1 given that the first group is the emptyTables group
	for (int i = startIndex; i < groups.size(); i++) { // Iterate for each group
		if (groups[i]->size() <= 2) continue; // In a group of not more than 2, each table is guaranteed to share a header with a neighbor
		set<string> groupHeaders = groups[i]->getHeaders();
		vector<shared_ptr<QueryResultsTable>> currGroupMembers = groups[i]->getMembers();
		set<string> currTableHeaders = currGroupMembers[0]->getHeadersAsSet();
		if (currTableHeaders == groupHeaders) continue; // Means that the first table already has headers that covers the whole group
		checkForUmbrellaHeadersPresence(currGroupMembers, currTableHeaders, groupHeaders); // Checks for a possible link between first table and some table that covers the whole group
		if (currTableHeaders == groupHeaders) continue; // Means there is a link from the first table to some table in a later row that covers the whole group
		tableReshuffleToCreateUmbrellasHeader(currGroupMembers, groupHeaders);
		groups[i]->setMembers(currGroupMembers);
	}
}

#endif
