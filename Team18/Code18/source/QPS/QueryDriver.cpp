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
#include "PQLTokenizer.h"


using namespace std;

list<string> QueryDriver::execute() {

	try {
		cout << "In Query Driver, starting tokenizer\n";
		cout << "Query: " << query << '\n';

		vector<string> tokens = PQLTokenizer::tokenize(query);
		shared_ptr<QueryParser> parser = make_shared<QueryParser>();
		vector<string_view> tokensView{ sToSvVector(tokens) };

		vector<shared_ptr<QueryObject>> queryObjects = parser->parsePQL(tokensView);
		vector<shared_ptr<QueryObject>> selectClauseQueryObjects = parser->getSelectClauseQueryObject(queryObjects);
		vector<shared_ptr<QueryObject>> nonSelectClauseQueryObjects = parser->getNonSelectClauseQueryObject(queryObjects);
		shared_ptr<DataAccessLayer> dataAccessLayer = make_shared<DataAccessLayer>();
		shared_ptr<QueryBuilder> queryBuilder = make_shared<QueryBuilder>(selectClauseQueryObjects, nonSelectClauseQueryObjects, dataAccessLayer);

		vector<shared_ptr<QueryResultsTable>> queryResultsTableSelectClause = queryBuilder->buildQuerySelectClause();
		vector<shared_ptr<QueryResultsTable>> queryResultsTableNonSelect = queryBuilder->buildQuery();

		shared_ptr<ResultHandler> resultHandler = make_shared<ResultHandler>();
		list<string> finalResult = resultHandler->processTables(queryResultsTableSelectClause, queryResultsTableNonSelect);
		
		return finalResult;
	}
	catch (const QPSError& ex) {
		list<string> empty;
		debug(ex.what());
		empty.push_back(ex.getType());
		return empty;
	}
	

}