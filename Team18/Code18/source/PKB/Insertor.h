#pragma once
#include "StorageManager.h"

/**
 * This class represents Insertor for the PKB, with APIs called by the SP.
 */
class Insertor {
 public:
  /*
   * This function adds entities into the corresponding database, based on the
   * entity type.
   *
   * @param entity_map the StringMap to be inserted
   * @param entity_type the type of entity to be inserted
   */
  void Insertor::addEntity(shared_ptr<StringMap> entity_map,
                           ENTITY entity_type = STMT) {
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

  /*
   * This function adds entity names into the corresponding database, based on
   * the entity type.
   *
   * @param entity_name_map the StringMap to be inserted
   * @param entity_type the type of entity to be inserted. Only CALL, READ and
   * PRINT are supported
   */
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

  /*
   * This function adds start and end lines information for procedures.
   *
   * @param proclines_map the map of procedure name to pair of start and end
   * line numbers
   */
  void Insertor::addProcLines(
      shared_ptr<map<string, pair<string, string>>> proclines_map) {
    shared_ptr<EntityStorage> entity_storage =
        StorageManager::getEntityStorage();
    (*entity_storage).setProcLinesDatabase(proclines_map);
  }

  /*
   * This function adds pattern subtrees into the corresponding
   * database.
   *
   * @param pattern_map the map of parent statement number to root of subtree of
   * that statement
   */
  void Insertor::addPatterns(
      shared_ptr<map<string, shared_ptr<Node>>> pattern_map) {
    shared_ptr<EntityStorage> entity_storage =
        StorageManager::getEntityStorage();
    (*entity_storage).setPatternDatabase(pattern_map);
  }

  /*
   * This function adds abstractions into the corresponding database, based on
   * the abstraction type. keys are the first argument for abstractions.
   * include variable names or line numbers.
   *
   * @param abstraction_map the StringMap to be inserted
   * @param abstraction_type the type of abstraction to be inserted
   */
  void Insertor::addAbstraction(shared_ptr<StringMap> abstraction_map,
                                ABSTRACTION abstraction_type) {
    shared_ptr<AbstractionStorage> abstraction_storage =
        StorageManager::getAbstractionStorage(abstraction_type);
    (*abstraction_storage).setAbstraction(abstraction_map);
  }
};