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

/**

class IfWhilePatternObject : public PatternObject {
public:
        IfWhilePatternObject(string_view clauseName,
vector<shared_ptr<ClauseArg>> const& arguments) : PatternObject{ clauseName ,
arguments } {}

        // TODO: Implement in Node.cpp Node::getTerminalVariables()
        // need a method to return all terminal variable nodes in the pattern
tree. (no constants!)

        // MILESTONE 2 IMPLEMENTATION:
        // If/While
        // 1. pattern ifs(_, _, _)
        // Return if: for all if in ifs
        // 2. pattern ifs(v, _, _)
        // Return if, v: for all if in ifs and v is one of all terminal
variables in the pattern tree of this if
        // 3. pattern ifs("x", _, _)
        // Return empty one column table if "x" not in variable database
        // Return if: for all if in ifs where "x" also a terminal variable in
the pattern tree of this if
        // (same for while)

        shared_ptr<QueryResultsTable> callAndProcess(shared_ptr<DataAccessLayer>
dataAccessLayer, unordered_map<string_view, shared_ptr<QueryObject>> synonyms)
override {

                string synonym = svToString(getPatternSynonym()->getArg());
                shared_ptr<ClauseArg> arg1 = getArg1();

                bool isSingleColumn = true;
                unordered_set<string> synonymColumn;
                StringMap columnValues;
                shared_ptr<QueryResultsTable> table;

                // Get PKB data
                unordered_set<string> PKBIfData =
dataAccessLayer->getEntity(IF); unordered_set<string> PKBWhileData =
dataAccessLayer->getEntity(WHILE); unordered_set<string> PKBVarData =
dataAccessLayer->getAllVariables();

                // Determine if the synonym is if or while
                if (PKBIfData.count(synonym)) {
                        cout << "synonym is if" << endl;
                        // Get all if statement numbers
                        synonymColumn = PKBIfData;
                }
                else if (PKBWhileData.count(synonym)) {
                        cout << "synonym is while" << endl;
                        // Get all while statement numbers
                        synonymColumn = PKBWhileData;
                }
                else {
                        synonymColumn = {};
                        table = QueryResultsTable::createTable(synonym,
synonymColumn);
                        // printVectorString(synonymColumn);
                        return table;
                }

                // Only refer to arg1
                if (arg1->isWildcard()) {}  // no further filtering
                else if (arg1->isExpr()) {  // variable name in quotes
                        string identifier = svToString(arg1->getIdentifier());

                        // Return empty one column table if "x" not in variable
list (early termination) if (PKBVarData.count(identifier) == 0) { synonymColumn
= {}; table = QueryResultsTable::createTable(synonym, synonymColumn);
                                // printVectorString(synonymColumn);
                                return table;
                        }
                        unordered_set<string> intersecting = {};
                        for (const string& lineNum : synonymColumn) {
                                shared_ptr<Node> lineTree =
dataAccessLayer->getPatternTree(lineNum);
                                // get all terminal variables in the pattern
tree
                                // check if variable is a terminal variable
                                unordered_set<string> terminals =
lineTree->getTerminalVariables(); if (terminals.count(identifier)) {
                                        intersecting.insert(lineNum);
                                }
                        }
                        synonymColumn = intersecting;
                }
                else if (arg1->isSynonym()) {  // variable synonym
                        isSingleColumn = false;
                        // for all if/while statements
                        for (const string& lineNum : synonymColumn) {
                                shared_ptr<Node> lineTree =
dataAccessLayer->getPatternTree(lineNum);
                                // get all terminal variables in the pattern
tree unordered_set<string> terminals = lineTree->getTerminalVariables();
                                // add (k=if/while statement number, v=variable
syn) to columnValues columnValues[lineNum] = terminals;
                        }
                }

                if (isSingleColumn) {
                        table = QueryResultsTable::createTable(synonym,
synonymColumn);
                        //printVectorString(synonymColumn);
                }
                else {
                        vector<string> headers = { synonym,
svToString(arg1->getArg()) };  // 1st arg is if/while line number, 2nd arg is
variable syn table = QueryResultsTable::createTable(headers, columnValues);
                        //printMap(columnValues);
                }

                return table;
        }
};
*/

#endif