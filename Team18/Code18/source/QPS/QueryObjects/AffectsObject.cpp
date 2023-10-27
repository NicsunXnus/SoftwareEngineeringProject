#include "ClauseObject.h"
#include "AffectsFilter.h"

shared_ptr<QueryResultsTable> ClauseObject::handleAffects(shared_ptr<DataAccessLayer> dataAccessLayer, ABSTRACTION clause) {
	const unordered_set<ENTITY> validEntitiesArg1{ ASSIGN };
	const unordered_set<ENTITY> validEntitiesArg2{ ASSIGN };
	if (!isValidSynonymType(validEntitiesArg1, validEntitiesArg2)) {
		return QueryResultsTable::createEmptyTable();
	}
	shared_ptr<QueryEval> clauseHandler = AffectsFilterFactory::create(getArg1(), getArg2());
	shared_ptr<QueryResultsTable> result = clauseHandler->evaluate(dataAccessLayer, clause);
	return result;
}



shared_ptr<QueryResultsTable> AffectsObject::callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer) {
	return handleAffects(dataAccessLayer, NEXT);
}

