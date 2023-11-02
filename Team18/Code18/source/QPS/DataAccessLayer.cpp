#pragma once
#include "DataAccessLayer.h"

#include <list>

unordered_set<string> DataAccessLayer::getEntity(ENTITY type) {
  if (type == CONSTANT) {
    return getAllConstants();
  } else if (type == VARIABLE) {
    return getAllVariables();
  } else if (type == PROCEDURE) {
    return getAllProcedures();
  }
  unordered_set<string> PKBResponse = PKB::responder.getEntityStatement(type);
  return PKBResponse;
}

unordered_set<string> DataAccessLayer::getAllProcedures() {
  unordered_set<string> PKBResponse =
      PKB::responder.getNonStatementEntityList(PROCEDURE);
  return PKBResponse;
}

unordered_set<string> DataAccessLayer::getAllVariables() {
  unordered_set<string> PKBResponse =
      PKB::responder.getNonStatementEntityList(VARIABLE);
  return PKBResponse;
}

unordered_set<string> DataAccessLayer::getAllConstants() {
  unordered_set<string> PKBResponse =
      PKB::responder.getNonStatementEntityList(CONSTANT);
  return PKBResponse;
}

map<string, unordered_set<string>> DataAccessLayer::getVariableMap() {
  StringMap PKBResponse = PKB::responder.getNonStatementEntityMap(VARIABLE);
  return PKBResponse;
}

map<string, unordered_set<string>> DataAccessLayer::getConstantMap() {
  StringMap PKBResponse = PKB::responder.getNonStatementEntityMap(CONSTANT);
  return PKBResponse;
}

shared_ptr<Node> DataAccessLayer::getPatternTree(string statement_number) {
  shared_ptr<Node> PKBResponse = PKB::responder.getPattern(statement_number);
  return PKBResponse;
}

StringMap DataAccessLayer::getClause(ABSTRACTION abstraction) {
  StringMap PKBResponse = PKB::responder.getAbstraction(abstraction);
  return PKBResponse;
}

StringMap DataAccessLayer::getClauseInverse(ABSTRACTION abstraction) {
  StringMap PKBResponse = PKB::responder.getAbstraction(abstraction, true);
  return PKBResponse;
}

StringMap DataAccessLayer::getCallProcNames() {
  StringMap PKBResponse = PKB::responder.getNameMap(CALL);
  return PKBResponse;
}

StringMap DataAccessLayer::getReadVarNames() {
  StringMap PKBResponse = PKB::responder.getNameMap(READ);
  return PKBResponse;
}

StringMap DataAccessLayer::getPrintVarNames() {
  StringMap PKBResponse = PKB::responder.getNameMap(PRINT);
  return PKBResponse;
}
