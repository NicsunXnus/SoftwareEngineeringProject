#ifndef PATTERNOBJECT_H
#define PATTERNOBJECT_H

#include <string>
#include <string_view>
#include <unordered_set>

#include "../../SP/AST/Node.h"
#include "ClauseArg.h"
#include "QueryObject.h"

/*
 * This class represents a Pattern clause object, for pattern entities
 */
class PatternObject : public QueryObject {
 protected:
  vector<shared_ptr<ClauseArg>> arguments;

 public:
  PatternObject(string_view clauseName,
                vector<shared_ptr<ClauseArg>> const& arguments)
      : QueryObject{clauseName}, arguments{arguments} {}

  shared_ptr<ClauseArg> getPatternSynonym() { return arguments[0]; }

  shared_ptr<ClauseArg> getArg1() { return arguments[1]; }

  shared_ptr<ClauseArg> getArg2() { return arguments[2]; }
  virtual shared_ptr<QueryResultsTable> callAndProcess(
      shared_ptr<DataAccessLayer> dataAccessLayer) = 0;
};

/*
 * This class represents an Assign Pattern clause object
 */
class AssignPatternObject : public PatternObject {
 public:
  AssignPatternObject(string_view clauseName,
                      vector<shared_ptr<ClauseArg>> const& arguments)
      : PatternObject{clauseName, arguments} {}

  shared_ptr<QueryResultsTable> callAndProcess(
      shared_ptr<DataAccessLayer> dataAccessLayer) override;

  unordered_set<string> findPatternTreeMatch(
      bool isPartialMatch, unordered_set<string> stmtsToCheck,
      shared_ptr<Node> patternTree,
      shared_ptr<DataAccessLayer> dataAccessLayer);
};

/*
 * This class represents an If Pattern clause object
 */
class IfPatternObject : public PatternObject {
 public:
  IfPatternObject(string_view clauseName,
                  vector<shared_ptr<ClauseArg>> const& arguments)
      : PatternObject{clauseName, arguments} {}
  shared_ptr<QueryResultsTable> callAndProcess(
      shared_ptr<DataAccessLayer> dataAccessLayer) override;
};

/*
 * This class represents a While Pattern clause object
 */
class WhilePatternObject : public PatternObject {
 public:
  WhilePatternObject(string_view clauseName,
                     vector<shared_ptr<ClauseArg>> const& arguments)
      : PatternObject{clauseName, arguments} {}
  shared_ptr<QueryResultsTable> callAndProcess(
      shared_ptr<DataAccessLayer> dataAccessLayer) override;
};
#endif