#include "StorageManager.h"

StorageManager::StorageManager() {
  this->statement_entities = make_shared<StatementStorage>();
  this->procedure_entities = make_shared<ProcedureStorage>();
  this->variable_entities = make_shared<VariableStorage>();
  this->constant_entities = make_shared<ConstantStorage>();
  this->call_procname_entities = make_shared<CallProcnameStorage>();
  this->read_varname_entities = make_shared<ReadVarnameStorage>();
  this->print_varname_entities = make_shared<PrintVarnameStorage>();
  this->proc_lines_entities = make_shared<ProcLinesStorage>();
  this->pattern_entities = make_shared<PatternStorage>();

  this->uses_abstractions = make_shared<UsesAbstractionStorage>();
  this->modifies_abstractions = make_shared<ModifiesAbstractionStorage>();
  this->follows_abstractions = make_shared<FollowsAbstractionStorage>();
  this->follows_star_abstractions =
      make_shared<FollowsStarAbstractionStorage>();
  this->parent_abstractions = make_shared<ParentAbstractionStorage>();
  this->parent_star_abstractions = make_shared<ParentStarAbstractionStorage>();
  this->calls_abstractions = make_shared<CallsAbstractionStorage>();
  this->calls_star_abstractions = make_shared<CallsStarAbstractionStorage>();
  this->next_abstractions = make_shared<NextAbstractionStorage>();

  // store all non-statement entities in a map for easy retrieval
  this->non_statement_entity_storage_map = {
      {PROCEDURE, procedure_entities},
      {VARIABLE, variable_entities},
      {CONSTANT, constant_entities}};

  // store all entity name storages in a map for easy retrieval
  this->entity_name_storage_map = {{READ, read_varname_entities},
                                   {PRINT, print_varname_entities},
                                   {CALL, call_procname_entities}};

  // store all abstraction storages in a map for easy retrieval
  this->abstraction_storage_map = {
      {USES, uses_abstractions},
      {MODIFIES, modifies_abstractions},
      {FOLLOWS, follows_abstractions},
      {FOLLOWSSTAR, follows_star_abstractions},
      {PARENT, parent_abstractions},
      {PARENTSTAR, parent_star_abstractions},
      {CALLS, calls_abstractions},
      {CALLSSTAR, calls_star_abstractions},
      {NEXT, next_abstractions}};
}

shared_ptr<AbstractionStorage> StorageManager::getAbstractionStorage(
    const ABSTRACTION abstraction_type) {
  return abstraction_storage_map.at(abstraction_type);
}

shared_ptr<NameStorage> StorageManager::getEntityNameStorage(
    const ENTITY entity_type) {
  if (entity_name_storage_map.find(entity_type) ==
      entity_name_storage_map.end()) {
    throw runtime_error("Entity type not found in entity name storage map");
  }
  return entity_name_storage_map.at(entity_type);
}

shared_ptr<NonStatementStorage> StorageManager::getEntityNonStmtStorage(
    const ENTITY entity_type) {
  if (non_statement_entity_storage_map.find(entity_type) ==
      non_statement_entity_storage_map.end()) {
    throw runtime_error(
        "Entity type not found in non-statement entity storage map");
  }
  return non_statement_entity_storage_map.at(entity_type);
}

shared_ptr<StatementStorage> StorageManager::getStatementStorage() {
  return statement_entities;
}

shared_ptr<ProcLinesStorage> StorageManager::getProcLinesStorage() {
  return proc_lines_entities;
}

shared_ptr<PatternStorage> StorageManager::getPatternStorage() {
  return pattern_entities;
}