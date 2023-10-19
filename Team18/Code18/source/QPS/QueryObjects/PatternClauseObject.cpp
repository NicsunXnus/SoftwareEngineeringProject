#include "PatternClauseObject.h"

inline shared_ptr<QueryResultsTable> callAndProcessIfWhile(shared_ptr<DataAccessLayer> dataAccessLayer,
	std::string synonym, shared_ptr<ClauseArg> arg1) {

	bool isSingleColumn = true;
	unordered_set<string> synonymColumn;
	StringMap columnValues;
	shared_ptr<QueryResultsTable> table;

	// Get PKB data
	unordered_set<string> PKBIfData = dataAccessLayer->getEntity(IF);
	unordered_set<string> PKBWhileData = dataAccessLayer->getEntity(WHILE);
	unordered_set<string> PKBVarData = dataAccessLayer->getAllVariables();

	// Determine if the synonym is if or while
	if (PKBIfData.count(synonym)) {
		cout << "synonym is if" << endl;
		// Get all if statement numbers
		synonymColumn = PKBIfData;
	}
	else if (PKBWhileData.count(synonym)) {
		cout << "synonym is while" << endl;
		// Get all while statement numbers
		synonymColumn = PKBWhileData;
	}
	else {
		synonymColumn = {};
		table = QueryResultsTable::createTable(synonym, synonymColumn);
		// printVectorString(synonymColumn);
		return table;
	}

	// Only refer to arg1
	if (arg1->isWildcard()) {}  // no further filtering
	else if (arg1->isExpr()) {  // variable name in quotes
		string identifier = svToString(arg1->getIdentifier());

		// Return empty one column table if "x" not in variable list (early termination)
		if (PKBVarData.count(identifier) == 0) {
			synonymColumn = {};
			table = QueryResultsTable::createTable(synonym, synonymColumn);
			// printVectorString(synonymColumn);
			return table;
		}
		unordered_set<string> intersecting = {};
		for (const string& lineNum : synonymColumn) {
			/**
			shared_ptr<Node> lineTree = dataAccessLayer->getPatternTree(lineNum);
			// get all terminal variables in the pattern tree
			// check if variable is a terminal variable
			unordered_set<string> terminals = lineTree->getTerminalVariables();
			if (terminals.count(identifier)) {
				intersecting.insert(lineNum);
			}
			*/
		}
		synonymColumn = intersecting;
	}
	else if (arg1->isSynonym()) {  // variable synonym
		isSingleColumn = false;
		// for all if/while statements
		for (const string& lineNum : synonymColumn) {
			/**
			shared_ptr<Node> lineTree = dataAccessLayer->getPatternTree(lineNum);
			// get all terminal variables in the pattern tree
			unordered_set<string> terminals = lineTree->getTerminalVariables();
			// add (k=if/while statement number, v=variable syn) to columnValues
			columnValues[lineNum] = terminals;
			*/
		}
	}

	if (isSingleColumn) {
		table = QueryResultsTable::createTable(synonym, synonymColumn);
		//printVectorString(synonymColumn);
	}
	else {
		vector<string> headers = { synonym, svToString(arg1->getArg()) };  // 1st arg is if/while line number, 2nd arg is variable syn
		table = QueryResultsTable::createTable(headers, columnValues);
		//printMap(columnValues);
	}

	return table;
}

shared_ptr<QueryResultsTable> IfPatternObject::callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer) {
	string synonym = svToString(getPatternSynonym()->getArg());
	shared_ptr<ClauseArg> arg1 = getArg1();
	return callAndProcessIfWhile(dataAccessLayer, synonym, arg1);
}

shared_ptr<QueryResultsTable> WhilePatternObject::callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer) {
	string synonym = svToString(getPatternSynonym()->getArg());
	shared_ptr<ClauseArg> arg1 = getArg1();
	return callAndProcessIfWhile(dataAccessLayer, synonym, arg1);
}

