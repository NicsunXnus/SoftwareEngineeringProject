#pragma once
#include <string>
#include <unordered_set>

#include "../Constants/DesignEnums.h"
#include "StorageManager.h"

using namespace std;

/**
 * This class represents Responder for the PKB, with APIs called by the QPS.
 * methods to respond to queries. generally, call the storage manager to do
 * work. storage manager will get entity storage/abstraction storage, perform
 * the query on that class via polymorphism, then return line numbers.
 */
class Responder {
 public:
  /*
   * This function gets entity statements, based on the entity
   * type.
   *
   * @param entity the type of entity to be retrieved
   * @return unordered_set<string> the set of statement numbers that are of the
   * entity type
   */
  unordered_set<string> Responder::getEntityStatement(ENTITY entity) const {
    shared_ptr<EntityStorage> entity_storage =
        StorageManager::getEntityStorage();
    shared_ptr<map<ENTITY, unordered_set<string>>> entity_database =
        entity_storage->getStatementDatabase();
    // check if entity exists
    if (entity_database->find(entity) == entity_database->end()) {
      return unordered_set<string>();
    }
    return (*entity_database).at(entity);
  }

  /*
   * This function gets all procedure names.
   *
   * @return unordered_set<string> the set of all procedure names
   */
  unordered_set<string> Responder::getAllProcedures() const {
    shared_ptr<EntityStorage> entity_storage =
        StorageManager::getEntityStorage();
    shared_ptr<StringMap> proc_database =
        entity_storage->getProcedureDatabase();
    return getKeys(proc_database);
  }

  /*
   * This function gets the map of all variable numbers to statement numbers.
   *
   * @return StringMap the map of all variable numbers to statement numbers
   */
  StringMap Responder::getVariableMap() const {
    shared_ptr<EntityStorage> entity_storage =
        StorageManager::getEntityStorage();
    shared_ptr<StringMap> var_database = entity_storage->getVariableDatabase();
    return *var_database;
  }

  /*
   * This function gets the set of all variable names.
   *
   * @return unordered_set<string> the set of all variable names
   */
  unordered_set<string> Responder::getAllVariables() const {
    shared_ptr<EntityStorage> entity_storage =
        StorageManager::getEntityStorage();
    shared_ptr<StringMap> var_database = entity_storage->getVariableDatabase();
    return getKeys(var_database);
  }

  /*
   * This function gets the map of all constant numbers to statement numbers.
   *
   * @return StringMap the map of all constant numbers to statement numbers
   */
  StringMap Responder::getConstantMap() const {
    shared_ptr<EntityStorage> entity_storage =
        StorageManager::getEntityStorage();
    shared_ptr<StringMap> const_database =
        entity_storage->getConstantDatabase();
    return *const_database;
  }

  /*
   * This function gets the set of all constant names.
   *
   * @return unordered_set<string> the set of all constant names
   */
  unordered_set<string> Responder::getAllConstants() const {
    shared_ptr<EntityStorage> entity_storage =
        StorageManager::getEntityStorage();
    shared_ptr<StringMap> const_database =
        entity_storage->getConstantDatabase();
    return getKeys(const_database);
  }

  /*
   * This function gets the start/end line numbers.
   *
   * @param procedure the name of the procedure
   * @return pair<string, string> the pair of start/end line numbers of the
   * procedure
   */
  pair<string, string> Responder::getProcLines(string procedure) const {
    shared_ptr<EntityStorage> entity_storage =
        StorageManager::getEntityStorage();
    shared_ptr<map<string, pair<string, string>>> proclines_database =
        entity_storage->getProcLinesDatabase();
    // check if procedure exists
    if (proclines_database->find(procedure) == proclines_database->end()) {
      return pair<string, string>();
    }
    return (*proclines_database).at(procedure);
  }

  /*
   * This function gets the map of all procedure calls to names of procedures
   * called.
   *
   * @return StringMap the map of all procedure calls to names of procedures
   * called
   */
  StringMap Responder::getCallProcNameMap() const {
    shared_ptr<EntityStorage> entity_storage =
        StorageManager::getEntityStorage();
    shared_ptr<StringMap> call_procname_database =
        entity_storage->getCallProcnameDatabase();
    return *call_procname_database;
  }

