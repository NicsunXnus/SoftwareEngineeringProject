#pragma once
#include "QueryDriver.h"
#include "QueryBuilder.h"
#include "QueryResultsTable.h"
#include "ResultsHandler.h"

#include <list>
#include <string>
#include "Errors/QPSError.h"


using namespace std;

list<string> QueryDriver::execute() {

	try {
		std::cout << "In Query Driver, starting tokenizer\n";

		vector<std::string> tokens = tokenize(query);

		shared_ptr<QueryParser> parser = make_shared<QueryParser>();

		vector<std::string_view> tokensView{ sToSvVector(tokens) };

		tuple<vector<string_view>, vector<string_view>> declarationQuery = parser->splitDeclarationQuery(tokensView);

		vector<shared_ptr<QueryObject>> declarationParser = parser->validateDeclaration(get<0>(declarationQuery));

		std::cout << "In Query Driver, starting validateQuery\n";

		vector<shared_ptr<QueryObject>> queryObjects = parser->validateQuery(get<1>(declarationQuery));
		unordered_map<string_view, shared_ptr<QueryObject>> synonyms = parser->getSynonyms();
		shared_ptr<DataAccessLayer> dataAccessLayer = make_shared<DataAccessLayer>();
		shared_ptr<QueryBuilder> queryBuilder = make_shared<QueryBuilder>(queryObjects, synonyms, dataAccessLayer);
		

		//shared_ptr<QueryObject> obj = queryParser[0]; // sprint 1
		/*For QueryBuilder passage*/
		//shared_ptr<QueryBuilder> queryBuilderObj = make_shared<QueryBuilder>();
		//vector<shared_ptr<QueryResultsTable>> queryResultsTable = queryBuilderObj->buildQuery(queryParser);

		//shared_ptr<ResultHandler> resultHandlerObj = make_shared<ResultHandler>();
		//list<string> resultOfCrossQueryExamination = resultHandlerObj->processTables(queryResultsTable);
		//return resultOfCrossQueryExamination;
		// 

		//std::cout << "QueryDriver::execute 4.5" << std::endl;
		//std::cout << typeid(*(obj.get())).name() << std::endl;
		/*obj->call(dataAccessLayer);
		variant<vector<string>, map<string, vector<string>>> resultVariant = obj->getResult();
		vector<string> result = get<vector<string>>(resultVariant);*/

		// chat-gpt code
		list<string> myList;
		/*variant<vector<string>, vector<string, vector<string>>> test;
		vector<string> ta = get<vector<string>>(test);*/
		
		return myList;
	}
	catch (const QPSError& ex) {
		list<string> empty;
		empty.push_back(ex.getType());
		return empty;
	}
	

}