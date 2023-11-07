#include "OptimisedFunctionsStub.h"
#include <execution>
bool sortEmptyFirstStub(const shared_ptr<QueryResultsTable>& a, const shared_ptr<QueryResultsTable>& b) {
	int priorityA = -1;
	int priorityB = 0;
	if (a->isEmpty() && !b->isEmpty()) {
		priorityA = 1;
	}
	return priorityA > priorityB;
}

bool OptimisedFunctionsStub::sortMostUniqueHeadersFirst(const std::shared_ptr<QueryResultsTable>& a, const std::shared_ptr<QueryResultsTable>& b) {
	set<string> setA = a->getHeadersAsSet(); set<string> setB = b->getHeadersAsSet();
	return setA.size() > setB.size();
}

// An auxiliary function to sort the tables in a vector by the most common header
void sortVectorOfTablesByHeaderStub(vector<shared_ptr<QueryResultsTable>>& tables) {
	/*for (shared_ptr<QueryResultsTable> table : tables) {
		vector<string> headers = table->getHeaders();
		for (string header : headers) {
			OptimisedFunctionsStub::updateCountHeaderStore(header);
		}
	}
	sort(tables.begin(), tables.end(),sortMostCommonHeaderFirstStub);
	OptimisedFunctionsStub::resetCountHeaderStore();*/
}

// 1. Removes columns that the select clauses do not ask for
// 2. Add all empty tables to the beginning
void OptimisedFunctionsStub::optimiseStepA(vector<shared_ptr<QueryResultsTable>>& nonSelectClauseTables, bool isParallel) {
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
	if (isParallel)
		sort(execution::par_unseq, nonSelectClauseTables.begin(), nonSelectClauseTables.end(), sortEmptyFirstStub);
	else sort(nonSelectClauseTables.begin(), nonSelectClauseTables.end(), sortEmptyFirstStub);
	//End of step 2			
}

// Goes through each group and merge groups that shares same headers
//void optimiseStepC(vector< vector<shared_ptr<QueryResultsTable>> >& groups, vector<set<string>> groupHeaders) {
void OptimisedFunctionsStub::optimiseStepC(vector< shared_ptr<GroupClause> >& groups) {
	//TODO
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

void optimisedSubStepD2Stub(vector<shared_ptr<QueryResultsTable>>& groupMembers, set<string> groupHeaders) {
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

void optimisedSubStepD1Stub(vector<shared_ptr<QueryResultsTable>> groupMembers, set<string>& headerContainer, set<string> groupHeaders) {
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

// Rearrange the clauses in the group such that the clauses with the most common headers are arranged at the front.
// This means a higher chance of "mutual friends" already forming at the beginning, so more likely that an inner join
// will occur than a cross product.
void OptimisedFunctionsStub::optimiseStepD(vector<shared_ptr<GroupClause>>&groups) {
	//for (vector<shared_ptr<QueryResultsTable>>& group : groups) {
	//	if (group.size() <= 1 || group[0]->isEmpty()) continue; //skip for empty table groups and if group size is not greater than 1
		//sortVectorOfTablesByHeaderStub(group);

	//}
	if (groups.size() == 0) return;
	int startIndex = 0;
	if (groups[0]->hasEmptyTablesFirst()) startIndex = 1; // start from 1 given that the first group is the emptyTables group
	for (int i = startIndex; i < groups.size(); i++) { // Iterate for each group
		if (groups[i]->size() <= 2) continue; // In a group of not more than 2, each table is guaranteed to share a header with a neighbor
		set<string> groupHeaders = groups[i]->getHeaders();
		vector<shared_ptr<QueryResultsTable>> currGroupMembers = groups[i]->getMembers();
		set<string> currTableHeaders = currGroupMembers[0]->getHeadersAsSet();
		if (currTableHeaders == groupHeaders) continue; // Means that the first table already has headers that covers the whole group
		optimisedSubStepD1Stub(currGroupMembers, currTableHeaders, groupHeaders); // Checks for a possible link between first table and some table that covers the whole group
		if (currTableHeaders == groupHeaders) continue; // Means there is a link from the first table to some table in a later row that covers the whole group
		optimisedSubStepD2Stub(currGroupMembers, groupHeaders);
		groups[i]->setMembers(currGroupMembers);
	}

}

// An auxiliary function to flatten a 2D Vector of shared_ptr<QueryResultsTable>
vector<shared_ptr<QueryResultsTable>> OptimisedFunctionsStub::revert1DTables(vector< shared_ptr<GroupClause> > groups) {
	vector<shared_ptr<QueryResultsTable>> v1d;
	for (shared_ptr<GroupClause> group : groups) {
		vector<shared_ptr<QueryResultsTable>> tables = group->getMembers();
		v1d.insert(v1d.end(), tables.begin(), tables.end());
	}
	return v1d;
}

// Group the clauses
// A group of clauses is guaranteed to have a continuous link between all clauses.
// However the order of the clauses is not guaranteed such that any two neighboring clauses
// will share a common header.
vector<shared_ptr<GroupClause>> OptimisedFunctionsStub::optimiseStepB(vector<shared_ptr<QueryResultsTable>>& nonSelectClauseTables, bool isParallel) {
	//vector< vector<shared_ptr<QueryResultsTable>> > groups;
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
		shared_ptr<GroupClause> twoNonEmptyTables = make_shared<GroupClause>();
		twoNonEmptyTables->addMany(nonEmptyTables);
		groups.emplace_back(twoNonEmptyTables);
		//optimiseStepC(groups);
		//nonSelectClauseTables = revertToVectorOfTables(groups);
		return groups;
	}

	// sort the most table with the most common headers at the start
	/*for (shared_ptr<QueryResultsTable> table : nonEmptyTables) {
		vector<string> headers = table->getHeaders();
		for (string header : headers) {
			OptimisedFunctionsStub::updateCountHeaderStore(header);
		}
	}
	sort(nonEmptyTables.begin(), nonEmptyTables.end(), sortMostCommonHeaderFirstStub);
	OptimisedFunctionsStub::resetCountHeaderStore();*/
	//sortVectorOfTablesByHeaderStub(nonEmptyTables);
	if (isParallel) sort(execution::par_unseq, nonEmptyTables.begin(), nonEmptyTables.end(), sortMostUniqueHeadersFirst);
	else sort(nonEmptyTables.begin(), nonEmptyTables.end(), sortMostUniqueHeadersFirst);
	//groups.emplace_back(nonEmptyTables);
	//nonSelectClauseTables = flatten2DArray(groups);
	shared_ptr<GroupClause> newGroup = make_shared<GroupClause>();

	//vector<string> headers = nonEmptyTables[0]->getHeaders();
	//set<string> firstHeaders;
	//copy(headers.begin(), headers.end(), inserter(firstHeaders, firstHeaders.begin()));
	//vector<set<string> > groupNames;
	//groupNames.emplace_back(firstHeaders);

	//vector<shared_ptr<QueryResultsTable>> newGroup;
	
	//newGroup.emplace_back(nonEmptyTables[0]);
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
	//optimiseStepC(groups);
	//optimiseStepD(groups);
	//nonSelectClauseTables = OptimisedFunctionsStub::revert1DTables(groups);
}

