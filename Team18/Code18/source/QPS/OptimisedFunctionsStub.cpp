#include "OptimisedFunctionsStub.h"

// A set to contain only unique select clauses
set<string> OptimisedFunctionsStub::getHeadersOfTableAsSet(vector<shared_ptr<QueryResultsTable>> selectClauseTables) {
	set<string> result;
	for (shared_ptr<QueryResultsTable> table : selectClauseTables) {
		result.insert(table->getHeaders()[0]);
	}
	return result;
}

bool sortEmptyFirstStub(const std::shared_ptr<QueryResultsTable>& a, const std::shared_ptr<QueryResultsTable>& b) {
	int priorityA = -1;
	int priorityB = 0;
	if (a->isEmpty() && !b->isEmpty()) {
		priorityA = 1;
	}
	return priorityA > priorityB;
}

// An auxiliary function to aid in the comparison within the data structure of vector<shared_ptr<QueryResultsTable>>
// The table containing the more common header in the group of clauses will be sorted before the other.
bool sortMostCommonHeaderFirstStub(const std::shared_ptr<QueryResultsTable>& a, const std::shared_ptr<QueryResultsTable>& b) {
	int maxValueA = 0;
	int maxValueB = 0;
	vector<string> headersA = a->getHeaders();
	vector<string> headersB = b->getHeaders();
	for (string header : headersA) {
		if (OptimisedFunctionsStub::getCount(header) > maxValueA) {
			maxValueA = OptimisedFunctionsStub::getCount(header);
		}
	}
	for (string header : headersB) {
		if (OptimisedFunctionsStub::getCount(header) > maxValueB) {
			maxValueB = OptimisedFunctionsStub::getCount(header);
		}
	}
	return maxValueA > maxValueB;
}

// An auxiliary function to sort the tables in a vector by the most common header
void sortVectorOfTablesByHeaderStub(vector<shared_ptr<QueryResultsTable>>& tables) {
	for (shared_ptr<QueryResultsTable> table : tables) {
		vector<string> headers = table->getHeaders();
		for (string header : headers) {
			OptimisedFunctionsStub::updateCountHeaderStore(header);
		}
	}
	sort(tables.begin(), tables.end(), sortMostCommonHeaderFirstStub);
	OptimisedFunctionsStub::resetCountHeaderStore();
}

// 1. Removes columns that the select clauses do not ask for
// 2. Add all empty tables to the beginning
void OptimisedFunctionsStub::optimiseStepA(vector<shared_ptr<QueryResultsTable>>& nonSelectClauseTables) {
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
	sort(nonSelectClauseTables.begin(), nonSelectClauseTables.end(), sortEmptyFirstStub);
	//End of step 2			
}

// Group the clauses
// A group of clauses is guaranteed to have a continuous link between all clauses.
// However the order of the clauses is not guaranteed such that any two neighboring clauses
// will share a common header.
void OptimisedFunctionsStub::optimiseStepB(vector<shared_ptr<QueryResultsTable>>& nonSelectClauseTables) {
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
		/*for (shared_ptr<QueryResultsTable> table : nonEmptyTables) {
			vector<string> headers = table->getHeaders();
			for (string header : headers) {
				OptimisedFunctionsStub::updateCountHeaderStore(header);
			}
		}
		sort(nonEmptyTables.begin(), nonEmptyTables.end(), sortMostCommonHeaderFirstStub);
		OptimisedFunctionsStub::resetCountHeaderStore();*/
		sortVectorOfTablesByHeaderStub(nonEmptyTables);

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

// Rearrange the clauses in the group such that the clauses with the most common headers are arranged at the front.
// This means a higher chance of "mutual friends" already forming at the beginning, so more likely that an inner join
// will occur than a cross product.
void OptimisedFunctionsStub::optimiseStepC(vector< vector<shared_ptr<QueryResultsTable>> >& groups) {
	for (vector<shared_ptr<QueryResultsTable>>& group : groups) {
		if (group.size() <= 1 || group[0]->isEmpty()) continue; //skip for empty table groups and if group size is not greater than 1
		sortVectorOfTablesByHeaderStub(group);
	}
}

// An auxiliary function to flatten vector< vector<shared_ptr<QueryResultsTable>> >
vector<shared_ptr<QueryResultsTable>> OptimisedFunctionsStub::flatten2DArray(vector< vector<shared_ptr<QueryResultsTable>> > v2d) {
	vector<shared_ptr<QueryResultsTable>> v1d;
	for (const auto& row : v2d) {
		v1d.insert(v1d.end(), row.begin(), row.end());
	}
	return v1d;
}
