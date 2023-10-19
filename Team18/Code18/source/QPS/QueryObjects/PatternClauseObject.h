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

	shared_ptr<QueryResultsTable> callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer) override {
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

		StringMap PKBModifiesData = dataAccessLayer->getClauseInverse(MODIFIES);
		StringMap PKBUsesData = dataAccessLayer->getClauseInverse(USES);
		unordered_set<string> PKBAssignData = dataAccessLayer->getEntity(ASSIGN);
		StringMap PKBVarData = dataAccessLayer->getVariableMap();
		StringMap PKBConstData = dataAccessLayer->getConstantMap();

		shared_ptr<QueryResultsTable> table;

		bool isSingleColumn = true;
		unordered_set<string> assignSynonymColumn;
		StringMap columnValues;

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
				unordered_set<string> to_intersect = PKBModifiesData.at(identifier);
				assignSynonymColumn = get_intersection(PKBAssignData, to_intersect);
			}
			else {
				assignSynonymColumn = {};
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
						columnValues[variable_key].insert(val);
					}
				}
			}
		}

		if (arg2->isWildcard()) {}
		else if (arg2->isExpr()) {
			// differentiate between constant and variable
			string identifier = svToString(arg2->getIdentifier());

			if (isNumber(identifier)) {  // constant
				//cout << "arg2 is constant" << endl;
				if (isSingleColumn) {
					// Get all assignment statement numbers that appear in constant database with constant as key
					if (PKBConstData.count(identifier)) {
						unordered_set<string> to_intersect = PKBConstData.at(identifier);
						assignSynonymColumn = get_intersection(assignSynonymColumn, to_intersect);
					}
					else {
						assignSynonymColumn = {};
					}
				}
				else {
					if (PKBConstData.count(identifier)) {
						unordered_set<string> to_intersect = PKBConstData.at(identifier);
						for (auto pair = columnValues.begin(); pair != columnValues.end();) {
							string variable_key = pair->first;
							unordered_set<string> intersect = get_intersection(columnValues[variable_key], to_intersect);
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
						unordered_set<string> to_intersect = PKBUsesData.at(identifier);
						assignSynonymColumn = get_intersection(assignSynonymColumn, to_intersect);
					}
					else {
						assignSynonymColumn = {};
					}
				}
				else {
					if (PKBVarData.count(identifier)) {
						unordered_set<string> to_intersect = PKBUsesData.at(identifier);
						for (auto pair = columnValues.begin(); pair != columnValues.end();) {
							string variable_key = pair->first;
							unordered_set<string> intersect = get_intersection(columnValues[variable_key], to_intersect);
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
			//printVectorString(assignSynonymColumn);
		}
		else {
			vector<string> headers = { svToString(arg1->getArg()), assignSynonym };
			table = QueryResultsTable::createTable(headers, columnValues);
			//printMap(columnValues);
		}
		
		return table;
	}
};
#endif