#include "ClauseObject.h"
#include "ClauseTableFilter.h"

inline shared_ptr<QueryResultsTable> handleUsesModifies(shared_ptr<ClauseArg> arg1, shared_ptr<ClauseArg> arg2, shared_ptr<DataAccessLayer> dataAccessLayer, ABSTRACTION clause) {
	
	shared_ptr<ClauseFilterHandler> clauseHandler = ClauseFilterFactory::create(arg1, arg2);
	shared_ptr<QueryResultsTable> result = clauseHandler->evaluate(dataAccessLayer, clause);
	return result;
}

shared_ptr<QueryResultsTable> UsesObject::callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer) {
	const unordered_set<ENTITY> validEntitiesArg1{ STMT, PRINT, ASSIGN, CALL, WHILE, IF, PROCEDURE };
	const unordered_set<ENTITY> validEntitiesArg2{ VARIABLE };
	if (!isValidSynonymType(validEntitiesArg1, validEntitiesArg2)) {
		return QueryResultsTable::createEmptyTable();
	}
	return handleUsesModifies(getArg1(), getArg2(), dataAccessLayer, USES);
}

shared_ptr<QueryResultsTable> UsesEntityObject::callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer) {
	const unordered_set<ENTITY> validEntitiesArg1{ STMT, PRINT, ASSIGN, CALL, WHILE, IF, PROCEDURE };
	const unordered_set<ENTITY> validEntitiesArg2{ VARIABLE };
	if (!isValidSynonymType(validEntitiesArg1, validEntitiesArg2)) {
		return QueryResultsTable::createEmptyTable();
	}
	return handleUsesModifies(getArg1(), getArg2(), dataAccessLayer, USES);
}

shared_ptr<QueryResultsTable> ModifiesObject::callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer) {
	const unordered_set<ENTITY> validEntitiesArg1{ STMT, READ, ASSIGN, CALL, WHILE, IF, PROCEDURE };
	const unordered_set<ENTITY> validEntitiesArg2{ VARIABLE };
	if (!isValidSynonymType(validEntitiesArg1, validEntitiesArg2)) {
		return QueryResultsTable::createEmptyTable();
	}
	return handleUsesModifies(getArg1(), getArg2(), dataAccessLayer, MODIFIES);
}

shared_ptr<QueryResultsTable> ModifiesEntityObject::callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer) {
	const unordered_set<ENTITY> validEntitiesArg1{ STMT, READ, ASSIGN, CALL, WHILE, IF, PROCEDURE };
	const unordered_set<ENTITY> validEntitiesArg2{ VARIABLE };
	if (!isValidSynonymType(validEntitiesArg1, validEntitiesArg2)) {
		return QueryResultsTable::createEmptyTable();
	}
	return handleUsesModifies(getArg1(), getArg2(), dataAccessLayer, MODIFIES);
}