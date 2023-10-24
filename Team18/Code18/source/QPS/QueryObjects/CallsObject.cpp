#include "ClauseObject.h"
#include "ClauseTableFilter.h"

inline shared_ptr<QueryResultsTable> handleCallsCallsStar(shared_ptr<ClauseArg> arg1, shared_ptr<ClauseArg> arg2, shared_ptr<DataAccessLayer> dataAccessLayer, ABSTRACTION clause) {
	shared_ptr<ClauseFilterHandler> clauseHandler = ClauseFilterFactory::create(arg1, arg2);
	shared_ptr<QueryResultsTable> result = clauseHandler->evaluate(dataAccessLayer, clause);
	return result;
}

shared_ptr<QueryResultsTable> CallsObject::callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer) {
	const unordered_set<ENTITY> validEntitiesArg1{ PROCEDURE };
	const unordered_set<ENTITY> validEntitiesArg2{ PROCEDURE };
	if (!isValidSynonymType(validEntitiesArg1, validEntitiesArg2)) {
		return QueryResultsTable::createEmptyTable();
	}
	return handleCallsCallsStar(getArg1(), getArg2(), dataAccessLayer, CALLS);
}

shared_ptr<QueryResultsTable> CallsStarObject::callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer) {
	const unordered_set<ENTITY> validEntitiesArg1{ PROCEDURE };
	const unordered_set<ENTITY> validEntitiesArg2{ PROCEDURE };
	if (!isValidSynonymType(validEntitiesArg1, validEntitiesArg2)) {
		return QueryResultsTable::createEmptyTable();
	}
	return handleCallsCallsStar(getArg1(), getArg2(), dataAccessLayer, CALLSSTAR);
}