  /*
   * This function gets the map of all reads to names of variables
   * being read.
   *
   * @return StringMap the map of all reads to names of variables
   */
  StringMap Responder::getReadVarNameMap() const {
    shared_ptr<EntityStorage> entity_storage =
        StorageManager::getEntityStorage();
    shared_ptr<StringMap> read_varname_database =
        entity_storage->getReadVarnameDatabase();
    return *read_varname_database;
  }

  /*
   * This function gets the map of all prints to names of variables
   * being printed.
   *
   * @return StringMap the map of all prints to names of variables
   */
  StringMap Responder::getPrintVarNameMap() const {
    shared_ptr<EntityStorage> entity_storage =
        StorageManager::getEntityStorage();
    shared_ptr<StringMap> print_varname_database =
        entity_storage->getPrintVarnameDatabase();
    return *print_varname_database;
  }

  /*
   * This function gets the map of all if/while/assign statement numbers to root
   * of pattern subtrees
   *
   * @return map<string, shared_ptr<Node>> the map of all if/while/assign
   * statement numbers to root of pattern subtrees
   */
  map<string, shared_ptr<Node>> Responder::getAllPatterns() const {
    shared_ptr<EntityStorage> entity_storage =
        StorageManager::getEntityStorage();
    shared_ptr<map<string, shared_ptr<Node>>> pattern_database =
        entity_storage->getPatternDatabase();
    return *pattern_database;
  }

  /*
   * This function gets the shared pointer to root of pattern subtree
   *
   * @param statement_number the statement number of the assign/if/while
   * statement
   * @return shared_ptr<Node> the root of the pattern subtree at that statement
   */
  shared_ptr<Node> Responder::getPattern(string statement_number) const {
    shared_ptr<EntityStorage> entity_storage =
        StorageManager::getEntityStorage();
    shared_ptr<map<string, shared_ptr<Node>>> pattern_database =
        entity_storage->getPatternDatabase();
    // check if pattern exists
    if (pattern_database->find(statement_number) == pattern_database->end()) {
      return nullptr;
    }
    return (*pattern_database).at(statement_number);
  }

  /*
   * This function gets the map for a design abstraction.
   *
   * @param abstraction the design abstraction to be retrieved
   * @param inverse whether to return the inverse map
   * @return StringMap the map of first arguments to all possible second
   * arguments for this abstraction, vice versa if inversed.
   */
  StringMap Responder::getAbstraction(ABSTRACTION abstraction,
                                      bool inverse = false) const {
    shared_ptr<AbstractionStorage> abstraction_storage =
        StorageManager::getAbstractionStorage(abstraction);
    if (inverse ^ isFlippedStorage(abstraction)) {
      return createInverseMap(abstraction_storage->getDatabase());
    }
    return *(abstraction_storage->getDatabase());
  }

  /*
   * This function gets the set of all second arguments for a design
   * abstraction, given a first argument.
   *
   * @param abstraction the design abstraction to be retrieved
   * @return unordered_set<string> the set of all second arguments for this
   * abstraction
   */
  unordered_set<string> Responder::getAbstractionVariable(
      ABSTRACTION abstraction, string key) const {
    shared_ptr<AbstractionStorage> abstraction_storage =
        StorageManager::getAbstractionStorage(abstraction);
    shared_ptr<StringMap> abstraction_database =
        abstraction_storage->getDatabase();
    if (abstraction_database->find(key) == abstraction_database->end()) {
      return unordered_set<string>();
    }
    return (*abstraction_database).at(key);
  }

 private:
  unordered_set<string> getKeys(shared_ptr<StringMap> db) const {
    unordered_set<string> keys;
    for (const auto& [k, v] : *db) {
      keys.insert(k);
    }
    return keys;
  }

  /*
   * This function creates an inverse map, reversing keys and values.
   *
   * @param originalMap the original map
   * @return StringMap the inverse map
   */
  StringMap createInverseMap(shared_ptr<StringMap> originalMap) const {
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
};