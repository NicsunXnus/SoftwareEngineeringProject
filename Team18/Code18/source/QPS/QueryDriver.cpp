#pragma once
#include "QueryDriver.h"
#include "QueryBuilder.h"
#include "QueryResultsTable.h"
#include "ResultsHandler.h"

#include <list>
#include <string>
#include <set>
#include "Errors/QPSError.h"
#include "../HelperFunctions.h"


using namespace std;

list<string> QueryDriver::execute() {

	try {
		std::cout << "In Query Driver, starting tokenizer\n";
		vector<std::string> tokens = tokenize(query);
		shared_ptr<QueryParser> parser = make_shared<QueryParser>();
		vector<std::string_view> tokensView{ sToSvVector(tokens) };

		vector<shared_ptr<QueryObject>> queryObjects = parser->parsePQL(tokensView);
		unordered_map<string_view, shared_ptr<QueryObject>> synonyms = parser->getSynonyms();
		shared_ptr<DataAccessLayer> dataAccessLayer = make_shared<DataAccessLayer>();
		shared_ptr<QueryBuilder> queryBuilder = make_shared<QueryBuilder>(queryObjects, synonyms, dataAccessLayer);
		
		vector<shared_ptr<QueryResultsTable>> queryResultsTable = queryBuilder->buildQuery();
		shared_ptr<ResultHandler> resultHandler = make_shared<ResultHandler>();
		list<string> finalResult = resultHandler->processTables(queryResultsTable);

		set<string> uniqueStrings;
		for (const string& str : finalResult) {
			uniqueStrings.insert(str);
		}

		list<string> stringList;
		for (const string& str : uniqueStrings) {
			stringList.push_back(str);
		}
		
		return stringList;
	}
	catch (const QPSError& ex) {
		list<string> empty;
		debug(ex.what());
		empty.push_back(ex.getType());
		return empty;
	}
	

}