#include "ResultsHandler.h"

list<string> ResultHandler::processTables(std::vector<std::shared_ptr<QueryResultsTable>> tables) {
	list<string> result;
	shared_ptr<QueryResultsTable> immediateTable;

	//for now we do brute force left to right execution, optimisation can come in the future
	//std::stack<std::shared_ptr<QueryResultsTable>> queryTableStack;

	shared_ptr<QueryResultsTable> selectTable = tables[0]; //always the first one added

	std::vector<string> selectVariables = selectTable->getHeaders();
	tables.erase(tables.begin());
	if (tables.empty()) {
		//it means only a select statement
		//for now no tuples, but need to check here
		vector<string> columns = selectTable->getColumnData(selectTable->getHeaders()[0]); // only one header
		return vectorToList(columns);
	}

	immediateTable = tables[0];
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
				list<string> empty;
				return empty;
			}
			
		}
		if (immediateTable->haveSameHeaders(currTable)) {
			//do inner join
			immediateTable = immediateTable->innerJoin(currTable);
		}
		else {
			//do cross product
			immediateTable = immediateTable->crossProduct(currTable);
		}
	}

	//filter out the required result
	//this is only meant for non tupled results
	if (selectVariables.size() == 1) { // in the future, maybe a isTuple check from parser
		string selectVar = selectVariables[0];

		vector<map<string, vector<string>>> thisColumns = immediateTable->getColumns();
		if (immediateTable->isEmpty()) {
			if (immediateTable->getSignificant()) {
				vector<string> colContents = selectTable->getColumnData(selectTable->getHeaders()[0]);
				return vectorToList(colContents);
			}
			else {
				list<string> empty;
				return empty;
			}

		}
		for (map<string, vector<string>> column : thisColumns) {
			string key = column.begin()->first;
			if (key == selectVar) {
				vector<string> colContents = column.begin()->second;
				return vectorToList(colContents);
			}
		}
		if (immediateTable->getSignificant()) {
			vector<string> colContents = selectTable->getColumns()[0].begin()->second;
			return vectorToList(colContents);
		}
		else {
			list<string> empty;
			return empty;
		}
	}
	else {
		//do next time for tupled/<a,b,c> results, probably cross product of a, b, c
	}

	return result;
}