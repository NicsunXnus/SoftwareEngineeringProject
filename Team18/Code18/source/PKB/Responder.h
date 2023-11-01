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
   unordered_set<string> Responder::getEntityStatement(ENTITY entity);

  /*
   * This function gets all procedure names.
   *
   * @return unordered_set<string> the set of all procedure names
   */
   unordered_set<string> Responder::getAllProcedures();

  /*
   * This function gets the map of all variable numbers to statement numbers.
   *
   * @return StringMap the map of all variable numbers to statement numbers
   */
   StringMap Responder::getVariableMap();

  /*
   * This function gets the set of all variable names.
   *
   * @return unordered_set<string> the set of all variable names
   */
   unordered_set<string> Responder::getAllVariables();

  /*
   * This function gets the map of all constant numbers to statement numbers.
   *
   * @return StringMap the map of all constant numbers to statement numbers
   */
  StringMap Responder::getConstantMap();

  /*
   * This function gets the set of all constant names.
   *
   * @return unordered_set<string> the set of all constant names
   */
  unordered_set<string> Responder::getAllConstants();

  /*
   * This function gets the start/end line numbers.
   *
   * @param procedure the name of the procedure
   * @return pair<string, string> the pair of start/end line numbers of the
   * procedure
   */
  pair<string, string> Responder::getProcLines(string procedure);

  /*
   * This function gets the map of all procedure calls to names of procedures
   * called.
   *
   * @return StringMap the map of all procedure calls to names of procedures
   * called
   */
  StringMap Responder::getCallProcNameMap();

  /*
   * This function gets the map of all reads to names of variables
   * being read.
   *
   * @return StringMap the map of all reads to names of variables
   */
  StringMap Responder::getReadVarNameMap();

  /*
   * This function gets the map of all prints to names of variables
   * being printed.
   *
   * @return StringMap the map of all prints to names of variables
   */
  StringMap Responder::getPrintVarNameMap();

  /*
   * This function gets the map of all if/while/assign statement numbers to root
   * of pattern subtrees
   *
   * @return map<string, shared_ptr<Node>> the map of all if/while/assign
   * statement numbers to root of pattern subtrees
   */
  map<string, shared_ptr<Node>> Responder::getAllPatterns();

  /*
   * This function gets the shared pointer to root of pattern subtree
   *
   * @param statement_number the statement number of the assign/if/while
   * statement
   * @return shared_ptr<Node> the root of the pattern subtree at that statement
   */
  shared_ptr<Node> Responder::getPattern(string statement_number);

  /*
   * This function gets the map for a design abstraction.
   *
   * @param abstraction the design abstraction to be retrieved
   * @param inverse whether to return the inverse map
   * @return StringMap the map of first arguments to all possible second
   * arguments for this abstraction, vice versa if inversed.
   */
  StringMap Responder::getAbstraction(ABSTRACTION abstraction,
    bool inverse = false);

  /*
   * This function gets the set of all second arguments for a design
   * abstraction, given a first argument.
   *
   * @param abstraction the design abstraction to be retrieved
   * @return unordered_set<string> the set of all second arguments for this
   * abstraction
   */
  unordered_set<string> Responder::getAbstractionVariable(
    ABSTRACTION abstraction, string key);

 private:
   unordered_set<string> getKeys(shared_ptr<StringMap> db);

  /*
   * This function creates an inverse map, reversing keys and values.
   *
   * @param originalMap the original map
   * @return StringMap the inverse map
   */
   StringMap createInverseMap(shared_ptr<StringMap> originalMap);
};