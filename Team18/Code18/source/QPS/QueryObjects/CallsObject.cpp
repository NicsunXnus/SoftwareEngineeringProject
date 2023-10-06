#include "ClauseObject.h"

inline shared_ptr<QueryResultsTable> handleCallsCallsStar(shared_ptr<ClauseArg> arg1, shared_ptr<ClauseArg> arg2, shared_ptr<DataAccessLayer> dataAccessLayer, ABSTRACTION clause) {
	if (arg1->isSynonym() && arg2->isSynonym()) {
		StringMap PKBClauseData = dataAccessLayer->getClause(clause);

		StringMap filteredPKBClauseDataArg1 = filterMapKeyReturnMap(arg1, dataAccessLayer, PKBClauseData);
		StringMap filteredPKBClauseDataArg2 = filterMapValueReturnMap(arg2, dataAccessLayer, filteredPKBClauseDataArg1);

		vector<string> headers({ svToString(arg1->getArgValue()), svToString(arg2->getArgValue()) });
		return QueryResultsTable::createTable(headers, filteredPKBClauseDataArg2);
	}
	else if (arg1->isSynonym() && arg2->isWildcard()) {
		StringMap PKBClauseData = dataAccessLayer->getClause(clause);
		StringMap filteredPKBClauseDataArg1 = filterMapKeyReturnMap(arg1, dataAccessLayer, PKBClauseData);
		unordered_set<string> filteredPKBClauseDataArg2 = removeMapValuesReturnSet(arg2, dataAccessLayer, filteredPKBClauseDataArg1);
		return QueryResultsTable::createTable(svToString(arg1->getArgValue()), filteredPKBClauseDataArg2);
	}
	else if (arg1->isSynonym() && arg2->isIdentifier()) {
		StringMap PKBClauseData = dataAccessLayer->getClauseInverse(clause);
		unordered_set<string> filteredPKBClauseDataArg2 = filterMapKeyReturnSetValues(arg2, dataAccessLayer, PKBClauseData);
		unordered_set<string> filteredPKBClauseDataArg1 = filterSetReturnSet(arg1, dataAccessLayer, filteredPKBClauseDataArg2);
		return QueryResultsTable::createTable(svToString(arg1->getArgValue()), filteredPKBClauseDataArg1);
	}
	else if (arg1->isWildcard() && arg2->isSynonym()) {
		StringMap PKBClauseData = dataAccessLayer->getClauseInverse(clause);
		StringMap filteredPKBClauseDataArg2 = filterMapKeyReturnMap(arg2, dataAccessLayer, PKBClauseData);
		unordered_set<string> filteredPKBClauseDataArg1 = removeMapValuesReturnSet(arg1, dataAccessLayer, filteredPKBClauseDataArg2);
		return QueryResultsTable::createTable(svToString(arg2->getArgValue()), filteredPKBClauseDataArg1);
	}
	else if (arg1->isWildcard() && arg2->isWildcard()) {
		StringMap PKBClauseData = dataAccessLayer->getClause(clause);
		shared_ptr<QueryResultsTable> table = make_shared<QueryResultsTable>();
		table->setSignificant(PKBClauseData.size() > 0);
		return table;
	}
	else if (arg1->isWildcard() && arg2->isIdentifier()) {
		StringMap PKBClauseData = dataAccessLayer->getClauseInverse(clause);
		unordered_set<string> filteredPKBClauseDataArg2 = filterMapKeyReturnSetValues(arg2, dataAccessLayer, PKBClauseData);
		shared_ptr<QueryResultsTable> table = make_shared<QueryResultsTable>();
		table->setSignificant(filteredPKBClauseDataArg2.size() > 0);
		return table;
	}
	else if (arg1->isIdentifier() && arg2->isSynonym()) {
		StringMap PKBClauseData = dataAccessLayer->getClause(clause);
		unordered_set<string> filteredPKBClauseDataArg1 = filterMapKeyReturnSetValues(arg1, dataAccessLayer, PKBClauseData);
		unordered_set<string> filteredPKBClauseDataArg2 = filterSetReturnSet(arg2, dataAccessLayer, filteredPKBClauseDataArg1);
		return QueryResultsTable::createTable(svToString(arg2->getArgValue()), filteredPKBClauseDataArg2);

	}
	else if (arg1->isIdentifier() && arg2->isWildcard()) {
		StringMap PKBClauseData = dataAccessLayer->getClause(clause);

		unordered_set<string> filteredPKBClauseDataArg1 = filterMapKeyReturnSetValues(arg1, dataAccessLayer, PKBClauseData);
		shared_ptr<QueryResultsTable> table = make_shared<QueryResultsTable>();
		table->setSignificant(filteredPKBClauseDataArg1.size() > 0);
		return table;
	}
	else if (arg1->isIdentifier() && arg2->isIdentifier()) {
		StringMap PKBClauseData = dataAccessLayer->getClause(clause);

		unordered_set<string> filteredPKBClauseDataArg1 = filterMapKeyReturnSetValues(arg1, dataAccessLayer, PKBClauseData);
		bool filteredPKBClauseDataArg2 = filterSetReturnBool(arg2, dataAccessLayer, filteredPKBClauseDataArg1);
		shared_ptr<QueryResultsTable> table = make_shared<QueryResultsTable>();
		table->setSignificant(filteredPKBClauseDataArg2);
		return table;
	}
	throw SemanticErrorException("Error evaluating clause: " + clause);
}

shared_ptr<QueryResultsTable> CallsObject::callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer, unordered_map<string_view, shared_ptr<QueryObject>> synonyms) {
	return handleCallsCallsStar(getArg1(), getArg2(), dataAccessLayer, CALLS);
}

shared_ptr<QueryResultsTable> CallsStarObject::callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer, unordered_map<string_view, shared_ptr<QueryObject>> synonyms) {
	return handleCallsCallsStar(getArg1(), getArg2(), dataAccessLayer, CALLSSTAR);
}
