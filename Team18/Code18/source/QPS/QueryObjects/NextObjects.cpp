#include "ClauseObject.h"
#include "ClauseTableFilter.h"
#include "NextStarFilter.h"
#include <stack>
#include <tuple>
#include "../QRTCache.h"

shared_ptr<QueryResultsTable> ClauseObject::handleNext(shared_ptr<DataAccessLayer> dataAccessLayer, ABSTRACTION clause) {
	shared_ptr<QueryEval> clauseHandler = ClauseFilterFactory::create(getArg1(), getArg2());
	shared_ptr<QueryResultsTable> result = clauseHandler->evaluate(dataAccessLayer, clause);
	return result;
}


shared_ptr<QueryResultsTable> ClauseObject::handleNextStar(shared_ptr<DataAccessLayer> dataAccessLayer, ABSTRACTION clause) {
	shared_ptr<QueryEval> clauseHandler = NextStarFilterFactory::create(getArg1(), getArg2());
	shared_ptr<QueryResultsTable> result = clauseHandler->evaluate(dataAccessLayer, clause);
	return result;
}

string NextStarObject::getCacheName() {
	return QRTCache::generateCacheName("NEXT*",
		{svToString(getArg1()->getArgValue()), svToString(getArg2()->getArgValue())}
	);
}

bool NextStarObject::shouldCache() {
	return true;
}

shared_ptr<QueryResultsTable> NextObject::callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer) {
	return handleNext(dataAccessLayer, NEXT);
}

shared_ptr<QueryResultsTable> NextStarObject::callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer) {
        return handleNextStar(dataAccessLayer, NEXT);
}
