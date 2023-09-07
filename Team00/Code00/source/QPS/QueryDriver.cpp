#include "QueryDriver.h"
#include "QueryParser.h"
#include "../TokenizerClasses/PQLTokenizer.h"
#include "DataAccessLayer.h"
#include <list>
#include <string>


using namespace std;

list<string> QueryDriver::execute() {

	try {
		vector<string_view> tokenizer = tokenize(query);
		shared_ptr<QueryParser> parser = make_shared<QueryParser>();

		tuple<vector<string_view>, vector<string_view>> declarationQuery = parser->splitDeclarationQuery(tokenizer);

		vector<shared_ptr<QueryObject>> declarationParser = parser->validateDeclaration(get<0>(declarationQuery));
		vector<shared_ptr<QueryObject>> queryParser = parser->validateQuery(get<1>(declarationQuery));
		shared_ptr<DataAccessLayer> dataAccessLayer = make_shared<DataAccessLayer>();


		shared_ptr<QueryObject> obj = queryParser[0];
		obj->call(dataAccessLayer);
		vector<int> result = obj->getResult();
		list<int> myList(result.begin(), result.end());
		list<string> stringList;
		for (const int& intValue : myList) {
			// Convert each integer to a string and append it to the stringList
			stringList.push_back(std::to_string(intValue));
		}

		return stringList;
	}
	catch (const exception& ex) {
		list<string> empty;
		return empty;
	}
	

}