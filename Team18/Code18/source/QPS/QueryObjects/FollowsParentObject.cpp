#include "ClauseObject.h"
#include "ClauseTableFilter.h"

inline shared_ptr<QueryResultsTable> handleFollowsParents(shared_ptr<ClauseArg> arg1, shared_ptr<ClauseArg> arg2, shared_ptr<DataAccessLayer> dataAccessLayer, ABSTRACTION clause) {
	shared_ptr<ClauseFilterHandler> clauseHandler = ClauseFilterFactory::create(arg1, arg2);
	shared_ptr<QueryResultsTable> result = clauseHandler->evaluate(dataAccessLayer, clause);
	return result;
}

shared_ptr<QueryResultsTable> FollowsObject::callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer) {
	return handleFollowsParents(getArg1(), getArg2(), dataAccessLayer, FOLLOWS);
}

shared_ptr<QueryResultsTable> FollowsStarObject::callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer) {
	return handleFollowsParents(getArg1(), getArg2(), dataAccessLayer, FOLLOWSSTAR);
}

shared_ptr<QueryResultsTable> ParentObject::callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer) {
	return handleFollowsParents(getArg1(), getArg2(), dataAccessLayer, PARENT);
}

shared_ptr<QueryResultsTable> ParentStarObject::callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer) {
	return handleFollowsParents(getArg1(), getArg2(), dataAccessLayer, PARENTSTAR);
}