#include "ResultsHandlerStub.h"

list<string> ResultsHandlerStub::processTables(vector<shared_ptr<QueryResultsTable>> selectClauseTables, vector<shared_ptr<QueryResultsTable>> nonSelectClauseTables) {
	ResultsHandlerStub::setOptimiseSwitch(); //trigger optimisation
	if (ResultsHandlerStub::getOptimisedSwitch()) { //perform optimised processing of tables in this branch
		optimiseStepA(selectClauseTables, nonSelectClauseTables);
		vector<shared_ptr<QueryResultsTable>> groups = optimiseStepB(nonSelectClauseTables);
		nonSelectClauseTables = groups;
	}
	if (isSingleSynonym(selectClauseTables)) {
		return handleSingleSynonym(selectClauseTables, nonSelectClauseTables);
	}
	else if (isTuples(selectClauseTables)) {
		return handleTuples(selectClauseTables, nonSelectClauseTables);
	}
	else { // BOOLEAN

	}

	list<string> empty;
	return empty;
}

// A set to contain only unique select clauses
set<string> ResultsHandlerStub::getHeadersOfTableAsSet(vector<shared_ptr<QueryResultsTable>> selectClauseTables) {
	set<string> result;
	for (shared_ptr<QueryResultsTable> table : selectClauseTables) {
		result.insert(table->getHeaders()[0]);
	}
	return result;
}

// 1. Removes columns that the select clauses do not ask for
// 2. Add all empty tables to the beginning
void ResultsHandlerStub::optimiseStepA(vector<shared_ptr<QueryResultsTable>> selectClauseTables, vector<shared_ptr<QueryResultsTable>>& nonSelectClauseTables) {
	set<string> selectClauses = getHeadersOfTableAsSet(selectClauseTables);
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
	}
	//End of step 1
	vector<shared_ptr<QueryResultsTable>> emptyTables;
	for (shared_ptr<QueryResultsTable> table : nonSelectClauseTables) {
		if (table->isEmpty()) {
			emptyTables.emplace_back(table);
		}
	}
	vector<shared_ptr<QueryResultsTable>> result;
	// Get non empty tables by taking the opposite of an intersection between the emptyTables and nonSelectClauseTables
	std::set_symmetric_difference(emptyTables.begin(), emptyTables.end(), nonSelectClauseTables.begin(), nonSelectClauseTables.end(), std::inserter(result, result.begin()));
	result.insert(result.begin(), emptyTables.begin(), emptyTables.end());
	nonSelectClauseTables = result;
	//End of step 2			
}

