#include "ResultsHandler.h"
#include <stack>
list<string> ResultHandler::processTables(std::vector<std::shared_ptr<QueryResultsTable>> tables) {
	list<string> result;
	shared_ptr<QueryResultsTable> immediateTable;

	//for now we do brute force left to right execution, optimisation can come in the future
	std::stack<std::shared_ptr<QueryResultsTable>> queryTableStack;

	shared_ptr<QueryResultsTable> selectTable = tables[0]; //always the first one added
	std::vector<string> selectVariables = selectTable->getHeaders();
	tables.erase(tables.begin());
	for (std::shared_ptr<QueryResultsTable> table : tables) {
		queryTableStack.push(std::move(table));
	}
	immediateTable = queryTableStack.top();
	queryTableStack.pop();

	shared_ptr<QueryResultsTable> currTable;
	while (!queryTableStack.empty()) {
		currTable = queryTableStack.top();
		queryTableStack.pop();
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
	if (selectVariables.size() == 1) {
		string selectVar = selectVariables[0];
		vector<map<string, vector<string>>> thisColumns = immediateTable->getColumns();
		for (map<string, vector<string>> column : thisColumns) {
			string key = column.begin()->first;
			if (key == selectVar) {
				vector<string> colContents = column.begin()->second;
				list<string> result(colContents.begin(), colContents.end());
				return result;
			}
		}
		cerr << "Cannot find symbol mentioned in select statement";
	}
	else {
		//do next time for tupled/<a,b,c> results
	}

	return result;
}