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
     ENTITY entity_type = STMT);

  /*
   * This function adds entity names into the corresponding database, based on
   * the entity type.
   *
   * @param entity_name_map the StringMap to be inserted
   * @param entity_type the type of entity to be inserted. Only CALL, READ and
   * PRINT are supported
   */
  void Insertor::addEntityNames(shared_ptr<StringMap> entity_name_map,
    ENTITY entity_type);

  /*
   * This function adds start and end lines information for procedures.
   *
   * @param proclines_map the map of procedure name to pair of start and end
   * line numbers
   */
  void Insertor::addProcLines(
    shared_ptr<map<string, pair<string, string>>> proclines_map);

  /*
   * This function adds pattern subtrees into the corresponding
   * database.
   *
   * @param pattern_map the map of parent statement number to root of subtree of
   * that statement
   */
  void Insertor::addPatterns(
    shared_ptr<map<string, shared_ptr<Node>>> pattern_map);

  /*
   * This function adds abstractions into the corresponding database, based on
   * the abstraction type. keys are the first argument for abstractions.
   * include variable names or line numbers.
   *
   * @param abstraction_map the StringMap to be inserted
   * @param abstraction_type the type of abstraction to be inserted
   */
  void Insertor::addAbstraction(shared_ptr<StringMap> abstraction_map,
    ABSTRACTION abstraction_type);
};