// Group the clauses
// A group of clauses is guaranteed to have a continuous link between all clauses.
// However the order of the clauses is not guaranteed such that any two neighboring clauses
// will share a common header.
vector<shared_ptr<QueryResultsTable>> ResultsHandlerStub::optimiseStepB(vector<shared_ptr<QueryResultsTable>> nonSelectClauseTables) {
	vector< vector<shared_ptr<QueryResultsTable>> > groups;
	vector<shared_ptr<QueryResultsTable>> emptyTables;
	for (shared_ptr<QueryResultsTable> table : nonSelectClauseTables) {
		if (table->isEmpty()) emptyTables.emplace_back(table);
		else break;
	}
	groups.emplace_back(emptyTables);
	vector<shared_ptr<QueryResultsTable>> nonEmptyTables;
	// Get non empty tables by taking the opposite of an intersection between the emptyTables and nonSelectClauseTables
	std::set_symmetric_difference(emptyTables.begin(), emptyTables.end(), nonSelectClauseTables.begin(), nonSelectClauseTables.end(), std::inserter(nonEmptyTables, nonEmptyTables.begin()));
	if (nonEmptyTables.size() > 1) {
		vector<string> headers = nonEmptyTables[0]->getHeaders();
		set<string> firstHeaders;
		copy(firstHeaders.begin(), firstHeaders.end(), inserter(headers, headers.end()));
		vector<set<string> > groupNames;
		groupNames.emplace_back(firstHeaders);
		vector<shared_ptr<QueryResultsTable>> newGroup;
		newGroup.emplace_back(nonEmptyTables[0]);
		groups.emplace_back(newGroup);
		nonEmptyTables.erase(nonEmptyTables.begin());

		for (shared_ptr<QueryResultsTable> table : nonEmptyTables) {
			vector<string> thisHeaders = table->getHeaders();
			int index = 0;
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
				copy(newHeaders.begin(), newHeaders.end(), inserter(thisHeaders, thisHeaders.end()));
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
	return flatten2DArray(groups);
}

// An auxiliary function to aid in the comparison within the data structure of vector<shared_ptr<QueryResultsTable>>
// The table containing the more common header in the group of clauses will be sorted before the other.
bool compareStub(const std::shared_ptr<QueryResultsTable>& a, const std::shared_ptr<QueryResultsTable>& b) {
	int maxValueA = 0;
	int maxValueB = 0;
	vector<string> headersA = a->getHeaders();
	vector<string> headersB = b->getHeaders();
	for (string header : headersA) {
		if (ResultsHandlerStub::getCount(header) > maxValueA) {
			maxValueA = ResultsHandlerStub::getCount(header);
		}
	}
	for (string header : headersB) {
		if (ResultsHandlerStub::getCount(header) > maxValueB) {
			maxValueB = ResultsHandlerStub::getCount(header);
		}
	}
	return maxValueA > maxValueB;
}

// Rearrange the clauses in the group such that the clauses with the most common headers are arranged at the front.
// This means a higher chance of "mutual friends" already forming at the beginning, so more likely that an inner join
// will occur than a cross product.
void ResultsHandlerStub::optimiseStepC(vector< vector<shared_ptr<QueryResultsTable>> >& groups) {
	for (vector<shared_ptr<QueryResultsTable>>& group : groups) {
		if (group[0]->isEmpty()) continue; //skip for empty table groups
		for (shared_ptr<QueryResultsTable> table : group) {
			vector<string> headers = table->getHeaders();
			for (string header : headers) {
				ResultsHandlerStub::updateCountHeaderStore(header);
			}
		}
		sort(group.begin(), group.end(), compareStub);
		ResultsHandlerStub::resetCountHeaderStore();
	}
}

// An auxiliary function to flatten vector< vector<shared_ptr<QueryResultsTable>> >
vector<shared_ptr<QueryResultsTable>> ResultsHandlerStub::flatten2DArray(vector< vector<shared_ptr<QueryResultsTable>> > v2d) {
	vector<shared_ptr<QueryResultsTable>> v1d;
	for (const auto& row : v2d) {
		v1d.insert(v1d.end(), row.begin(), row.end());
	}
	return v1d;
}

// tables must be non-empty
shared_ptr<QueryResultsTable> ResultsHandlerStub::joinIntermediateTables(vector<shared_ptr<QueryResultsTable>> tables) {
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
		if (intermediateTable->haveSameHeaders(currTable)) {
			//do inner join
			intermediateTable = intermediateTable->innerJoin(currTable);
		}
		else {
			//do cross product
			intermediateTable = intermediateTable->crossProduct(currTable);
		}
	}
	return intermediateTable;
}

/*
* Case 1: No such that clause
* Just return select table column
* Case 2: Has such that clause
* 2.1: Such that clause is empty
* 2.1.1: Such that clause is significant
* return select table column like case 1
* 2.1.2 Such that clause is not significant
* return empty
* 2.2: Such that clause is not empty (means significant)
* join with select clause only if has common headers
* 2.2.1 select the header and return
* 2.3 Return empty by default (unhandled case?)
*/

list<string> ResultsHandlerStub::handleSingleSynonym(vector<shared_ptr<QueryResultsTable>> selectClauseTables, vector<shared_ptr<QueryResultsTable>> nonSelectClauseTables) {
	if (nonSelectClauseTables.empty()) { // case 1
		shared_ptr<QueryResultsTable> selectTable = selectClauseTables[0]; // guaranteed to exist, by conditional check prior
		vector<string> columns = selectTable->getColumnData(selectTable->getPrimaryKey());

		return vectorToUniqueList(columns);
	}

	shared_ptr<QueryResultsTable> intermediateTable = joinIntermediateTables(nonSelectClauseTables);

	string selectVar = selectClauseTables[0]->getPrimaryKey();
	if (intermediateTable->isEmpty()) { // case 2.1
		if (intermediateTable->getSignificant()) { // case 2.1.1
			shared_ptr<QueryResultsTable> selectTable = selectClauseTables[0];
			vector<string> colContents = selectTable->getColumnData(selectVar);
			return vectorToUniqueList(colContents);
		}
		else { // case 2.1.2
			list<string> empty;
			return empty;
		}

	}

	// add select clauses in for 'filtering', for attributes which are 2 column tables
	vector<shared_ptr<QueryResultsTable>> mergeTable = { intermediateTable };
	mergeTable.insert(mergeTable.end(), selectClauseTables.begin(), selectClauseTables.end());
	// case 2.2
	if (selectClauseTables[0]->haveSameHeaders(intermediateTable)) {
		intermediateTable = joinIntermediateTables(mergeTable);
		vector<string> colContents = intermediateTable->getColumnData(selectVar);
		return vectorToUniqueList(colContents);
	}
	else {
		vector<string> colContents = selectClauseTables[0]->getColumnData(selectVar);
		return vectorToUniqueList(colContents);
	}


	list<string> empty;
	return empty;


}

list<string> ResultsHandlerStub::returnTuples(vector<shared_ptr<QueryResultsTable>> selectClauseTables) {
	shared_ptr<QueryResultsTable> intermediateTable = selectClauseTables[0];
	intermediateTable->getPrimaryKeyOnlyTable();
	selectClauseTables.erase(selectClauseTables.begin());
	for (shared_ptr<QueryResultsTable> table : selectClauseTables) {
		table->getPrimaryKeyOnlyTable();
		if (intermediateTable->haveSameHeaders(table)) { // for cases like select<s, s>, duplicate the column (do not cross product)
			intermediateTable->duplicateColumns(table->getPrimaryKey());
		}
		else {
			intermediateTable = intermediateTable->crossProduct(table);
		}
	}
	vector<string> result = tableToVectorForTuples(intermediateTable);
	return vectorToUniqueList(result);
}

list<string> ResultsHandlerStub::handleTuples(vector<shared_ptr<QueryResultsTable>> selectClauseTables, vector<shared_ptr<QueryResultsTable>> nonSelectClauseTables) {
	if (nonSelectClauseTables.empty()) { // case 1
		return returnTuples(selectClauseTables);
	}

	shared_ptr<QueryResultsTable> intermediateTable = joinIntermediateTables(nonSelectClauseTables);

	if (intermediateTable->isEmpty()) { // case 2.1
		if (intermediateTable->getSignificant()) { // case 2.1.1
			return returnTuples(selectClauseTables);
		}
		else { // case 2.1.2
			list<string> empty;
			return empty;
		}

	}


	// case 2.2
	// join select clause tables
	vector<string> selectClauseHeaders;
	vector<shared_ptr<QueryResultsTable>> selectClausesInIntermediateTable;
	map<string, shared_ptr<QueryResultsTable>> selectClausesNotInIntermediateTable;

	for (shared_ptr<QueryResultsTable> table : selectClauseTables) {
		selectClauseHeaders.push_back(table->getPrimaryKey());
		if (intermediateTable->haveSameHeaders(table)) {
			selectClausesInIntermediateTable.push_back(table);
		}
		else {
			table->getPrimaryKeyOnlyTable();
			selectClausesNotInIntermediateTable[table->getPrimaryKey()] = table;
		}
	}


	vector<shared_ptr<QueryResultsTable>> mergeTable = { intermediateTable };
	mergeTable.insert(mergeTable.end(), selectClausesInIntermediateTable.begin(), selectClausesInIntermediateTable.end());
	intermediateTable = joinIntermediateTables(mergeTable);

	vector<shared_ptr<QueryResultsTable>> tupleTables; // case 2.2.1
	for (string header : selectClauseHeaders) {
		if (intermediateTable->hasHeader(header)) {
			shared_ptr<QueryResultsTable> oneDTable = QueryResultsTable::createTable(header, intermediateTable->getColumnData(header));
			oneDTable->setPrimaryKey(header);
			tupleTables.push_back(oneDTable);
		}
		else {
			tupleTables.push_back(selectClausesNotInIntermediateTable[header]);
		}
	}

	return returnTuples(tupleTables);
}