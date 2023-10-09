#include "ResultsHandler.h"



list<string> ResultHandler::processTables(vector<shared_ptr<QueryResultsTable>> selectClauseTables, vector<shared_ptr<QueryResultsTable>> nonSelectClauseTables) {
	//for now we do brute force left to right execution, optimisation can come in the future
	
	// TODO: condense logic into helper functions

	// CASE 1: query has no such that clause
	if (nonSelectClauseTables.empty()) {
		if (isSingleSynonym(selectClauseTables)) {  // normal select a;
			shared_ptr<QueryResultsTable> selectTable = selectClauseTables[0];
			vector<string> columns = selectTable->getColumnData(selectTable->getHeaders()[0]); // only one header

			return vectorToUniqueList(columns);
		}
		else if (isTuples(selectClauseTables)) {
			shared_ptr<QueryResultsTable> intermediateTable = selectClauseTables[0];
			selectClauseTables.erase(selectClauseTables.begin());
			for (shared_ptr<QueryResultsTable> table : selectClauseTables) {
				intermediateTable = intermediateTable->crossProduct(table);
			}
			vector<string> result =  tableToListForTuples(intermediateTable);
			return vectorToUniqueList(result);
		}
		else {
			// TODO handle BOOLEAN
		}
	}

	// CASE 2: query has such that clauses
	shared_ptr<QueryResultsTable> intermediateTable = nonSelectClauseTables[0];
	nonSelectClauseTables.erase(nonSelectClauseTables.begin());
	shared_ptr<QueryResultsTable> currTable;
	for (shared_ptr<QueryResultsTable> table : nonSelectClauseTables) {
		currTable = table;
		// for empty but significant tables
		if (currTable->isEmpty()) {
			if (currTable->getSignificant()) {
				continue; // just keep current table
			}
			else { // no need to evaluate the rest of the query
				list<string> empty;
				return empty;
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

	//filter out the required result

	vector<map<string, vector<string>>> thisColumns = intermediateTable->getColumns();
	if (intermediateTable->isEmpty()) { // empty such that clause tables after joining
		if (intermediateTable->getSignificant()) {
			if (isSingleSynonym(selectClauseTables)) {
				shared_ptr<QueryResultsTable> selectTable = selectClauseTables[0];
				vector<string> colContents = selectTable->getColumnData(selectTable->getHeaders()[0]);
				return vectorToList(colContents);
			}
			else if (isTuples(selectClauseTables)) {
				shared_ptr<QueryResultsTable> intermediateTable = selectClauseTables[0];
				selectClauseTables.erase(selectClauseTables.begin());
				for (shared_ptr<QueryResultsTable> table : selectClauseTables) {
					intermediateTable = intermediateTable->crossProduct(table);
				}
				vector<string> result = tableToListForTuples(intermediateTable);
				return vectorToUniqueList(result);
			}
				
		}
		else {
			list<string> empty;
			return empty;
		}

	}
	string selectVar = selectClauseTables[0]->getHeaders()[0];
	// non-empty such that clause tables after joining
	for (map<string, vector<string>> column : thisColumns) {
		string key = column.begin()->first;
		if (key == selectVar) { // select clause has common headers with tables
			vector<string> colContents = column.begin()->second;
			return vectorToList(colContents);
		}
	}


	if (intermediateTable->getSignificant()) {// select clause has no common headers with tables
		shared_ptr<QueryResultsTable> selectTable = selectClauseTables[0];
		vector<string> colContents = selectTable->getColumns()[0].begin()->second;
		return vectorToList(colContents);
	}
	else {
		list<string> empty;
		return empty;
	}


	list<string> empty;
	return empty;
}