#include "ClauseObject.h"

// ensure empty table for invalid queries. a query like Select c such that Modifies(c, v), if modifies table has line 1, 2, and
// program has constants 1, 2, will return non empty table
inline bool isValidSynonymType(shared_ptr<ClauseArg> arg1, shared_ptr<ClauseArg> arg2, ABSTRACTION clause) {
	
	if (clause == MODIFIES) {
		const unordered_set<ENTITY> validEntitiesArg1{ STMT, READ, ASSIGN, CALL, WHILE, IF, PROCEDURE };
		const unordered_set<ENTITY> validEntitiesArg2{ VARIABLE };
		if (arg1->isSynonym() && validEntitiesArg1.find(arg1->getSynonym()->getEntityType()) == validEntitiesArg1.end()) {
			return false;
		}
		if (arg2->isSynonym() && validEntitiesArg2.find(arg2->getSynonym()->getEntityType()) == validEntitiesArg2.end()) {
			return false;
		}
	}
	if (clause == USES) {
		const unordered_set<ENTITY> validEntitiesArg1{ STMT, PRINT, ASSIGN, CALL, WHILE, IF, PROCEDURE };
		const unordered_set<ENTITY> validEntitiesArg2{ VARIABLE };
		if (arg1->isSynonym() && validEntitiesArg1.find(arg1->getSynonym()->getEntityType()) == validEntitiesArg1.end()) {
			return false;
		}
		if (arg2->isSynonym() && validEntitiesArg2.find(arg2->getSynonym()->getEntityType()) == validEntitiesArg2.end()) {
			return false;
		}
	}
	return true;
}

inline shared_ptr<QueryResultsTable> handleUsesModifies(shared_ptr<ClauseArg> arg1, shared_ptr<ClauseArg> arg2, shared_ptr<DataAccessLayer> dataAccessLayer, ABSTRACTION clause) {
	if (!isValidSynonymType(arg1, arg2, clause)) {  // arg2 must be a variable synonym
		return make_shared<QueryResultsTable>();
	}
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
	else if (arg1->isInteger() && arg2->isSynonym()) {
		StringMap PKBClauseData = dataAccessLayer->getClause(clause);
		unordered_set<string> filteredPKBClauseDataArg1 = filterMapKeyReturnSetValues(arg1, dataAccessLayer, PKBClauseData);
		unordered_set<string> filteredPKBClauseDataArg2 = filterSetReturnSet(arg2, dataAccessLayer, filteredPKBClauseDataArg1);
		return QueryResultsTable::createTable(svToString(arg2->getArgValue()), filteredPKBClauseDataArg2);
	}
	else if (arg1->isInteger() && arg2->isWildcard()) {
		StringMap PKBClauseData = dataAccessLayer->getClause(clause);

		unordered_set<string> filteredPKBClauseDataArg1 = filterMapKeyReturnSetValues(arg1, dataAccessLayer, PKBClauseData);
		shared_ptr<QueryResultsTable> table = make_shared<QueryResultsTable>();
		table->setSignificant(filteredPKBClauseDataArg1.size() > 0);
		return table;
	}
	else if (arg1->isInteger() && arg2->isIdentifier()) {
		StringMap PKBClauseData = dataAccessLayer->getClause(clause);

		unordered_set<string> filteredPKBClauseDataArg1 = filterMapKeyReturnSetValues(arg1, dataAccessLayer, PKBClauseData);
		bool filteredPKBClauseDataArg2 = filterSetReturnBool(arg2, dataAccessLayer, filteredPKBClauseDataArg1);
		shared_ptr<QueryResultsTable> table = make_shared<QueryResultsTable>();
		table->setSignificant(filteredPKBClauseDataArg2);
		return table;
	}
	// ent refs as arg1, for usesP and modifiesP
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

shared_ptr<QueryResultsTable> UsesObject::callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer, unordered_map<string_view, shared_ptr<QueryObject>> synonyms) {
	return handleUsesModifies(getArg1(), getArg2(), dataAccessLayer, USES);
}

shared_ptr<QueryResultsTable> UsesEntityObject::callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer, unordered_map<string_view, shared_ptr<QueryObject>> synonyms) {
	return handleUsesModifies(getArg1(), getArg2(), dataAccessLayer, USES);
}

shared_ptr<QueryResultsTable> ModifiesObject::callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer, unordered_map<string_view, shared_ptr<QueryObject>> synonyms) {
	return handleUsesModifies(getArg1(), getArg2(), dataAccessLayer, MODIFIES);
}

shared_ptr<QueryResultsTable> ModifiesEntityObject::callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer, unordered_map<string_view, shared_ptr<QueryObject>> synonyms) {
	return handleUsesModifies(getArg1(), getArg2(), dataAccessLayer, MODIFIES);
}