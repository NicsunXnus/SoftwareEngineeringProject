#ifndef INVALIDQUERYOBJECT_H
#define INVALIDQUERYOBJECT_H

#include <string_view>
#include <variant>
#include "QueryObject.h"
#include "ClauseArg.h"


/*
* This class represents a Query object, for clause entities
*/
class InvalidQueryObject : public QueryObject {
private:
	unordered_map<string, vector<string>> res;
	shared_ptr<ClauseArg> argument0;
	shared_ptr<ClauseArg> argument1;

public:
	InvalidQueryObject(string_view message) : QueryObject{ message } {}

	/*void setResult(variant<vector<string>, unordered_map<string, vector<string>>> result) override {
		res = get<unordered_map<string, vector<string>>>(result);
	}

	variant<vector<string>, unordered_map<string, vector<string>>> getResult() override {
		return res;
	}*/

	//shared_ptr<QueryResultsTable> call(shared_ptr<DataAccessLayer> dataAccessLayer) override {
	//	vector<string> emptyVec;
	//	variant<vector<string>, unordered_map<string, vector<string>>> var{ emptyVec };

	//	//setResult(var);
	//}

	//shared_ptr<QueryResultsTable> filter(shared_ptr<QueryResultsTable> table, unordered_unordered_map<string_view, shared_ptr<QueryObject>> synonyms) {
	//	shared_ptr<QueryResultsTable> test;
	//	return test;
	//}
};

#endif
