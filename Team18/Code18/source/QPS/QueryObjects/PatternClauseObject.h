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
		 
		// Get all assignment statement numbers
		
		// Check cases: assign synonym a, variable synonym v, variables "x" "y", constant "1"
		// 9 possible cases.
		// First arg: variable synonym | character string | wildcard
		// Second arg: variable string partial | constant string partial | wildcard
		
		// For now, QueryResultsTable will always have column for assign synonym, and optionally column for variable synonym.

		// 1. pattern a (v, _)
		// Return a, v: for all n in Modifies(n, v) [for all map values] that are in assignments
		
		// 2. pattern a (v, _"y"_)
		// Return a, v: for all n in Modifies(n, v) [for all map values] that are in assignments and n also in variable database for key "y"
		
		// 3. pattern a (v, _"1"_)
		// Return a, v: for all n in Modifies(n, v) [for all map values] that are in assignments and n also in constant database for key "1"
		
		// 4. pattern a ("x", _)
		// Return a: for all n in Modifies(n, "x") that are in assignments and n also in variable database for key "x"

		// 5. pattern a ("x", _"y"_)
		// Return a: for all n in Modifies(n, "x") that are in assignments and n also in variable database for key "y"
		
		// 6. pattern a ("x", _"1"_)
		// Return a: for all n in Modifies(n, "x") that are in assignments and n also in constant database for key "1"
		
		// 7. pattern a (_, _)
		// Return a: for all n in assignments
		
		// 8. pattern a (_, _"y"_)
		// Return a: for all n in assignments and n also in variable database for key "y"
		
		// 9. pattern a (_, _"1"_)
		// Return a: for all n in assignments and n also in constant database for key "1"



		cout <<"here"<<endl;
		shared_ptr<ClauseArg> arg1 = getArg1();
		shared_ptr<ClauseArg> arg2 = getArg2();

		cout << "pattern syn " << getPatternSynonym()->getArg() << endl;
		cout << "arg1" << arg1->getArg() << endl;
		cout << "arg2" << arg2->getArg() << endl;

		if (arg1->isWildcard()) {
			cout << "arg1 is wildcard" << endl;
		}
		else if (arg1->isInteger()) {
			cout << "arg1 is integer" << endl;
		}
		else if (arg1->isExpr()) {
			cout << "arg1 is expr" << endl;
		}
		else {
			cout << "arg1 is none" << endl;
		}

		map<string, vector<string>> PKBModifiesData = dataAccessLayer->getClause(MODIFIES);
		vector<string> PKBAssignData = dataAccessLayer->getEntity(ASSIGN);
		//map<string, vector<string>> PKBVarData = dataAccessLayer->getVariableMap();
		//map<string, vector<string>> PKBConstData = dataAccessLayer->getConstantMap();

		vector<string> headers;
		
		headers.push_back(col1);
		headers.push_back(col2);
		
		// create table with temporary name table headers: col1, col2
		//shared_ptr<QueryResultsTable> table = QueryResultsTable::createTable(headers, PKBdata);


		//shared_ptr<QueryResultsTable> filterFirstArg = filterStmtRef(getArg1(), col2, table, dataAccessLayer, synonyms);
		//shared_ptr<QueryResultsTable> filterSecondArg = filterEntRef(getArg2(), col1, filterFirstArg, dataAccessLayer, synonyms);
		//return filterSecondArg;
		return nullptr;
	}

	// variant: design entities, clauses
	// Setting the results of the PKB call
	void setResult(variant<vector<string>, map<string, vector<string>>> result) {
		//this->res = result;
	}

	variant<vector<string>, map<string, vector<string>>> getResult() {
		return res;
	}
};
#endif