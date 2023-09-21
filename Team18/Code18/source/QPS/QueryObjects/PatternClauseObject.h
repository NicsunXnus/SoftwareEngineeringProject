#ifndef PATTERNOBJECT_H
#define PATTERNOBJECT_H

#include <string_view>
#include "QueryObject.h"
#include "ClauseArg.h"

/*
* This class represents a Pattern clause object, for pattern entities
*/
class PatternObject : public QueryObject {
private:
	map<string, vector<string>> res;
	vector<shared_ptr<ClauseArg>> arguments;

public:
	PatternObject(string_view clauseName, vector<shared_ptr<ClauseArg>> arguments)
		: QueryObject{ "pattern"sv }, arguments{ arguments } {}

	shared_ptr<ClauseArg> getPatternSynonym() {
		return arguments[0];
	}

	shared_ptr<ClauseArg> getArg1() {
		return arguments[1];
	}

	shared_ptr<ClauseArg> getArg2() {
		return arguments[2];
	}

	// TODO: impleme
	void call(shared_ptr<DataAccessLayer> dataAccessLayer) {};

	// variant: design entities, clauses
	// Setting the results of the PKB call
	void setResult(variant<vector<string>, map<string, vector<string>>> result) {}

	variant<vector<string>, map<string, vector<string>>> getResult() {
		return res;
	}
};
#endif