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
class EntityStorage {
 public:
  EntityStorage() {
    // statement entities
    this->statement_database =
        make_shared<map<ENTITY, unordered_set<string>>>();
    // non-statement entities
    this->procedure_database = make_shared<StringMap>();
    this->variable_database = make_shared<StringMap>();
    this->constant_database = make_shared<StringMap>();
    // procedure line numbers
    this->proclines_database = make_shared<map<string, pair<string, string>>>();
    // attributes
    this->call_procname_database = make_shared<StringMap>();
    this->read_varname_database = make_shared<StringMap>();
    this->print_varname_database = make_shared<StringMap>();
    // patterns
    this->pattern_database = make_shared<map<string, shared_ptr<Node>>>();
  }

  ~EntityStorage() {}

  // entities categorised as statement types and non-statement types.
  shared_ptr<map<ENTITY, unordered_set<string>>>
  EntityStorage::getStatementDatabase() {
    if (!(this->statement_database)) {
      cerr << "stmt_database is null" << endl;
      return nullptr;
    }
    return this->statement_database;
  }

  shared_ptr<StringMap> EntityStorage::getProcedureDatabase() {
    if (!(this->procedure_database)) {
      cerr << "proc_database is null" << endl;
      return nullptr;
    }
    return this->procedure_database;
  }

  shared_ptr<StringMap> EntityStorage::getVariableDatabase() {
    if (!(this->variable_database)) {
      cerr << "var_database is null" << endl;
      return nullptr;
    }
    return this->variable_database;
  }

  shared_ptr<StringMap> EntityStorage::getConstantDatabase() {
    if (!(this->constant_database)) {
      cerr << "const_database is null" << endl;
      return nullptr;
    }
    return this->constant_database;
  }

  shared_ptr<map<string, pair<string, string>>>
  EntityStorage::getProcLinesDatabase() {
    if (!(this->proclines_database)) {
      cerr << "proclines_database is null" << endl;
      return nullptr;
    }
    return this->proclines_database;
  }

  shared_ptr<StringMap> EntityStorage::getCallProcnameDatabase() {
    if (!(this->call_procname_database)) {
      cerr << "call_procname_database is null" << endl;
      return nullptr;
    }
    return this->call_procname_database;
  }

  shared_ptr<StringMap> EntityStorage::getReadVarnameDatabase() {
    if (!(this->read_varname_database)) {
      cerr << "read_varname_database is null" << endl;
      return nullptr;
    }
    return this->read_varname_database;
  }

  shared_ptr<StringMap> EntityStorage::getPrintVarnameDatabase() {
    if (!(this->print_varname_database)) {
      cerr << "print_varname_database is null" << endl;
      return nullptr;
    }
    return this->print_varname_database;
  }

  shared_ptr<map<string, shared_ptr<Node>>>
  EntityStorage::getPatternDatabase() {
    if (!(this->pattern_database)) {
      cerr << "pattern_database is null" << endl;
      return nullptr;
    }
    return this->pattern_database;
  }

  void EntityStorage::setStatementDatabase(shared_ptr<StringMap> database) {
    static map<ENTITY, unordered_set<string>> statement_db;
    this->statement_database =
        make_shared<map<ENTITY, unordered_set<string>>>(statement_db);
    for (auto const& [entity_string, value] : *database) {
      (*statement_database)[EntityStringToEnum(entity_string)] = value;
    }
  }

  void EntityStorage::setProcedureDatabase(shared_ptr<StringMap> database) {
    static StringMap procedure_db;
    this->procedure_database = make_shared<StringMap>(procedure_db);
    for (auto const& [procName, lines] : *database) {
      (*(this->procedure_database))[procName] = lines;
    }
  }

  void EntityStorage::setVariableDatabase(shared_ptr<StringMap> database) {
    static StringMap variable_db;
    this->variable_database = make_shared<StringMap>(variable_db);
    for (auto const& [varName, lines] : *database) {
      (*(this->variable_database))[varName] = lines;
    }
  }

  void EntityStorage::setConstantDatabase(shared_ptr<StringMap> database) {
    static StringMap constant_db;
    this->constant_database = make_shared<StringMap>(constant_db);
    for (auto const& [constName, lines] : *database) {
      (*(this->constant_database))[constName] = lines;
    }
  }

  void EntityStorage::setProcLinesDatabase(
      shared_ptr<map<string, pair<string, string>>> database) {
    static map<string, pair<string, string>> proclines_db;
    this->proclines_database =
        make_shared<map<string, pair<string, string>>>(proclines_db);
    for (auto const& [procName, lines] : *database) {
      (*(this->proclines_database))[procName] = lines;
    }
  }

  void EntityStorage::setCallProcnameDatabase(shared_ptr<StringMap> database) {
    static StringMap call_procname_db;
    this->call_procname_database = make_shared<StringMap>(call_procname_db);
    for (auto const& [varName, lines] : *database) {
      (*(this->call_procname_database))[varName] = lines;
    }
  }

  void EntityStorage::setReadVarnameDatabase(shared_ptr<StringMap> database) {
    static StringMap read_varname_db;
    this->read_varname_database = make_shared<StringMap>(read_varname_db);
    for (auto const& [varName, lines] : *database) {
      (*(this->read_varname_database))[varName] = lines;
    }
  }

  void EntityStorage::setPrintVarnameDatabase(shared_ptr<StringMap> database) {
    static StringMap print_varname_db;
    this->print_varname_database = make_shared<StringMap>(print_varname_db);
    for (auto const& [varName, lines] : *database) {
      (*(this->print_varname_database))[varName] = lines;
    }
  }

  void EntityStorage::setPatternDatabase(
      shared_ptr<map<string, shared_ptr<Node>>> database) {
    static map<string, shared_ptr<Node>> pattern_db;
    this->pattern_database =
        make_shared<map<string, shared_ptr<Node>>>(pattern_db);
    for (auto const& [varName, node] : *database) {
      (*(this->pattern_database))[varName] = node;
    }
  }

  void EntityStorage::printDatabase() const {
    for (const auto& pair : *statement_database) {
      string result;
      for (const string& s : pair.second) {
        result += s;
      }
      cout << "Key: " << pair.first << ", Value: " << result << endl;
    }
  }

 private:
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
};
