#include "Insertor.h"

void Insertor::addEntity(shared_ptr<StringMap> entity_map,
                          ENTITY entity_type) {
  shared_ptr<EntityStorage> entity_storage =
      StorageManager::getEntityStorage();

  switch (entity_type) {
    case VARIABLE:
      (*entity_storage).setVariableDatabase(entity_map);
      break;
    case CONSTANT:
      (*entity_storage).setConstantDatabase(entity_map);
      break;
    case PROCEDURE:
      (*entity_storage).setProcedureDatabase(entity_map);
      break;
    default:  // other entities are statement types
      (*entity_storage).setStatementDatabase(entity_map);
      break;
  }
}

void Insertor::addEntityNames(shared_ptr<StringMap> entity_name_map,
                              ENTITY entity_type) {
  shared_ptr<EntityStorage> entity_storage =
      StorageManager::getEntityStorage();

  switch (entity_type) {
    case CALL:
      (*entity_storage).setCallProcnameDatabase(entity_name_map);
      break;
    case READ:
      (*entity_storage).setReadVarnameDatabase(entity_name_map);
      break;
    case PRINT:
      (*entity_storage).setPrintVarnameDatabase(entity_name_map);
      break;
    default:  // other entities are statement types
      throw runtime_error("This entity does not support name storage");
      break;
  }
}

void Insertor::addProcLines(
    shared_ptr<map<string, pair<string, string>>> proclines_map) {
  shared_ptr<EntityStorage> entity_storage =
      StorageManager::getEntityStorage();
  (*entity_storage).setProcLinesDatabase(proclines_map);
}

void Insertor::addPatterns(
    shared_ptr<map<string, shared_ptr<Node>>> pattern_map) {
  shared_ptr<EntityStorage> entity_storage =
      StorageManager::getEntityStorage();
  (*entity_storage).setPatternDatabase(pattern_map);
}

void Insertor::addAbstraction(shared_ptr<StringMap> abstraction_map,
                              ABSTRACTION abstraction_type) {
  shared_ptr<AbstractionStorage> abstraction_storage =
      StorageManager::getAbstractionStorage(abstraction_type);
  (*abstraction_storage).setAbstraction(abstraction_map);
}