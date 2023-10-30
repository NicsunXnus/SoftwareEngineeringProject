#include "ClauseObject.h"
#include "ClauseTableFilter.h"

shared_ptr<QueryResultsTable> ClauseObject::handleCallsCallsStar(shared_ptr<DataAccessLayer> dataAccessLayer, ABSTRACTION clause) {
	const unordered_set<ENTITY> validEntitiesArg1{ PROCEDURE };
	const unordered_set<ENTITY> validEntitiesArg2{ PROCEDURE };
	if (!isValidSynonymType(validEntitiesArg1, validEntitiesArg2)) {
		return QueryResultsTable::createEmptyTable();
	}
	shared_ptr<QueryEval> clauseHandler = ClauseFilterFactory::create(getArg1(), getArg2());
	shared_ptr<QueryResultsTable> result = clauseHandler->evaluate(dataAccessLayer, clause);
	return result;
}

shared_ptr<QueryResultsTable> CallsObject::callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer) {
	return handleCallsCallsStar(dataAccessLayer, CALLS);
}

shared_ptr<QueryResultsTable> CallsStarObject::callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer) {
	return handleCallsCallsStar(dataAccessLayer, CALLSSTAR);
}
