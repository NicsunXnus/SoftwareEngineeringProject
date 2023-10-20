#include "ResultsHandler.h"



list<string> ResultHandler::processTables(vector<shared_ptr<QueryResultsTable>> selectClauseTables, vector<shared_ptr<QueryResultsTable>> nonSelectClauseTables) {
	//for now we do brute force left to right execution, optimisation can come in the future
	

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

// tables must be non-empty
shared_ptr<QueryResultsTable> ResultHandler::joinIntermediateTables(vector<shared_ptr<QueryResultsTable>> tables) {
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

list<string> ResultHandler::handleSingleSynonym(vector<shared_ptr<QueryResultsTable>> selectClauseTables, vector<shared_ptr<QueryResultsTable>> nonSelectClauseTables) {
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

list<string> ResultHandler::returnTuples(vector<shared_ptr<QueryResultsTable>> selectClauseTables) {
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

list<string> ResultHandler::handleTuples(vector<shared_ptr<QueryResultsTable>> selectClauseTables, vector<shared_ptr<QueryResultsTable>> nonSelectClauseTables) {
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