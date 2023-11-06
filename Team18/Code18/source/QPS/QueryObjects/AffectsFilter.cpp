#include "AffectsFilter.h"
#include "ClauseTableFilter.h"

typedef tuple<string, unordered_set<string>> AffectsStackElement; // parent, set of childs

// returns element in an unordered set, use only if set has one element, else random element from set is returned
static std::string returnSingleElementFromSet(std::unordered_set<std::string> stringSet) {
	if (stringSet.size() <= 0) {
		return "";
	}
	auto it = stringSet.begin();
	std::string onlyElement = *it;
	return onlyElement;
}

unordered_set<string> AffectsFilter::filterAssignmentsInProcedure(unordered_set<string> assignments, int target, shared_ptr<DataAccessLayer> dataAccessLayer) {
	unordered_set<string> procedures = dataAccessLayer->getAllProcedures();
	// first determine the procedure that the target lies in
	string targetProcedure;
	bool procedureExists = false;
	for (string procedure : procedures) {
		pair<string, string> range = dataAccessLayer->getProcLines(procedure);
		int startInt = stoi(range.first);
		int endInt = stoi(range.second);
		if (startInt <= target && target <= endInt) {
			targetProcedure = procedure;
			procedureExists = true;
			break;
		}
	}
	if (!procedureExists) {
		return {};
	}
	pair<string, string> targetRange = dataAccessLayer->getProcLines(targetProcedure);
	int startRange = stoi(targetRange.first);
	int endRange = stoi(targetRange.second);
	unordered_set<string> filteredAssignments;
	for (string assignment : assignments) {
		int assignmentInt = stoi(assignment);
		if (startRange <= assignmentInt && assignmentInt <= endRange) {
			filteredAssignments.insert(assignment);
		}
	}
	return filteredAssignments;
}

/*
* Strictly assume PKB data is correct
* In general, affects logic is as follows:
* Start from an assignment statement (all assignments are modifies / uses, but the converse is not true like read / print/ call)
* Save the variable it modifies
* DFS from this assignment statement, to the other uses statements.
* If variable is modified along the way: terminate, return false
* If uses found for that variable, return true
*/
shared_ptr<QueryResultsTable> AffectsSynSyn::evaluate(shared_ptr<DataAccessLayer> dataAccessLayer, ABSTRACTION clause) {
	shared_ptr<ExtendedCFG> cfg = dataAccessLayer->getCFG();
	StringMap cfgResults = cfg->affectsSynSyn();
	StringMap results = filterMapKeyReturnMap(arg1, dataAccessLayer, cfgResults);
	if (arg1->getArgValue() == arg2->getArgValue()) {
		unordered_set<string> result = filterSynSyn(results);
		shared_ptr<QueryResultsTable> qrt = QueryResultsTable::createTable(svToString(arg1->getArgValue()), result);
		return qrt;
	}
	vector<string> headers({ svToString(arg1->getArgValue()), svToString(arg2->getArgValue()) });
	shared_ptr<QueryResultsTable> qrt = QueryResultsTable::createTable(headers, results);
	return qrt;
}

shared_ptr<QueryResultsTable> AffectsSynWildcard::evaluate(shared_ptr<DataAccessLayer> dataAccessLayer, ABSTRACTION clause) {
	shared_ptr<ExtendedCFG> cfg = dataAccessLayer->getCFG();
	StringMap cfgResults = cfg->affectsSynSyn();
	StringMap results = filterMapKeyReturnMap(arg1, dataAccessLayer, cfgResults);
	unordered_set<string> filteredResults = removeMapValuesReturnSet(arg1, dataAccessLayer, results);
	shared_ptr<QueryResultsTable> qrt = QueryResultsTable::createTable(svToString(arg1->getArgValue()), filteredResults);
	return qrt;
}


shared_ptr<QueryResultsTable> AffectsSynInt::evaluate(shared_ptr<DataAccessLayer> dataAccessLayer, ABSTRACTION clause) {
	shared_ptr<ExtendedCFG> cfg = dataAccessLayer->getCFG();
	unordered_set<string> cfgResults = cfg->affectsSynInt(svToString(arg2->getArgValue()));
	unordered_set<string> results = filterSetReturnSet(arg1, dataAccessLayer, cfgResults);

	shared_ptr<QueryResultsTable> qrt = QueryResultsTable::createTable(svToString(arg1->getArgValue()), results);
	return qrt;
}

