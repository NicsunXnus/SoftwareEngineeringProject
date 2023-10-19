#include "PatternClauseObject.h"

#include "../../SP/SimpleProcessor/ExpressionProcessor.h"

shared_ptr<QueryResultsTable> AssignPatternObject::callAndProcess(
    shared_ptr<DataAccessLayer> dataAccessLayer) {
  // Currently patterns only supported for assign

  // assign synonym a, variable synonym v, variables "x" "y", constant "1"
  // 9 possible cases.
  // First arg: variable synonym | character string | wildcard
  // Second arg: variable string partial | constant string partial | wildcard

  // QueryResultsTable will always have column for assign synonym, and
  // optionally column for variable synonym.

  string synonym = svToString(getPatternSynonym()->getArg());
  shared_ptr<ClauseArg> arg1 = getArg1();
  shared_ptr<ClauseArg> arg2 = getArg2();

  bool isSingleColumn = true;
  unordered_set<string> synonymColumn;
  StringMap columnValues;
  shared_ptr<QueryResultsTable> table;

  // Get PKB data
  unordered_set<string> PKBAssignData = dataAccessLayer->getEntity(ASSIGN);
  StringMap PKBModifiesData =
      dataAccessLayer->getClauseInverse(MODIFIES);  // inverse
  unordered_set<string> PKBVarData = dataAccessLayer->getAllVariables();

  if (arg1->isWildcard()) {
    // Get all assignment statement numbers
    synonymColumn = PKBAssignData;
  } else if (arg1->isExpr()) {
    string identifier = svToString(arg1->getIdentifier());

    // Return empty one column table if "x" not in variable list (early
    // termination)
    if (PKBVarData.count(identifier) == 0) {
      synonymColumn = {};
      table = QueryResultsTable::createTable(synonym, synonymColumn);
      // printVectorString(synonymColumn);
      return table;
    }

    // Get all assignment statement numbers that appear in Modifies(n, "x")
    if (PKBModifiesData.count(identifier)) {
      unordered_set<string> to_intersect = PKBModifiesData.at(identifier);
      synonymColumn = get_intersection(PKBAssignData, to_intersect);
    } else {
      synonymColumn = {};
    }
  } else if (arg1->isSynonym()) {
    isSingleColumn = false;
    // search Modifies database for all keys, find line numbers in the value
    // (value=lineNum) for each key=variable_key. if these line numbers appear
    // in assignment database, add (v=variable_key, a=lineNum)
    for (const auto& pair : PKBModifiesData) {
      string variable_key = pair.first;
      for (const string& lineNum : pair.second) {
        if (PKBAssignData.count(lineNum)) {
          columnValues[variable_key].insert(lineNum);
        }
      }
    }
  }

  if (arg2->isWildcard()) {
  }                           // no further filtering
  else if (arg2->isExpr()) {  // partial or full match.
    // build pattern tree
    string expression = svToString(arg2->getIdentifier());
    shared_ptr<Node> patternTree;
    try {
      ExpressionProcessor ep = ExpressionProcessor();
      patternTree = ep.nodifyArithmeticExpression("a * b / 3 % 5");
    } catch (invalid_argument e) {
      synonymColumn = {};
      table = QueryResultsTable::createTable(synonym, synonymColumn);
      // printVectorString(synonymColumn);
      return table;
    }
    unordered_set<string> to_intersect = {};
    // compare pattern tree with pattern tree of each assignment statement
    if (arg2->isPartialMatchingExprSpec()) {
      // for each assignment statement, check if pattern tree is subtree of
      // pattern tree of this assignment
      for (const string& lineNum : synonymColumn) {
        shared_ptr<Node> lineTree = dataAccessLayer->getPatternTree(lineNum);
        if (patternTree->isSubtreeOf(lineTree)) {
          to_intersect.insert(lineNum);
        }
      }
    } else {  // full match
      // for each assignment statement, check if pattern tree is identical to
      // pattern tree of this assignment
      for (const string& lineNum : synonymColumn) {
        shared_ptr<Node> lineTree = dataAccessLayer->getPatternTree(lineNum);
        if (patternTree->isIdentical(lineTree)) {
          to_intersect.insert(lineNum);
        }
      }
    }

    // updating single column and double column tables
    if (isSingleColumn) {
      synonymColumn = get_intersection(synonymColumn, to_intersect);
    } else {
      for (auto pair = columnValues.begin();
           pair != columnValues.end();) {  // loop through variable keys
        string variable_key = pair->first;
        unordered_set<string> intersect =
            get_intersection(columnValues[variable_key], to_intersect);
        if (intersect.size() == 0) {
          pair = columnValues.erase(pair);
        } else {
          columnValues[variable_key] = intersect;
          ++pair;
        }
      }
    }
  }

  if (isSingleColumn) {
    table = QueryResultsTable::createTable(synonym, synonymColumn);
    // printVectorString(synonymColumn);
  } else {
    // arg1 is variable synonym, arg2 is assign line number
    vector<string> headers = {svToString(arg1->getArg()), synonym};
    table = QueryResultsTable::createTable(headers, columnValues);
    // printMap(columnValues);
  }

  return table;
}

