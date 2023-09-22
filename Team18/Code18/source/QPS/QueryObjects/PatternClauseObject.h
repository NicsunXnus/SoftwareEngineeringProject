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

	shared_ptr<QueryResultsTable> callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer, unordered_map<string_view, shared_ptr<QueryObject>> synonyms) override {
		// Currently patterns only supported for assign
		
		// assign synonym a, variable synonym v, variables "x" "y", constant "1"
		// 9 possible cases.
		// First arg: variable synonym | character string | wildcard
		// Second arg: variable string partial | constant string partial | wildcard
		
		// For now, QueryResultsTable will always have column for assign synonym, and optionally column for variable synonym.

		// 1. pattern a (v, _)
		// Return a, v: for all n in Modifies(n, v) [for all map values] that are in assignments
		// 2. pattern a (v, _"y"_)
		// Return a, v: for all n in Modifies(n, v) [for all map values] that are in assignments and n also in Uses(n, "y")
		// 3. pattern a (v, _"1"_)
		// Return a, v: for all n in Modifies(n, v) [for all map values] that are in assignments and n also in constant database for key "1"
		// 4. pattern a ("x", _)
		// Return a: for all n in Modifies(n, "x") that are in assignments and n also in variable database for key "x"
		// 5. pattern a ("x", _"y"_)
		// Return a: for all n in Modifies(n, "x") that are in assignments and n also in Uses(n, "y")
		// 6. pattern a ("x", _"1"_)
		// Return a: for all n in Modifies(n, "x") that are in assignments and n also in constant database for key "1"
		// 7. pattern a (_, _)
		// Return a: for all n in assignments
		// 8. pattern a (_, _"y"_)
		// Return a: for all n in assignments and n also in Uses(n, "y")
		// 9. pattern a (_, _"1"_)
		// Return a: for all n in assignments and n also in constant database for key "1"

		string assignSynonym = svToString(getPatternSynonym()->getArg());
		shared_ptr<ClauseArg> arg1 = getArg1();
		shared_ptr<ClauseArg> arg2 = getArg2();

		map<string, vector<string>> PKBModifiesData = dataAccessLayer->getClause(MODIFIES);
		map<string, vector<string>> PKBUsesData = dataAccessLayer->getClause(USES);
		vector<string> PKBAssignData = dataAccessLayer->getEntity(ASSIGN);
		map<string, vector<string>> PKBVarData = dataAccessLayer->getVariableMap();
		map<string, vector<string>> PKBConstData = dataAccessLayer->getConstantMap();

		//map<string, vector<string>> PKBModifiesData = { {"x", {"main", "2"}}, {"y", {"main", "3"}}};
		//map<string, vector<string>> PKBUsesData = { {"x", {"main"}}, {"y", {"main", "4"}} };
		//vector<string> PKBAssignData = { "2", "3" };
		//map<string, vector<string>> PKBVarData = { {"x", {"main", "1", "2"}}, {"y", {"main", "3", "4"}} };;
		//map<string, vector<string>> PKBConstData = { {"2", {"2"}}, {"3", {"3"}} };

		shared_ptr<QueryResultsTable> table;

		bool isSingleColumn = true;
		vector<string> assignSynonymColumn;
		map<string, vector<string>> columnValues;

		if (arg1->isWildcard()) {
			//cout << "arg1 is wildcard" << endl;
			// Get all assignment statement numbers
			assignSynonymColumn = PKBAssignData;
		}
		else if (arg1->isExpr()) {
			//cout << "arg1 is expr" << endl;
			// Get all assignment statement numbers that appear in Modifies(n, "x")
			string identifier = svToString(arg1->getIdentifier());
			if (PKBModifiesData.count(identifier)) {
				vector<string> to_intersect = PKBModifiesData.at(identifier);
				assignSynonymColumn = intersection(PKBAssignData, to_intersect);
			}
			else {
				assignSynonymColumn = PKBAssignData;
			}
		}
		else if (arg1->isSynonym()) {
			//cout << "arg1 is synonym" << endl;
			isSingleColumn = false;
			// search Modifies database for all keys, find line numbers in the value (value=lineNum) for each key=variable_key.
			// if these line numbers appear in assignment database, add (v=variable_key, a=lineNum)
			for (const auto& pair : PKBModifiesData) {
				string variable_key = pair.first;
				for (const string& val : pair.second) {
					if (find(PKBAssignData.begin(), PKBAssignData.end(), val) != PKBAssignData.end()) {
						columnValues[variable_key].push_back(val);
					}
				}
			}
		}

		if (arg2->isWildcard()) {}
		else if (arg2->isPartialMatchingExprSpec()) {
			// differentiate between constant and variable
			string identifier = svToString(arg2->getIdentifier());

			if (isNumber(identifier)) {  // constant
				//cout << "arg2 is constant" << endl;
				if (isSingleColumn) {
					// Get all assignment statement numbers that appear in constant database with constant as key
					if (PKBConstData.count(identifier)) {
						vector<string> to_intersect = PKBConstData.at(identifier);
						assignSynonymColumn = intersection(assignSynonymColumn, to_intersect);
					}
					else {
						assignSynonymColumn = {};
					}
				}
				else {
					if (PKBConstData.count(identifier)) {
						vector<string> to_intersect = PKBConstData.at(identifier);
						for (auto pair = columnValues.begin(); pair != columnValues.end();) {
							string variable_key = pair->first;
							vector<string> intersect = intersection(columnValues[variable_key], to_intersect);
							if (intersect.size() == 0) {
								pair = columnValues.erase(pair);
							}
							else {
								columnValues[variable_key] = intersect;
								++pair;
							}
						}
					}
					else {
						columnValues = {};
					}
				}
			}
			else {  // variable
				//cout << "arg2 is variable" << endl;
				if (isSingleColumn) {
					// Get all assignment statement numbers that appear in variable database with variable as key
					if (PKBVarData.count(identifier)) {
						vector<string> to_intersect = PKBUsesData.at(identifier);
						assignSynonymColumn = intersection(assignSynonymColumn, to_intersect);
					}
					else {
						assignSynonymColumn = {};
					}
				}
				else {
					if (PKBVarData.count(identifier)) {
						vector<string> to_intersect = PKBUsesData.at(identifier);
						for (auto pair = columnValues.begin(); pair != columnValues.end();) {
							string variable_key = pair->first;
							vector<string> intersect = intersection(columnValues[variable_key], to_intersect);
							if (intersect.size() == 0) {
								pair = columnValues.erase(pair);
							}
							else {
								columnValues[variable_key] = intersect;
								++pair;
							}
						}
					}
					else {
						columnValues = {};
					}
				}
			}
		}

		if (isSingleColumn) {
			table = QueryResultsTable::createTable(assignSynonym, assignSynonymColumn);
			//cout << "FINAL ANSWER:" << endl;
			//printVectorString(assignSynonymColumn);
		}
		else {
			vector<string> headers = { svToString(arg1->getArg()), assignSynonym };
			table = QueryResultsTable::createTable(headers, columnValues);
			//cout << "FINAL ANSWER:" << endl;
			//printMap(columnValues);
		}
		
		return table;
	}



	// Gets the intersect of two vectors
	vector<string> intersection(vector<string>& strings1, vector<string>& strings2) {
		unordered_set<string> m(strings1.begin(), strings1.end());
		vector<string> res;
		for (auto a : strings2)
			if (m.count(a)) {
				res.push_back(a);
				m.erase(a);
			}
		return res;
	}

	// debugging
	void printVectorString(vector<string> v) {
		for (const string& element : v) {
			cout << element << endl;
		}
	}

	// debugging
	void printMap(map<string, vector<string>> m) {
		for (const auto& pair : m) {
			string result;
			for (const string& num : pair.second) {
				result += num;
			}
			cout << "Key: " << pair.first << ", Value: " << result << endl;
		}
	}
};
#endif