#pragma once
#include <iostream>
#include <map>
#include <string>
#include <unordered_set>
#include <vector>

#include "../../Constants/DesignEnums.h"
#include "../../SP/AST/Node.h"

using namespace std;

/**
 * This class is for the storage of entities.
 */
class EntityStorage {};

class StatementStorage : public EntityStorage {
 public:
  shared_ptr<map<ENTITY, unordered_set<string>>>
  StatementStorage::getDatabase();

  void StatementStorage::setEntity(
      shared_ptr<StringMap> database_to_insert);

 private:
  shared_ptr<map<ENTITY, unordered_set<string>>> database;
};

class NonStatementStorage : public EntityStorage {
 public:
  shared_ptr<StringMap> NonStatementStorage::getDatabase();

  void NonStatementStorage::setEntity(shared_ptr<StringMap> database_to_insert);

 private:
  shared_ptr<StringMap> database;
};

class ProcedureStorage : public NonStatementStorage {};

class VariableStorage : public NonStatementStorage {};

class ConstantStorage : public NonStatementStorage {};

class NameStorage : public NonStatementStorage {};

class CallProcnameStorage : public NameStorage {};

class ReadVarnameStorage : public NameStorage {};

class PrintVarnameStorage : public NameStorage {};

class ProcLinesStorage : public EntityStorage {
 public:
  shared_ptr<map<string, pair<string, string>>> ProcLinesStorage::getDatabase();

  void ProcLinesStorage::setProcLines(
      shared_ptr<map<string, pair<string, string>>> database_to_insert);

 private:
  shared_ptr<map<string, pair<string, string>>> database;
};

class PatternStorage : public EntityStorage {
 public:
  shared_ptr<map<string, shared_ptr<Node>>> PatternStorage::getDatabase();

  void PatternStorage::setPattern(
      shared_ptr<map<string, shared_ptr<Node>>> database_to_insert);

 private:
  shared_ptr<map<string, shared_ptr<Node>>> database;
};

/*
  static inline shared_ptr<map<ENTITY, unordered_set<string>>>
      statement_database;
  static inline shared_ptr<StringMap> procedure_database;
  static inline shared_ptr<StringMap> variable_database;
  static inline shared_ptr<StringMap> constant_database;
  static inline shared_ptr<map<string, pair<string, string>>>
      proclines_database;
  static inline shared_ptr<StringMap> call_procname_database;
  static inline shared_ptr<StringMap> read_varname_database;
  static inline shared_ptr<StringMap> print_varname_database;
  static inline shared_ptr<map<string, shared_ptr<Node>>> pattern_database;
*/

// class EntityStorage {
//  public:
//   /**
//    * This is the constructor for the EntityStorage class.
//    * Initializes all the databases.
//    */
//   EntityStorage() {
//     // statement entities
//     this->statement_database =
//         make_shared<map<ENTITY, unordered_set<string>>>();
//     // non-statement entities
//     this->procedure_database = make_shared<StringMap>();
//     this->variable_database = make_shared<StringMap>();
//     this->constant_database = make_shared<StringMap>();
//     // procedure line numbers
//     this->proclines_database = make_shared<map<string, pair<string,
//     string>>>();
//     // attributes
//     this->call_procname_database = make_shared<StringMap>();
//     this->read_varname_database = make_shared<StringMap>();
//     this->print_varname_database = make_shared<StringMap>();
//     // patterns
//     this->pattern_database = make_shared<map<string, shared_ptr<Node>>>();
//   }

///*
// * This function gets statement database.
// *
// * @return shared_ptr<map<ENTITY, unordered_set<string>>> statement database
// */
//shared_ptr<map<ENTITY, unordered_set<string>>>
//EntityStorage::getStatementDatabase() {
//  return this->statement_database;
//}
//
///*
// * This function gets procedure database.
// *
// * @return shared_ptr<StringMap> procedure database
// */
//shared_ptr<StringMap> EntityStorage::getProcedureDatabase() {
//  return this->procedure_database;
//}
//
///*
// * This function gets variable database.
// *
// * @return shared_ptr<StringMap> variable database
// */
//shared_ptr<StringMap> EntityStorage::getVariableDatabase() {
//  return this->variable_database;
//}
//
///*
// * This function gets constant database.
// *
// * @return shared_ptr<StringMap> constant database
// */
//shared_ptr<StringMap> EntityStorage::getConstantDatabase() {
//  return this->constant_database;
//}

/*
 * This function gets procedure line numbers database.
 *
 * @return shared_ptr<map<string, pair<string, string>>> procedure line
 * numbers database
 */
