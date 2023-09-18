#pragma once
#include "QueryDriver.h"

#include <list>
#include <string>
#include "Errors/QPSError.h"
#include "../HelperFunctions.h"


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

		vector<shared_ptr<QueryObject>> queryParser = parser->validateQuery(get<1>(declarationQuery));
		shared_ptr<DataAccessLayer> dataAccessLayer = make_shared<DataAccessLayer>();


		shared_ptr<QueryObject> obj = queryParser[0];
		/*For QueryBuilder passage*/
		//vector<QueryResultsTable> queryResultsTable = QueryBulder::buildQuery(queryParser);
		// list<string> resultOfCrossQueryExamination = ResultHandler::processQueryResultsTable(queryResultsTable);
		// return resultOfCrossQueryExamination;
		// 

		//std::cout << "QueryDriver::execute 4.5" << std::endl;
		//std::cout << typeid(*(obj.get())).name() << std::endl;
		obj->call(dataAccessLayer);
		variant<vector<string>, map<string, vector<string>>> resultVariant = obj->getResult();
		vector<string> result = get<vector<string>>(resultVariant);

		// chat-gpt code
		list<string> myList(result.begin(), result.end());
		/*variant<vector<string>, vector<string, vector<string>>> test;
		vector<string> ta = get<vector<string>>(test);*/

		return myList;
	}
	catch (const QPSError& ex) {
		list<string> empty;
		debug(ex.what());
		empty.push_back(ex.getType());
		return empty;
	}
	

}