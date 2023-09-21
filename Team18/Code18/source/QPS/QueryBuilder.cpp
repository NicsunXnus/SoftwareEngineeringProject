#pragma once

#include "QueryBuilder.h"
#include "QueryObjects/DesignObjects.h"
#include "QueryObjects/ClauseObject.h"
using namespace std;

//Takes in a Query object and then returns a vector of QueryResultsTable. This vector will
//be pased to ResultHandler to process the various tables of the clauses

vector<shared_ptr<QueryResultsTable>> QueryBuilder::buildQuery() {

	vector<shared_ptr<QueryResultsTable>> queryResultsTables;
	//shared_ptr<DataAccessLayer> dataAccessLayer = make_shared<DataAccessLayer>();
	for (shared_ptr<QueryObject> obj : queryObjects) {
		shared_ptr<QueryResultsTable> table = obj->callAndProcess(dataAccessLayer, synonyms);
		queryResultsTables.push_back(table);
		//obj->call(dataAccessLayer);
		//If it is a select query, a vector<string> is returned,
		auto designEntity = std::dynamic_pointer_cast<DesignObject>(obj);
		auto clauseEntity = std::dynamic_pointer_cast<ClauseObject>(obj);
		if (designEntity) {
			/*variant<vector<string>, map<string, vector<string>>> resultVariant = obj->getResult();
			vector<string> result = get<vector<string>>(resultVariant);
			if (result.empty()) {
				cerr << "Empty Select Statement Query Result";
			}
			map<string, vector<string>> columnWithHeader;
			columnWithHeader.insert({string(obj->getQueryObjectName()), result});
			vector<map<string, vector<string>>> table;
			table.emplace_back(columnWithHeader);
			shared_ptr<QueryResultsTable> selectQueryTable = make_shared<QueryResultsTable>(table);
			queryResultsTables.emplace_back(selectQueryTable);*/
		}
		//else if it is a modifies,follows,uses,parent query, a map<string,vector<string>> is returned
		else if (clauseEntity) {
			/*map<string,vector<string>> result = obj->getResult();
			if (result.empty()) {
				//should just return nothing since it is the result of a false clause
				//short circuit the process and return an empty list
				vector<shared_ptr<QueryResultsTable>> emptyQueryResultsTables;
				return emptyQueryResultsTables;
			}
			vector<string> column1;
			vector<string> column2;

			for (const auto& [key, vector] : result) {
				for (string value : vector) {
					column1.emplace_back(key);
					column2.emplace_back(vector);
				}
			}

			map<string, vector<string>> columnWithHeader1;
			map<string, vector<string>> columnWithHeader2;
			columnWithHeader1.insert({obj->getArg1()->getArg(), column1});
			columnWithHeader2.insert({obj->getArg2()->getArg(), column2});
			vector<map<string, vector<string>>> table;
			table.emplace_back(columnWithHeader1);
			table.emplace_back(columnWithHeader2);*/
		} else {
			cerr << "Unknown Entity detected. Please debug.";
		}	
	}
	//sort(queryResultsTables.begin(), queryResultsTables.end());
	return queryResultsTables;
}
