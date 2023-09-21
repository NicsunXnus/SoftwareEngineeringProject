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

	string col1 = "col1";
	string col2 = "col2";

	shared_ptr<QueryResultsTable> callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer, unordered_map<string_view, shared_ptr<QueryObject>> synonyms) override {
		// Currently patterns only supported for assign
		
		map<string, vector<string>> PKBUsesData = dataAccessLayer->getClause(USES);
		map<string, vector<string>> PKBModifiesData = dataAccessLayer->getClause(MODIFIES);
		vector<string> headers;
		
		headers.push_back(col1);
		headers.push_back(col2);
		
		// create table with temporary name table headers: col1, col2
		//shared_ptr<QueryResultsTable> table = QueryResultsTable::createTable(headers, PKBdata);


		//shared_ptr<QueryResultsTable> filterFirstArg = filterStmtRef(getArg1(), col2, table, dataAccessLayer, synonyms);
		//shared_ptr<QueryResultsTable> filterSecondArg = filterEntRef(getArg2(), col1, filterFirstArg, dataAccessLayer, synonyms);
		//return filterSecondArg;
	}

	// variant: design entities, clauses
	// Setting the results of the PKB call
	void setResult(variant<vector<string>, map<string, vector<string>>> result) {}

	variant<vector<string>, map<string, vector<string>>> getResult() {
		return res;
	}
};
#endif