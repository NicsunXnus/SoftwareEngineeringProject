#include "Insertor.h"

void Insertor::addEntity(shared_ptr<StringMap> entity_map,
                         ENTITY entity_type) {
  if (entity_type == STMT) {
    shared_ptr<StatementStorage> entity_storage =
        StorageManager::getStatementStorage();
    (*entity_storage).setEntity(entity_map);
  } else {
    shared_ptr<NonStatementStorage> entity_storage =
        StorageManager::getEntityNonStmtStorage(entity_type);
    (*entity_storage).setEntity(entity_map);
  }
}

void Insertor::addEntityNames(shared_ptr<StringMap> entity_name_map,
                              ENTITY entity_type) {
  shared_ptr<NameStorage> entity_storage =
      StorageManager::getEntityNameStorage(entity_type);
  (*entity_storage).setEntity(entity_name_map);
}

void Insertor::addProcLines(
    shared_ptr<map<string, pair<string, string>>> proclines_map) {
  shared_ptr<ProcLinesStorage> entity_storage = StorageManager::getProcLinesStorage();
  (*entity_storage).setProcLines(proclines_map);
}

void Insertor::addPatterns(
    shared_ptr<map<string, shared_ptr<Node>>> pattern_map) {
  shared_ptr<PatternStorage> entity_storage = StorageManager::getPatternStorage();
  (*entity_storage).setPattern(pattern_map);
}

void Insertor::addAbstraction(shared_ptr<StringMap> abstraction_map,
                              ABSTRACTION abstraction_type) {
  shared_ptr<AbstractionStorage> abstraction_storage =
      StorageManager::getAbstractionStorage(abstraction_type);
  (*abstraction_storage).setAbstraction(abstraction_map);
}