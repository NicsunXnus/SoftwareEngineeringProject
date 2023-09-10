#pragma once
#include "QueryDriver.h"

#include <list>
#include <string>


using namespace std;

list<string> QueryDriver::execute() {

	try {
		vector<string_view> tokens = tokenize(query);
		shared_ptr<QueryParser> parser = make_shared<QueryParser>();

		tuple<vector<string_view>, vector<string_view>> declarationQuery = parser->splitDeclarationQuery(tokens);

		vector<shared_ptr<QueryObject>> declarationParser = parser->validateDeclaration(get<0>(declarationQuery));
		vector<shared_ptr<QueryObject>> queryParser = parser->validateQuery(get<1>(declarationQuery));
		shared_ptr<DataAccessLayer> dataAccessLayer = make_shared<DataAccessLayer>();


		shared_ptr<QueryObject> obj = queryParser[0];
		//std::cout << "QueryDriver::execute 4.5" << std::endl;
		//std::cout << typeid(*(obj.get())).name() << std::endl;
		obj->call(dataAccessLayer);
		//std::cout << "QueryDriver::execute 5" << std::endl;
		vector<string> result = obj->getResult();
		// chat-gpt code
		list<string> myList(result.begin(), result.end());

		return myList;
	}
	catch (const exception& ex) {
		list<string> empty;
		return empty;
	}
	

}