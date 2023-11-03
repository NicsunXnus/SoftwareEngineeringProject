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