shared_ptr<QueryResultsTable> AffectsIntSyn::evaluate(shared_ptr<DataAccessLayer> dataAccessLayer, ABSTRACTION clause) {
	shared_ptr<ExtendedCFG> cfg = dataAccessLayer->getCFG();
	unordered_set<string> cfgResults = cfg->affectsIntSyn(svToString(arg1->getArgValue()));
	unordered_set<string> results = filterSetReturnSet(arg2, dataAccessLayer, cfgResults);
	shared_ptr<QueryResultsTable> qrt = QueryResultsTable::createTable(svToString(arg2->getArgValue()), results);
	return qrt;
}

shared_ptr<QueryResultsTable> AffectsIntWildcard::evaluate(shared_ptr<DataAccessLayer> dataAccessLayer, ABSTRACTION clause) {
	shared_ptr<ExtendedCFG> cfg = dataAccessLayer->getCFG();
	unordered_set<string> cfgResults = cfg->affectsIntSyn(svToString(arg1->getArgValue()));
	shared_ptr<QueryResultsTable> qrt = QueryResultsTable::createEmptyTable(cfgResults.size() > 0);
	return qrt;
}

shared_ptr<QueryResultsTable> AffectsIntInt::evaluate(shared_ptr<DataAccessLayer> dataAccessLayer, ABSTRACTION clause) {
	shared_ptr<ExtendedCFG> cfg = dataAccessLayer->getCFG();
	bool results = cfg->affectsIntInt(svToString(arg1->getArgValue()), svToString(arg2->getArgValue()));
	shared_ptr<QueryResultsTable> qrt = QueryResultsTable::createEmptyTable(results);
	return qrt;
}

shared_ptr<QueryResultsTable> AffectsWildcardSyn::evaluate(shared_ptr<DataAccessLayer> dataAccessLayer, ABSTRACTION clause) {
	shared_ptr<ExtendedCFG> cfg = dataAccessLayer->getCFG();
	StringMap cfgResults = cfg->affectsSynSyn();
	unordered_set<string> results = keepMapValues(cfgResults);
	unordered_set<string> filteredResults = filterSetReturnSet(arg2, dataAccessLayer, results);
	shared_ptr<QueryResultsTable> qrt = QueryResultsTable::createTable(svToString(arg2->getArgValue()), filteredResults);
	return qrt;
}

shared_ptr<QueryResultsTable> AffectsWildcardWildcard::evaluate(shared_ptr<DataAccessLayer> dataAccessLayer, ABSTRACTION clause) {
	shared_ptr<ExtendedCFG> cfg = dataAccessLayer->getCFG();
	StringMap cfgResults = cfg->affectsSynSyn();
	shared_ptr<QueryResultsTable> qrt = QueryResultsTable::createEmptyTable(cfgResults.size() > 0);
	return qrt;
}

shared_ptr<QueryResultsTable> AffectsWildcardInt::evaluate(shared_ptr<DataAccessLayer> dataAccessLayer, ABSTRACTION clause) {
	shared_ptr<ExtendedCFG> cfg = dataAccessLayer->getCFG();
	unordered_set<string> cfgResults = cfg->affectsSynInt(svToString(arg2->getArgValue()));
	shared_ptr<QueryResultsTable> qrt = QueryResultsTable::createEmptyTable(cfgResults.size() > 0);
	return qrt;
}

shared_ptr<QueryEval> AffectsFilterFactory::create(shared_ptr<ClauseArg> argument1, shared_ptr<ClauseArg> argument2) {
	if (argument1->isSynonym() && argument2->isSynonym()) {
		return make_shared<AffectsSynSyn>(argument1, argument2);
	}
	else if (argument1->isSynonym() && argument2->isWildcard()) {
		return make_shared<AffectsSynWildcard>(argument1, argument2);
	}
	else if (argument1->isSynonym() && argument2->isInteger()) {
		return make_shared<AffectsSynInt>(argument1, argument2);
	}
	else if (argument1->isInteger() && argument2->isSynonym()) {
		return make_shared<AffectsIntSyn>(argument1, argument2);
	}
	else if (argument1->isInteger() && argument2->isWildcard()) {
		return make_shared<AffectsIntWildcard>(argument1, argument2);
	}
	else if (argument1->isInteger() && argument2->isInteger()) {
		return make_shared<AffectsIntInt>(argument1, argument2);
	}
	else if (argument1->isWildcard() && argument2->isSynonym()) {
		return make_shared<AffectsWildcardSyn>(argument1, argument2);
	}
	else if (argument1->isWildcard() && argument2->isWildcard()) {
		return make_shared<AffectsWildcardWildcard>(argument1, argument2);
	}
	else if (argument1->isWildcard() && argument2->isInteger()) {
		return make_shared<AffectsWildcardInt>(argument1, argument2);
	}
	throw SemanticErrorException("Error in affects filter factory, invalid argument combination");
}