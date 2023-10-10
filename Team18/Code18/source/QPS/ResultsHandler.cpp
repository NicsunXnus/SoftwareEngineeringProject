#include "ResultsHandler.h"



list<string> ResultHandler::processTables(vector<shared_ptr<QueryResultsTable>> selectClauseTables, vector<shared_ptr<QueryResultsTable>> nonSelectClauseTables) {
	//for now we do brute force left to right execution, optimisation can come in the future
	
	// TODO: condense logic into helper functions

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

shared_ptr<QueryResultsTable> ResultHandler::joinIntermediateTables(vector<shared_ptr<QueryResultsTable>> nonSelectClauseTables) {
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
				return make_shared<QueryResultsTable>();
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

list<string> ResultHandler::handleSingleSynonym(vector<shared_ptr<QueryResultsTable>> selectClauseTables, vector<shared_ptr<QueryResultsTable>> nonSelectClauseTables) {
	if (nonSelectClauseTables.empty()) { // query has no such that clause
		shared_ptr<QueryResultsTable> selectTable = selectClauseTables[0];
		vector<string> columns = selectTable->getColumnData(selectTable->getHeaders()[0]); // only one header

		return vectorToUniqueList(columns);
	}

	shared_ptr<QueryResultsTable> intermediateTable = joinIntermediateTables(nonSelectClauseTables);

	vector<map<string, vector<string>>> thisColumns = intermediateTable->getColumns();
	if (intermediateTable->isEmpty()) { // empty such that clause tables after joining
		if (intermediateTable->getSignificant()) {
			shared_ptr<QueryResultsTable> selectTable = selectClauseTables[0];
			vector<string> colContents = selectTable->getColumnData(selectTable->getHeaders()[0]);
			return vectorToUniqueList(colContents);
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
			return vectorToUniqueList(colContents);
		}
	}


	if (intermediateTable->getSignificant()) {// select clause has no common headers with tables
		shared_ptr<QueryResultsTable> selectTable = selectClauseTables[0];
		vector<string> colContents = selectTable->getColumns()[0].begin()->second;
		return vectorToUniqueList(colContents);
	}
	list<string> empty;
	return empty;


}

list<string> ResultHandler::handleTuples(vector<shared_ptr<QueryResultsTable>> selectClauseTables, vector<shared_ptr<QueryResultsTable>> nonSelectClauseTables) {
	if (nonSelectClauseTables.empty()) {
		shared_ptr<QueryResultsTable> intermediateTable = selectClauseTables[0];
		selectClauseTables.erase(selectClauseTables.begin());
		for (shared_ptr<QueryResultsTable> table : selectClauseTables) {
			intermediateTable = intermediateTable->crossProduct(table);
		}
		vector<string> result = tableToListForTuples(intermediateTable);
		return vectorToUniqueList(result);
	}

	shared_ptr<QueryResultsTable> intermediateTable = joinIntermediateTables(nonSelectClauseTables);
	
	if (intermediateTable->isEmpty()) { // empty such that clause tables after joining
		if (!intermediateTable->getSignificant()) {
			list<string> empty;
			return empty;
		}
	}

	// non-empty such that clause tables after joining
	// get columns with similar headers
	vector<shared_ptr<QueryResultsTable>> tupleTables;
	for (shared_ptr<QueryResultsTable> selectTable : selectClauseTables) {
		if (intermediateTable->haveSameHeaders(selectTable)) {
			string header = selectTable->getHeaders()[0];
			shared_ptr<QueryResultsTable> oneDTable = QueryResultsTable::createTable(header, intermediateTable->getColumnData(header));
			tupleTables.push_back(oneDTable);
		}
		else {
			tupleTables.push_back(selectTable);
		}
	}
	shared_ptr<QueryResultsTable> intermediateResultsTable = tupleTables[0];
	tupleTables.erase(tupleTables.begin());
	for (shared_ptr<QueryResultsTable> selectTable : tupleTables) {
		intermediateResultsTable = intermediateResultsTable->crossProduct(selectTable);
	}
	vector<string> result = tableToListForTuples(intermediateResultsTable);
	return vectorToUniqueList(result);


}