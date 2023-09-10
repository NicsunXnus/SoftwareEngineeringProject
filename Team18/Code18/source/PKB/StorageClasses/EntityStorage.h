#pragma once
#include <string>
#include <map>
#include <vector>
#include <iostream>

#include "../../Constants/QPSPKB.h"

using namespace std;

/**
* This class is for the storage of entities.
* Database stores map of entity names to list of line numbers
*/
class EntityStorage {
public:
  EntityStorage() {
    statement_database = new map<ENTITY, vector<int>>();

    procedure_database = new map<string, vector<int>>();

    variable_database = new map<string, vector<int>>();

    constant_database = new map<string, vector<int>>();
    }
    // entities categorised as statement types and non-statement types.
    map<ENTITY, vector<int>>* EntityStorage::getStatementDatabase() {
        return this->statement_database;
    }

    map<string, vector<int>>* EntityStorage::getProcedureDatabase() {
        return this->procedure_database;
    }

    map<string, vector<int>>* EntityStorage::getVariableDatabase() {
      //cout << "EntityStorage::getVariableDatabase addr:" << variable_database << endl << std::flush;
      //for (auto [a, b] : *variable_database) {
      //  cout << a << endl;
      //  for (auto c : b) {
      //    cout << c << endl;
      //  }
      //}
      //std::cout.flush();
        return this->variable_database;
    }

    map<string, vector<int>>* EntityStorage::getConstantDatabase() {
        return this->constant_database;
    }

    void EntityStorage::setStatementDatabase(map<string, vector<int>>* database) {
        for (auto const& [entity_string, value] : *database) {
            (*statement_database)[EntityEnumToString(entity_string)] = value;
        }
    }

    void EntityStorage::setProcedureDatabase(map<string, vector<int>>* database) {
        this->procedure_database = database;
    }

    void EntityStorage::setVariableDatabase(map<string, vector<int>>* database) {
      for (auto const& [varName, lines] : *database) {
        (*(this->variable_database))[varName] = lines;
      }
      //cout << "EntityStorage::setVariableDatabase addr:" << variable_database << endl;
    }

    void EntityStorage::setConstantDatabase(map<string, vector<int>>* database) {
        this->constant_database = database;
    }

    void EntityStorage::printDatabase() const {
        //    for (const auto& pair : *statement_database) {
        //        string result;
        //        for (const int& num : pair.second) {
        //            result += to_string(num);
        //        }
        //        cout << "Key: " << pair.first << ", Value: " << result << endl;
        //    }
    }

private:
    static inline map<ENTITY, vector<int>>* statement_database;

    static inline map<string, vector<int>>* procedure_database;

    static inline map<string, vector<int>>* variable_database;

    static inline map<string, vector<int>>* constant_database;
};
