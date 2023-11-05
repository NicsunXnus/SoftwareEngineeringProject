#pragma once
#include "DataAccessLayerStub.h"

#include <list>

#include "../SP/SimpleProcessor/ExpressionProcessor.h"

unordered_set<string> entities = {"1", "2", "3", "4"};
unordered_set<string> procedures = {"a", "b", "c", "d", "e"};
unordered_set<string> variables = {"a", "b", "c"};
unordered_set<string> constants = {"100", "300"};
StringMap constantMap = {{"100", {"1"}}, {"300", {"3", "4"}}};
StringMap variableMap = {{"a", {"1"}}, {"b", {"2", "3"}}, {"c", {"3"}}};
StringMap callProcName = {{"1", {"a"}}, {"2", {"b"}}};
StringMap readVarName = {{"3", {"c"}}, {"4", {"d"}}};
StringMap printVarName = {{"1", {"e"}}};

StringMap clauses = {{"1", {"2"}}, {"2", {"3"}}, {"3", {"4"}}};
StringMap clausesMult = {{"1", {"2", "3"}}, {"2", {"3", "4"}}, {"3", {"4"}}};
StringMap clausesEnts = {{"1", {"a"}}, {"2", {"b"}}, {"3", {"c", "b"}}};
StringMap clausesProc = {{"a", {"b"}}, {"b", {"c"}}, {"c", {"d", "e"}}};
StringMap nextMap = {{"1", {"2"}}, {"2", {"3", "5"}}, {"3", {"4"}},
                     {"4", {"2"}}, {"5", {"6", "7"}}, {"6", {"8"}},
                     {"7", {"8"}}};

StringMap clausesInverse = {{"2", {"1"}}, {"3", {"2"}}, {"4", {"3"}}};
StringMap clausesMultInverse = {
    {"2", {"1"}}, {"3", {"2", "1"}}, {"4", {"3", "2"}}};
StringMap clausesEntsInverse = {{"a", {"1"}}, {"b", {"2", "3"}}, {"c", {"3"}}};
StringMap clausesProcInverse = {
    {"b", {"a"}}, {"c", {"b"}}, {"d", {"c"}}, {"e", {"c"}}};
StringMap nextMapInverse = {{"2", {"1", "4"}}, {"3", {"2"}}, {"5", {"2"}},
                            {"4", {"3"}},      {"6", {"5"}}, {"7", {"5"}},
                            {"8", {"6", "7"}}};

unordered_set<string> DataAccessLayerStub::getEntity(ENTITY type) {
  if (type == VARIABLE) {
    return variables;
  }
  if (type == PROCEDURE) {
    return procedures;
  }
  if (type == CONSTANT) {
    return constants;
  }
  return entities;
}

unordered_set<string> DataAccessLayerStub::getAllProcedures() {
  return procedures;
}

unordered_set<string> DataAccessLayerStub::getAllVariables() {
  return variables;
}

unordered_set<string> DataAccessLayerStub::getAllConstants() {
  return constants;
}

StringMap DataAccessLayerStub::getVariableMap() { return variableMap; }

StringMap DataAccessLayerStub::getConstantMap() { return constantMap; }

shared_ptr<Node> DataAccessLayerStub::getPatternTree(string statement_number) {
  ExpressionProcessor ep = ExpressionProcessor();

  unordered_map<string, shared_ptr<Node>> patternTree = {
      {"1", ep.nodifyArithmeticExpression("a + 100 - b")},
      {"2", ep.nodifyArithmeticExpression("d / c")},
      {"3", ep.nodifyArithmeticExpression("300")},
      {"4", ep.nodifyArithmeticExpression("b * (c * 300)")},
  };
  return patternTree.at(statement_number);
}

StringMap DataAccessLayerStub::getClause(ABSTRACTION abstraction) {
  if (abstraction == USES || abstraction == MODIFIES) {
    return clausesEnts;
  }
  if (abstraction == CALLS || abstraction == CALLSSTAR) {
    return clausesProc;
  }
  if (abstraction == FOLLOWSSTAR || abstraction == PARENTSTAR) {
    return clausesMult;
  }
  if (abstraction == NEXT) {
    return nextMap;
  }
  return clauses;
}

StringMap DataAccessLayerStub::getClauseInverse(ABSTRACTION abstraction) {
  if (abstraction == USES || abstraction == MODIFIES) {
    return clausesEntsInverse;
  }
  if (abstraction == CALLS || abstraction == CALLSSTAR) {
    return clausesProcInverse;
  }
  if (abstraction == FOLLOWSSTAR || abstraction == PARENTSTAR) {
    return clausesMultInverse;
  }
  if (abstraction == NEXT) {
    return nextMapInverse;
  }
  return clausesInverse;
}

StringMap DataAccessLayerStub::getCallProcNames() { return callProcName; }
StringMap DataAccessLayerStub::getReadVarNames() { return readVarName; }
StringMap DataAccessLayerStub::getPrintVarNames() { return printVarName; }