//shared_ptr<map<string, pair<string, string>>>
//EntityStorage::getProcLinesDatabase() {
//  return this->proclines_database;
//}
//
///*
// * This function gets call procname database.
// *
// * @return shared_ptr<StringMap> call procname database
// */
//shared_ptr<StringMap> EntityStorage::getCallProcnameDatabase() {
//  return this->call_procname_database;
//}
//
///*
// * This function gets read varname database.
// *
// * @return shared_ptr<StringMap> read varname database
// */
//shared_ptr<StringMap> EntityStorage::getReadVarnameDatabase() {
//  return this->read_varname_database;
//}
//
///*
// * This function gets print varname database.
// *
// * @return shared_ptr<StringMap> print varname database
// */
//shared_ptr<StringMap> EntityStorage::getPrintVarnameDatabase() {
//  return this->print_varname_database;
//}
//
///*
// * This function gets pattern database.
// *
// * @return shared_ptr<map<string, shared_ptr<Node>>> pattern database
// */
//shared_ptr<map<string, shared_ptr<Node>>> EntityStorage::getPatternDatabase() {
//  return this->pattern_database;
//}

/*
 * This function sets statement database.
 *
 * @param database statement database
 */
//void EntityStorage::setStatementDatabase(shared_ptr<StringMap> database) {
//  static map<ENTITY, unordered_set<string>> statement_db;
//  this->statement_database =
//      make_shared<map<ENTITY, unordered_set<string>>>(statement_db);
//  for (auto const& [entity_string, value] : *database) {
//    (*statement_database)[EntityStringToEnum(entity_string)] = value;
//  }
//}
//
///*
// * This function sets procedure database.
// *
// * @param database procedure database
// */
//void EntityStorage::setProcedureDatabase(shared_ptr<StringMap> database) {
//  static StringMap procedure_db;
//  this->procedure_database = make_shared<StringMap>(procedure_db);
//  for (auto const& [procName, lines] : *database) {
//    (*(this->procedure_database))[procName] = lines;
//  }
//}
//
///*
// * This function sets variable database.
// *
// * @param database variable database
// */
//void EntityStorage::setVariableDatabase(shared_ptr<StringMap> database) {
//  static StringMap variable_db;
//  this->variable_database = make_shared<StringMap>(variable_db);
//  for (auto const& [varName, lines] : *database) {
//    (*(this->variable_database))[varName] = lines;
//  }
//}
//
///*
// * This function sets constant database.
// *
// * @param database constant database
// */
//void EntityStorage::setConstantDatabase(shared_ptr<StringMap> database) {
//  static StringMap constant_db;
//  this->constant_database = make_shared<StringMap>(constant_db);
//  for (auto const& [constName, lines] : *database) {
//    (*(this->constant_database))[constName] = lines;
//  }
//}
//
///*
// * This function sets procedure line numbers database.
// *
// * @param database procedure line numbers database
// */
//void EntityStorage::setProcLinesDatabase(
//    shared_ptr<map<string, pair<string, string>>> database) {
//  static map<string, pair<string, string>> proclines_db;
//  this->proclines_database =
//      make_shared<map<string, pair<string, string>>>(proclines_db);
//  for (auto const& [procName, lines] : *database) {
//    (*(this->proclines_database))[procName] = lines;
//  }
//}
//
///*
// * This function sets call procname database.
// *
// * @param database call procname database
// */
//void EntityStorage::setCallProcnameDatabase(shared_ptr<StringMap> database) {
//  static StringMap call_procname_db;
//  this->call_procname_database = make_shared<StringMap>(call_procname_db);
//  for (auto const& [varName, lines] : *database) {
//    (*(this->call_procname_database))[varName] = lines;
//  }
//}
//
///*
// * This function sets read varname database.
// *
// * @param database read varname database
// */
//void EntityStorage::setReadVarnameDatabase(shared_ptr<StringMap> database) {
//  static StringMap read_varname_db;
//  this->read_varname_database = make_shared<StringMap>(read_varname_db);
//  for (auto const& [varName, lines] : *database) {
//    (*(this->read_varname_database))[varName] = lines;
//  }
//}
//
///*
// * This function sets print varname database.
// *
// * @param database print varname database
// */
//void EntityStorage::setPrintVarnameDatabase(shared_ptr<StringMap> database) {
//  static StringMap print_varname_db;
//  this->print_varname_database = make_shared<StringMap>(print_varname_db);
//  for (auto const& [varName, lines] : *database) {
//    (*(this->print_varname_database))[varName] = lines;
//  }
//}
//
///*
// * This function sets pattern database.
// *
// * @param database pattern database
// */
//void EntityStorage::setPatternDatabase(
//    shared_ptr<map<string, shared_ptr<Node>>> database) {
//  static map<string, shared_ptr<Node>> pattern_db;
//  this->pattern_database =
//      make_shared<map<string, shared_ptr<Node>>>(pattern_db);
//  for (auto const& [varName, node] : *database) {
//    (*(this->pattern_database))[varName] = node;
//  }
//}
//
//private:
//static inline shared_ptr<map<ENTITY, unordered_set<string>>> statement_database;
//static inline shared_ptr<StringMap> procedure_database;
//static inline shared_ptr<StringMap> variable_database;
//static inline shared_ptr<StringMap> constant_database;
//static inline shared_ptr<map<string, pair<string, string>>> proclines_database;
//static inline shared_ptr<StringMap> call_procname_database;
//static inline shared_ptr<StringMap> read_varname_database;
//static inline shared_ptr<StringMap> print_varname_database;
//static inline shared_ptr<map<string, shared_ptr<Node>>> pattern_database;
//}
//;
