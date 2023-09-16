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
	map<string, vector<string>> res;
	shared_ptr<ClauseArg> argument0;
	shared_ptr<ClauseArg> argument1;

public:
	InvalidQueryObject(string_view message) : QueryObject{ message } {}

	void setResult(variant<vector<string>, map<string, vector<string>>> result) override {
		res = get<map<string, vector<string>>>(result);
	}

	std::variant<vector<string>, map<string, vector<string>>> getResult() override {
		return res;
	}

	void call(shared_ptr<DataAccessLayer> dataAccessLayer) override {
		std::vector<string> emptyVec;
		variant<vector<string>, map<string, vector<string>>> var{ emptyVec };

		setResult(var);
	}
};

#endif
