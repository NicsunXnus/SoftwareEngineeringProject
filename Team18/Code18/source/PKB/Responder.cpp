#include "Responder.h"

unordered_set<string> Responder::getEntityStatement(ENTITY entity) {
  shared_ptr<StatementStorage> entity_storage =
      StorageManager::getStatementStorage();
  shared_ptr<unordered_map<ENTITY, unordered_set<string>>> entity_database =
      entity_storage->getDatabase();
  // check if entity exists
  if (entity_database->find(entity) == entity_database->end()) {
    return unordered_set<string>();
  }
  return (*entity_database).at(entity);
}

unordered_set<string> Responder::getNonStatementEntityList(ENTITY entity) {
  shared_ptr<NonStatementStorage> entity_storage =
      StorageManager::getEntityNonStmtStorage(entity);
  shared_ptr<StringMap> entity_database = entity_storage->getDatabase();
  return getKeys(entity_database);
}

StringMap Responder::getNonStatementEntityMap(ENTITY entity) {
  shared_ptr<NonStatementStorage> entity_storage =
      StorageManager::getEntityNonStmtStorage(entity);
  return *(entity_storage->getDatabase());
}

pair<string, string> Responder::getProcLines(string procedure) {
  shared_ptr<ProcLinesStorage> entity_storage =
      StorageManager::getProcLinesStorage();
  shared_ptr<unordered_map<string, pair<string, string>>> proclines_database =
      entity_storage->getDatabase();
  // check if procedure exists
  if (proclines_database->find(procedure) == proclines_database->end()) {
    return pair<string, string>();
  }
  return (*proclines_database).at(procedure);
}

StringMap Responder::getNameMap(ENTITY entity_type) { 
  shared_ptr<NameStorage> name_storage =
	  StorageManager::getEntityNameStorage(entity_type);
  return *(name_storage->getDatabase());
}

unordered_map<string, shared_ptr<Node>> Responder::getAllPatterns() {
  shared_ptr<PatternStorage> entity_storage =
      StorageManager::getPatternStorage();
  return *(entity_storage->getDatabase());
}

shared_ptr<Node> Responder::getPattern(string statement_number) {
  shared_ptr<PatternStorage> entity_storage =
      StorageManager::getPatternStorage();
  shared_ptr<unordered_map<string, shared_ptr<Node>>> pattern_database =
      entity_storage->getDatabase();
  // check if pattern exists
  if (pattern_database->find(statement_number) == pattern_database->end()) {
    return nullptr;
  }
  return (*pattern_database).at(statement_number);
}

StringMap Responder::getAbstraction(ABSTRACTION abstraction,
                                    bool inverse) {
  shared_ptr<AbstractionStorage> abstraction_storage =
      StorageManager::getAbstractionStorage(abstraction);
  if (inverse ^ isFlippedStorage(abstraction)) {
    return createInverseMap(abstraction_storage->getDatabase());
  }
  return *(abstraction_storage->getDatabase());
}

unordered_set<string> Responder::getAbstractionVariable(
    ABSTRACTION abstraction, string key) {
  shared_ptr<AbstractionStorage> abstraction_storage =
      StorageManager::getAbstractionStorage(abstraction);
  shared_ptr<StringMap> abstraction_database =
      abstraction_storage->getDatabase();
  if (abstraction_database->find(key) == abstraction_database->end()) {
    return unordered_set<string>();
  }
  return (*abstraction_database).at(key);
}

unordered_set<string> Responder::getKeys(shared_ptr<StringMap> db) {
  unordered_set<string> keys;
  for (const auto& [k, v] : *db) {
    keys.insert(k);
  }
  return keys;
}

StringMap Responder::createInverseMap(shared_ptr<StringMap> originalMap) {
  StringMap inverseMap;
  for (const auto& pair : *originalMap) {
    const string& key = pair.first;
    const unordered_set<string>& values = pair.second;
    for (const string& value : values) {
      inverseMap[value].insert(key);
    }
  }
  return inverseMap;
}