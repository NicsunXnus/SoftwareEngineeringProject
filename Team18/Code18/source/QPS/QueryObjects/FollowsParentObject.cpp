#include "ClauseObject.h"

inline shared_ptr<QueryResultsTable> handleFollowsParents(shared_ptr<ClauseArg> arg1, shared_ptr<ClauseArg> arg2, shared_ptr<DataAccessLayer> dataAccessLayer, ABSTRACTION clause) {
	if (arg1->isSynonym() && arg2->isSynonym()) {
		if (arg1->getArg() == arg2->getArg()) {
			return make_shared<QueryResultsTable>();
		}
		StringMap PKBClauseData = dataAccessLayer->getClause(clause);

		StringMap filteredPKBClauseDataArg1 = handleSynonymKeyColumn(arg1, dataAccessLayer, PKBClauseData);
		StringMap filteredPKBClauseDataArg2 = handleSynonymSetColumn(arg2, dataAccessLayer, filteredPKBClauseDataArg1);

		vector<string> headers({ svToString(arg1->getArg()), svToString(arg2->getArg()) });
		return QueryResultsTable::createTable(headers, filteredPKBClauseDataArg2);
	}
	else if (arg1->isSynonym() && arg2->isInteger()) {
		StringMap PKBClauseData = dataAccessLayer->getClauseInverse(clause);
		unordered_set<string> filteredPKBClauseDataArg2 = handleIntegerKeyColumn(arg2, dataAccessLayer, PKBClauseData);
		unordered_set<string> filteredPKBClauseDataArg1 = handleSynonymSet(arg1, dataAccessLayer, filteredPKBClauseDataArg2);
		return QueryResultsTable::createTable(svToString(arg1->getArg()), filteredPKBClauseDataArg1);
	}
	else if (arg2->isSynonym() && arg1->isInteger()) {
		StringMap PKBClauseData = dataAccessLayer->getClause(clause);
		unordered_set<string> filteredPKBClauseDataArg1 = handleIntegerKeyColumn(arg1, dataAccessLayer, PKBClauseData);
		unordered_set<string> filteredPKBClauseDataArg2 = handleSynonymSet(arg2, dataAccessLayer, filteredPKBClauseDataArg1);
		return QueryResultsTable::createTable(svToString(arg2->getArg()), filteredPKBClauseDataArg2);

	}
	else if (arg1->isSynonym() && arg2->isWildcard()) {
		StringMap PKBClauseData = dataAccessLayer->getClause(clause);
		StringMap filteredPKBClauseDataArg1 = handleSynonymKeyColumn(arg1, dataAccessLayer, PKBClauseData);
		unordered_set<string> filteredPKBClauseDataArg2 = handleWildCardDropSetColumn(arg2, dataAccessLayer, filteredPKBClauseDataArg1);
		return QueryResultsTable::createTable(svToString(arg1->getArg()), filteredPKBClauseDataArg2);
	}
	else if (arg2->isSynonym() && arg1->isWildcard()) {
		StringMap PKBClauseData = dataAccessLayer->getClauseInverse(clause);
		StringMap filteredPKBClauseDataArg2 = handleSynonymKeyColumn(arg2, dataAccessLayer, PKBClauseData);
		unordered_set<string> filteredPKBClauseDataArg1 = handleWildCardDropSetColumn(arg1, dataAccessLayer, filteredPKBClauseDataArg2);
		return QueryResultsTable::createTable(svToString(arg2->getArg()), filteredPKBClauseDataArg1);
	}
	else if (arg1->isInteger() && arg2->isWildcard()) {
		StringMap PKBClauseData = dataAccessLayer->getClause(clause);

		unordered_set<string> filteredPKBClauseDataArg1 = handleIntegerKeyColumn(arg1, dataAccessLayer, PKBClauseData);
		shared_ptr<QueryResultsTable> table = make_shared<QueryResultsTable>();
		table->setSignificant(filteredPKBClauseDataArg1.size() > 0);
		return table;
	}
	else if (arg2->isInteger() && arg1->isWildcard()) {
		StringMap PKBClauseData = dataAccessLayer->getClauseInverse(clause);
		unordered_set<string> filteredPKBClauseDataArg2 = handleIntegerKeyColumn(arg2, dataAccessLayer, PKBClauseData);
		shared_ptr<QueryResultsTable> table = make_shared<QueryResultsTable>();
		table->setSignificant(filteredPKBClauseDataArg2.size() > 0);
		return table;

	}
	else if (arg1->isInteger() && arg2->isInteger()) {
		StringMap PKBClauseData = dataAccessLayer->getClause(clause);

		unordered_set<string> filteredPKBClauseDataArg1 = handleIntegerKeyColumn(arg1, dataAccessLayer, PKBClauseData);
		bool filteredPKBClauseDataArg2 = handleIntegerSet(arg2, dataAccessLayer, filteredPKBClauseDataArg1);
		shared_ptr<QueryResultsTable> table = make_shared<QueryResultsTable>();
		table->setSignificant(filteredPKBClauseDataArg2);
		return table;
	}
	else { // wildcard, wildcard
		StringMap PKBClauseData = dataAccessLayer->getClause(clause);
		shared_ptr<QueryResultsTable> table = make_shared<QueryResultsTable>();
		table->setSignificant(PKBClauseData.size() > 0);
		return table;
	}
}

shared_ptr<QueryResultsTable> FollowsObject::callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer, unordered_map<string_view, shared_ptr<QueryObject>> synonyms) {
	shared_ptr<ClauseArg> arg1 = getArg1();
	shared_ptr<ClauseArg> arg2 = getArg2();

	return handleFollowsParents(arg1, arg2, dataAccessLayer, FOLLOWS);
}

shared_ptr<QueryResultsTable> FollowsStarObject::callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer, unordered_map<string_view, shared_ptr<QueryObject>> synonyms) {
	shared_ptr<ClauseArg> arg1 = getArg1();
	shared_ptr<ClauseArg> arg2 = getArg2();

	return handleFollowsParents(arg1, arg2, dataAccessLayer, FOLLOWSSTAR);
}

shared_ptr<QueryResultsTable> ParentObject::callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer, unordered_map<string_view, shared_ptr<QueryObject>> synonyms) {
	shared_ptr<ClauseArg> arg1 = getArg1();
	shared_ptr<ClauseArg> arg2 = getArg2();

	return handleFollowsParents(arg1, arg2, dataAccessLayer, PARENT);
}

shared_ptr<QueryResultsTable> ParentStarObject::callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer, unordered_map<string_view, shared_ptr<QueryObject>> synonyms) {
	shared_ptr<ClauseArg> arg1 = getArg1();
	shared_ptr<ClauseArg> arg2 = getArg2();

	return handleFollowsParents(arg1, arg2, dataAccessLayer, PARENTSTAR);
}