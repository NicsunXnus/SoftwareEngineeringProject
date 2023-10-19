#pragma once
#include "DataAccessLayerStub.h"

#include <list>

#include "../SP/SimpleProcessor/ExpressionProcessor.h"

unordered_set<string> entities = {"1", "2", "3", "4"};
unordered_set<string> procedures = {"a", "b", "c", "d", "e"};
unordered_set<string> variables = {"a", "b", "c"};
unordered_set<string> constants = {"100", "300"};
map<string, unordered_set<string>> constantMap = {{"100", {"1"}},
                                                  {"300", {"3", "4"}}};
map<string, unordered_set<string>> variableMap = {
    {"a", {"1"}}, {"b", {"2", "3"}}, {"c", {"3"}}};

map<string, unordered_set<string>> clauses = {
    {"1", {"2"}}, {"2", {"3"}}, {"3", {"4"}}};
map<string, unordered_set<string>> clausesMult = {
    {"1", {"2", "3"}}, {"2", {"3", "4"}}, {"3", {"4"}}};
map<string, unordered_set<string>> clausesEnts = {
    {"1", {"a"}}, {"2", {"b"}}, {"3", {"c", "b"}}};
map<string, unordered_set<string>> clausesProc = {
    {"a", {"b"}}, {"b", {"c"}}, {"c", {"d", "e"}}};
map<string, unordered_set<string>> nextMap = {
    {"1", {"2"}},      {"2", {"3", "5"}}, {"3", {"4"}}, {"4", {"2"}},
    {"5", {"6", "7"}}, {"6", {"8"}},      {"7", {"8"}}};

map<string, unordered_set<string>> clausesInverse = {
    {"2", {"1"}}, {"3", {"2"}}, {"4", {"3"}}};
map<string, unordered_set<string>> clausesMultInverse = {
    {"2", {"1"}}, {"3", {"2", "1"}}, {"4", {"3", "2"}}};
map<string, unordered_set<string>> clausesEntsInverse = {
    {"a", {"1"}}, {"b", {"2", "3"}}, {"c", {"3"}}};
map<string, unordered_set<string>> clausesProcInverse = {
    {"b", {"a"}}, {"c", {"b"}}, {"d", {"c"}}, {"e", {"c"}}};
map<string, unordered_set<string>> nextMapInverse = {
    {"2", {"1", "4"}}, {"3", {"2"}}, {"5", {"2"}},     {"4", {"3"}},
    {"6", {"5"}},      {"7", {"5"}}, {"8", {"6", "7"}}};

unordered_set<string> DataAccessLayerStub::getEntity(ENTITY type) {
  if (type == VARIABLE) {
    return variables;
  }
  if (type == PROCEDURE) {
    return procedures;
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

map<string, unordered_set<string>> DataAccessLayerStub::getVariableMap() {
  return variableMap;
}

map<string, unordered_set<string>> DataAccessLayerStub::getConstantMap() {
  return constantMap;
}

shared_ptr<Node> DataAccessLayerStub::getPatternTree(string statement_number) {
  ExpressionProcessor ep = ExpressionProcessor();

  map<string, shared_ptr<Node>> patternTree = {
      {"1", ep.nodifyArithmeticExpression("a + 100 - b")},
      {"2", ep.nodifyArithmeticExpression("d / c")},
      {"3", ep.nodifyArithmeticExpression("300")},
      {"4", ep.nodifyArithmeticExpression("b * (c * 300)")},
  };
  return patternTree.at(statement_number);
}

map<string, unordered_set<string>> DataAccessLayerStub::getClause(
    ABSTRACTION abstraction) {
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

map<string, unordered_set<string>> DataAccessLayerStub::getClauseInverse(
    ABSTRACTION abstraction) {
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
