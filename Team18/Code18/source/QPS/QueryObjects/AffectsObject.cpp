#include "ClauseObject.h"
#include "AffectsFilter.h"
#include "../QRTCache.h"

shared_ptr<QueryResultsTable> ClauseObject::handleAffects(shared_ptr<DataAccessLayer> dataAccessLayer, ABSTRACTION clause) {
	const unordered_set<ENTITY> validEntitiesArg1{ ASSIGN, STMT };
	const unordered_set<ENTITY> validEntitiesArg2{ ASSIGN, STMT };
	if (!isValidSynonymType(validEntitiesArg1, validEntitiesArg2)) {
		return getEmptyTable();
	}
	shared_ptr<QueryEval> clauseHandler = AffectsFilterFactory::create(getArg1(), getArg2());
	shared_ptr<QueryResultsTable> result = clauseHandler->evaluate(dataAccessLayer, clause);
	return result;
}

shared_ptr<QueryResultsTable> AffectsObject::callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer) {
	return handleAffects(dataAccessLayer, NEXT);
}

string AffectsObject::getCacheName() {
	return QRTCache::generateCacheName("AFFECTS",
		{ svToString(getArg1()->getArgValue()), svToString(getArg2()->getArgValue()) }
	);
}

bool AffectsObject::shouldCache() {
	return true;
}

