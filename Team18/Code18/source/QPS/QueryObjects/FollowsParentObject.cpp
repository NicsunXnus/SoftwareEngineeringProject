#include "ClauseObject.h"
#include "ClauseTableFilter.h"

shared_ptr<QueryResultsTable> ClauseObject::handleFollowsParents(
    shared_ptr<DataAccessLayer> dataAccessLayer, ABSTRACTION clause) {
  shared_ptr<QueryEval> clauseHandler =
      ClauseFilterFactory::create(getArg1(), getArg2());
  shared_ptr<QueryResultsTable> result =
      clauseHandler->evaluate(dataAccessLayer, clause);
  return result;
}

shared_ptr<QueryResultsTable> FollowsObject::callAndProcess(
    shared_ptr<DataAccessLayer> dataAccessLayer) {
  return handleFollowsParents(dataAccessLayer, FOLLOWS);
}

shared_ptr<QueryResultsTable> FollowsStarObject::callAndProcess(
    shared_ptr<DataAccessLayer> dataAccessLayer) {
  return handleFollowsParents(dataAccessLayer, FOLLOWSSTAR);
}

shared_ptr<QueryResultsTable> ParentObject::callAndProcess(
    shared_ptr<DataAccessLayer> dataAccessLayer) {
  return handleFollowsParents(dataAccessLayer, PARENT);
}

shared_ptr<QueryResultsTable> ParentStarObject::callAndProcess(
    shared_ptr<DataAccessLayer> dataAccessLayer) {
  return handleFollowsParents(dataAccessLayer, PARENTSTAR);
}