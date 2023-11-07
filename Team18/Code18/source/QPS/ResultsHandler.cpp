#include "ResultsHandler.h"

list<string> ResultHandler::processTables(vector<shared_ptr<QueryResultsTable>> selectClauseTables, vector<shared_ptr<QueryResultsTable>> nonSelectClauseTables) {
	if (isSingleSynonym(selectClauseTables)) {
		return handleSingleSynonym(selectClauseTables, nonSelectClauseTables);
	}
	else if (isTuples(selectClauseTables)) {
		return handleTuples(selectClauseTables, nonSelectClauseTables);
	}
	else { // BOOLEAN
		return handleBoolean(selectClauseTables, nonSelectClauseTables);
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
		if (intermediateTable->haveSimilarHeaders(currTable)) {
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

unordered_set<string> ResultHandler::getSetOfSelectClauseHeaders(vector<shared_ptr<QueryResultsTable>> selectClauseTables) {
	unordered_set<string> headers;
	for (shared_ptr<QueryResultsTable> table : selectClauseTables) {
		vector<string> tableHeaders = table->getHeaders();
		for (string tableHeader : tableHeaders) {
			headers.insert(tableHeader);
		}
	}
	return headers;
}


string ResultHandler::getAttributeSynonym(string attribute) {
	size_t pos = attribute.find('.');
	if (pos == string::npos) {
		return attribute;
	}

	return attribute.substr(0, pos);
}

vector<string> ResultHandler::getVectorOfSelectClausePkey(vector<shared_ptr<QueryResultsTable>> selectClauseTables) {
	vector<string> primaryKeys;
	for (shared_ptr<QueryResultsTable> table : selectClauseTables) {
		primaryKeys.push_back(table->getPrimaryKey());
	}
	return primaryKeys;
}

vector<string> ResultHandler::tableToVectorForTuples(shared_ptr<QueryResultsTable> table) {
	vector<string> result;
	vector<string> headers = table->getHeaders();
	int totalRow = table->getNumberOfRows();
	int totalHeaders = headers.size();

	for (int i = 0; i < totalRow; i++) {
		string curr = "";
		for (int j = 0; j < totalHeaders; j++) {
			vector<map<string, vector<string>>> cols = table->getColumns();
			vector<string> column = cols[j].begin()->second;
			if (j == totalHeaders - 1) { // last element
				curr += column[i];
				break;
			}
			curr += column[i] + " ";
		}
		result.push_back(curr);
	}
	return result;
}

tuple<vector<shared_ptr<QueryResultsTable>>, 
	shared_ptr<QueryResultsTable>> ResultHandler::
	processIntermediateTable(vector<shared_ptr<QueryResultsTable>> selectClauseTables, shared_ptr<QueryResultsTable> intermediateTable) {
	unordered_set<string> selectClauseHeaders = getSetOfSelectClauseHeaders(selectClauseTables);
	intermediateTable->condenseTable(selectClauseHeaders); // keep important columns only
	vector<shared_ptr<QueryResultsTable>> selectClausesNotInIntermediateTable;
	vector<map<string, vector<string>>> cols;
	for (shared_ptr<QueryResultsTable> selectClauseTable : selectClauseTables) {
		string primaryKey = selectClauseTable->getPrimaryKey();
		if (intermediateTable->hasHeader(primaryKey)) {
			map<string, vector<string>> col = { {primaryKey, intermediateTable->getColumnData(primaryKey)} };
			cols.push_back(col);
		}
		else if (intermediateTable->hasAttributeHeader(primaryKey)) {
			vector<string> col = intermediateTable->getColumnData(getAttributeSynonym(primaryKey));
			// convert col to attribute
			StringMap attr = selectClauseTable->getAttr();
			for (int i = 0; i < col.size(); i++) {
				col[i] = *attr[col[i]].begin(); // replace p with p.procname
			}
			cols.push_back({ {primaryKey, col} });
		}
		else {
			selectClauseTable->getPrimaryKeyOnlyTable();
			selectClausesNotInIntermediateTable.push_back(selectClauseTable);
		}

	}

	return { selectClausesNotInIntermediateTable, make_shared<QueryResultsTable>(cols) };
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
* Manipulation of table required
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

	// case 2.2
	tuple<vector<shared_ptr<QueryResultsTable>>,
		shared_ptr<QueryResultsTable>> processTables = processIntermediateTable(selectClauseTables, intermediateTable);
	vector<shared_ptr<QueryResultsTable>> selectClausesNotInIntermediateTable = get<0>(processTables);
	shared_ptr<QueryResultsTable> processedIntermediateTable = get<1>(processTables);
	shared_ptr<QueryResultsTable> finalTable = QueryResultsTable::createEmptyTable(true);
	if (processedIntermediateTable->getNumberOfCols() > 0) { // synonyms required exist in the intermediate table
		shared_ptr<QueryResultsTable> synsInIntermediateTable = processedIntermediateTable;
		finalTable = finalTable->crossProduct(synsInIntermediateTable);
	}
	for (shared_ptr<QueryResultsTable> table : selectClausesNotInIntermediateTable) {
		finalTable = finalTable->crossProduct(table);
	}
	vector<string> colContents = finalTable->getColumnData(selectVar);
	return vectorToUniqueList(colContents);
}

list<string> ResultHandler::returnTuples(vector<shared_ptr<QueryResultsTable>> selectClauseTables) {
	shared_ptr<QueryResultsTable> intermediateTable = selectClauseTables[0];
	intermediateTable->getPrimaryKeyOnlyTable();
	selectClauseTables.erase(selectClauseTables.begin());
	for (shared_ptr<QueryResultsTable> table : selectClauseTables) {
		table->getPrimaryKeyOnlyTable();
		if (intermediateTable->haveSimilarHeaders(table)) { // for cases like select<s, s>, duplicate the column (do not cross product)
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
	tuple<vector<shared_ptr<QueryResultsTable>>,
		shared_ptr<QueryResultsTable>> processTables = processIntermediateTable(selectClauseTables, intermediateTable);
	vector<shared_ptr<QueryResultsTable>> selectClausesNotInIntermediateTable = get<0>(processTables);
	shared_ptr<QueryResultsTable> processedIntermediateTable = get<1>(processTables);
	vector<string> targetSyns = getVectorOfSelectClausePkey(selectClauseTables); // order matters
	shared_ptr<QueryResultsTable> finalTable = QueryResultsTable::createEmptyTable(true);
	if (processedIntermediateTable->getNumberOfCols() > 0) { // synonyms required exists in the intermediate table
		shared_ptr<QueryResultsTable> synsInIntermediateTable = processedIntermediateTable;
		finalTable = finalTable->crossProduct(synsInIntermediateTable);
	}
	for (shared_ptr<QueryResultsTable> table : selectClausesNotInIntermediateTable) {
		if (finalTable->haveSimilarHeaders(table)) {
			finalTable->duplicateColumns(table->getPrimaryKey());
		}
		else {
			finalTable = finalTable->crossProduct(table);
		}
	}
	vector<map<string, vector<string>>> resultCols;
	for (string target : targetSyns) {
		resultCols.push_back({ {target,  finalTable->getColumnData(target)} });
	}
	shared_ptr<QueryResultsTable> resultTable = make_shared<QueryResultsTable>(resultCols);
	vector<string> result = tableToVectorForTuples(resultTable);
	return vectorToUniqueList(result);
}

list<string> ResultHandler::handleBoolean(vector<shared_ptr<QueryResultsTable>> selectClauseTables, vector<shared_ptr<QueryResultsTable>> nonSelectClauseTables) {
	list<string> result;
	
	if (nonSelectClauseTables.empty()) {
		result.push_back(TRUE_STRING);
		return result;
	}

	shared_ptr<QueryResultsTable> intermediateTable = joinIntermediateTables(nonSelectClauseTables);

	if (intermediateTable->isEmpty()) { // case 2.1
		if (intermediateTable->getSignificant()) { // case 2.1.1
			result.push_back(TRUE_STRING);
		}
		else { // case 2.1.2
			result.push_back(FALSE_STRING);
		}
		return result;
	}
	// non-empty intermediate table

	result.push_back(TRUE_STRING);
	return result;
}
