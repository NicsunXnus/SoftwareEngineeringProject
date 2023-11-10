#include "EntityStorage.h"

shared_ptr<unordered_map<ENTITY, unordered_set<string>>> StatementStorage::getDatabase() {
  return this->database;
}

void StatementStorage::setEntity(shared_ptr<StringMap> database_to_insert) {
  static unordered_map<ENTITY, unordered_set<string>> statement_db;
  this->database =
      make_shared<unordered_map<ENTITY, unordered_set<string>>>(statement_db);
  for (auto const& [entity_string, value] : *database_to_insert) {
    (*database)[EntityStringToEnum(entity_string)] = value;
  }
}

shared_ptr<StringMap> NonStatementStorage::getDatabase() {
  return this->database;
}

void NonStatementStorage::setEntity(shared_ptr<StringMap> database_to_insert) {
  static StringMap db;
  this->database = make_shared<StringMap>(db);
  for (auto const& [procName, lines] : *database_to_insert) {
    (*(this->database))[procName] = lines;
  }
}

shared_ptr<unordered_map<string, pair<string, string>>> ProcLinesStorage::getDatabase() {
  return this->database;
}

void ProcLinesStorage::setProcLines(
    shared_ptr<unordered_map<string, pair<string, string>>> database_to_insert) {
  static unordered_map<string, pair<string, string>> proclines_db;
  this->database =
      make_shared<unordered_map<string, pair<string, string>>>(proclines_db);
  for (auto const& [procName, lines] : *database_to_insert) {
    (*(this->database))[procName] = lines;
  }
}

shared_ptr<unordered_map<string, shared_ptr<Node>>> PatternStorage::getDatabase() {
  return this->database;
}

void PatternStorage::setPattern(
    shared_ptr<unordered_map<string, shared_ptr<Node>>> database_to_insert) {
  static unordered_map<string, shared_ptr<Node>> pattern_db;
  this->database =
      make_shared<unordered_map<string, shared_ptr<Node>>>(pattern_db);
  for (auto const& [varName, node] : *database_to_insert) {
    (*(this->database))[varName] = node;
  }
}
