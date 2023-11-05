#pragma once
#ifndef STORAGEMANAGER_H
#define STORAGEMANAGER_H

#include "StorageClasses/AbstractionStorage.h"
#include "StorageClasses/EntityStorage.h"

using namespace std;

/**
 * This class represents Storage for the PKB. Consists of entity/abstraction
 * storages.
 */
class StorageManager {
 public:
  StorageManager();

  ~StorageManager() {}

  /*
   * This function gets the abstraction storage, based on the
   * abstraction type.
   *
   * @param abstraction_type the type of abstraction to be retrieved
   * @return shared pointer to the abstraction storage
   */
  static shared_ptr<AbstractionStorage> StorageManager::getAbstractionStorage(
      const ABSTRACTION abstraction_type);

  /*
   * This function gets the entity name storage, based on the
   * entity type.
   *
   * @param entity_type the type of entity name to be retrieved
   * @return shared pointer to the entity name storage
   */
  static shared_ptr<NameStorage> getEntityNameStorage(const ENTITY entity_type);

  /*
   * This function gets the non statement entity storage, based on the
   * entity type.
   *
   * @param entity_type the type of non statement entity to be retrieved
   * @return shared pointer to the non statement entity storage
   */
  static shared_ptr<NonStatementStorage> getEntityNonStmtStorage(
      const ENTITY entity_type);

  /*
   * This function gets the statement entity storage.
   *
   * @return shared pointer to the statement entity storage
   */
  static shared_ptr<StatementStorage> getStatementStorage();

  /*
   * This function gets the procedure lines entity storage.
   *
   * @return shared pointer to the procedure lines entity storage
   */
  static shared_ptr<ProcLinesStorage> getProcLinesStorage();

  /*
   * This function gets the pattern entity storage.
   *
   * @return shared pointer to the pattern entity storage
   */
  static shared_ptr<PatternStorage> getPatternStorage();

 private:
  // entities
  static inline shared_ptr<StatementStorage> statement_entities;
  static inline shared_ptr<ProcedureStorage> procedure_entities;
  static inline shared_ptr<VariableStorage> variable_entities;
  static inline shared_ptr<ConstantStorage> constant_entities;
  static inline shared_ptr<CallProcnameStorage> call_procname_entities;
  static inline shared_ptr<ReadVarnameStorage> read_varname_entities;
  static inline shared_ptr<PrintVarnameStorage> print_varname_entities;
  static inline shared_ptr<ProcLinesStorage> proc_lines_entities;
  static inline shared_ptr<PatternStorage> pattern_entities;

  // abstractions
  static inline shared_ptr<UsesAbstractionStorage> uses_abstractions;
  static inline shared_ptr<ModifiesAbstractionStorage> modifies_abstractions;
  static inline shared_ptr<FollowsAbstractionStorage> follows_abstractions;
  static inline shared_ptr<FollowsStarAbstractionStorage>
      follows_star_abstractions;
  static inline shared_ptr<ParentAbstractionStorage> parent_abstractions;
  static inline shared_ptr<ParentStarAbstractionStorage>
      parent_star_abstractions;
  static inline shared_ptr<CallsAbstractionStorage> calls_abstractions;
  static inline shared_ptr<CallsStarAbstractionStorage> calls_star_abstractions;
  static inline shared_ptr<NextAbstractionStorage> next_abstractions;

  // map of abstraction storages for retrieval based on enums
  static inline map<ABSTRACTION, shared_ptr<AbstractionStorage>>
      abstraction_storage_map;

  // map of non-statement entities for retrieval based on enums
  static inline map<ENTITY, shared_ptr<NonStatementStorage>>
      non_statement_entity_storage_map;

  // map of entity name storages for retrieval based on enums
  static inline map<ENTITY, shared_ptr<NameStorage>> entity_name_storage_map;
};
#endif