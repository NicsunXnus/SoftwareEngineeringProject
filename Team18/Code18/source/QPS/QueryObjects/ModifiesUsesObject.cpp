#include "ClauseObject.h"
#include "ClauseTableFilter.h"

shared_ptr<QueryResultsTable> ClauseObject::handleUses(shared_ptr<DataAccessLayer> dataAccessLayer, ABSTRACTION clause) {
	const unordered_set<ENTITY> validEntitiesArg1{ STMT, PRINT, ASSIGN, CALL, WHILE, IF, PROCEDURE };
	const unordered_set<ENTITY> validEntitiesArg2{ VARIABLE };
	if (!isValidSynonymType(validEntitiesArg1, validEntitiesArg2)) {
		return getEmptyTable();
	}
	shared_ptr<QueryEval> clauseHandler = ClauseFilterFactory::create(getArg1(), getArg2());
	shared_ptr<QueryResultsTable> result = clauseHandler->evaluate(dataAccessLayer, clause);
	return result;
}

shared_ptr<QueryResultsTable> ClauseObject::handleModifies(shared_ptr<DataAccessLayer> dataAccessLayer, ABSTRACTION clause) {
	const unordered_set<ENTITY> validEntitiesArg1{ STMT, READ, ASSIGN, CALL, WHILE, IF, PROCEDURE };
	const unordered_set<ENTITY> validEntitiesArg2{ VARIABLE };
	if (!isValidSynonymType(validEntitiesArg1, validEntitiesArg2)) {
		return QueryResultsTable::createEmptyTable();
	}
	shared_ptr<QueryEval> clauseHandler = ClauseFilterFactory::create(getArg1(), getArg2());
	shared_ptr<QueryResultsTable> result = clauseHandler->evaluate(dataAccessLayer, clause);
	return result;
}

shared_ptr<QueryResultsTable> UsesObject::callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer) {
	return handleUses(dataAccessLayer, USES);
}

shared_ptr<QueryResultsTable> UsesEntityObject::callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer) {
	return handleUses(dataAccessLayer, USES);
}

shared_ptr<QueryResultsTable> ModifiesObject::callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer) {
	return handleModifies(dataAccessLayer, MODIFIES);
}

shared_ptr<QueryResultsTable> ModifiesEntityObject::callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer) {
	return handleModifies(dataAccessLayer, MODIFIES);
}