inline shared_ptr<QueryResultsTable> callAndProcessIfWhile(
    shared_ptr<DataAccessLayer> dataAccessLayer, std::string synonym,
    shared_ptr<ClauseArg> arg1) {
  // If/While
  // 1. pattern ifs(_, _, _)
  // Return if: for all if in ifs
  // 2. pattern ifs(v, _, _)
  // Return if, v: for all if in ifs and v is one of all terminal variables in
  // the pattern tree of this if
  // 3. pattern ifs("x", _, _)
  // Return empty one column table if "x" not in variable database
  // Return if: for all if in ifs where "x" also a terminal variable in the
  // pattern tree of this if (same for while)
  bool isSingleColumn = true;
  unordered_set<string> synonymColumn;
  StringMap columnValues;
  shared_ptr<QueryResultsTable> table;

  // Get PKB data
  unordered_set<string> PKBIfData = dataAccessLayer->getEntity(IF);
  unordered_set<string> PKBWhileData = dataAccessLayer->getEntity(WHILE);
  unordered_set<string> PKBVarData = dataAccessLayer->getAllVariables();

  // Determine if the synonym is if or while
  if (PKBIfData.count(synonym)) {
    cout << "synonym is if" << endl;
    // Get all if statement numbers
    synonymColumn = PKBIfData;
  } else if (PKBWhileData.count(synonym)) {
    cout << "synonym is while" << endl;
    // Get all while statement numbers
    synonymColumn = PKBWhileData;
  } else {
    synonymColumn = {};
    table = QueryResultsTable::createTable(synonym, synonymColumn);
    // printVectorString(synonymColumn);
    return table;
  }

  // Only refer to arg1
  if (arg1->isWildcard()) {
  }                           // no further filtering
  else if (arg1->isExpr()) {  // variable name in quotes
    string identifier = svToString(arg1->getIdentifier());

    // Return empty one column table if "x" not in variable list (early
    // termination)
    if (PKBVarData.count(identifier) == 0) {
      synonymColumn = {};
      table = QueryResultsTable::createTable(synonym, synonymColumn);
      // printVectorString(synonymColumn);
      return table;
    }
    unordered_set<string> intersecting = {};
    for (const string& lineNum : synonymColumn) {
      shared_ptr<Node> lineTree = dataAccessLayer->getPatternTree(lineNum);
      // get all terminal variables in the pattern tree
      // check if variable is a terminal variable
      unordered_set<string> terminals = lineTree->getTerminalVariables();
      if (terminals.count(identifier)) {
        intersecting.insert(lineNum);
      }
    }
    synonymColumn = intersecting;
  } else if (arg1->isSynonym()) {  // variable synonym
    isSingleColumn = false;
    // for all if/while statements
    for (const string& lineNum : synonymColumn) {
      shared_ptr<Node> lineTree = dataAccessLayer->getPatternTree(lineNum);
      // get all terminal variables in the pattern tree
      unordered_set<string> terminals = lineTree->getTerminalVariables();
      // add (k=if/while statement number, v=variable syn) to columnValues
      columnValues[lineNum] = terminals;
    }
  }

  if (isSingleColumn) {
    table = QueryResultsTable::createTable(synonym, synonymColumn);
    // printVectorString(synonymColumn);
  } else {
    vector<string> headers = {
        synonym,
        svToString(arg1->getArg())};  // 1st arg is if/while line number,
                                      // 2nd arg is variable syn
    table = QueryResultsTable::createTable(headers, columnValues);
    // printMap(columnValues);
  }

  return table;
}

shared_ptr<QueryResultsTable> IfPatternObject::callAndProcess(
    shared_ptr<DataAccessLayer> dataAccessLayer) {
  string synonym = svToString(getPatternSynonym()->getArg());
  shared_ptr<ClauseArg> arg1 = getArg1();
  return callAndProcessIfWhile(dataAccessLayer, synonym, arg1);
}

shared_ptr<QueryResultsTable> WhilePatternObject::callAndProcess(
    shared_ptr<DataAccessLayer> dataAccessLayer) {
  string synonym = svToString(getPatternSynonym()->getArg());
  shared_ptr<ClauseArg> arg1 = getArg1();
  return callAndProcessIfWhile(dataAccessLayer, synonym, arg1);
}
