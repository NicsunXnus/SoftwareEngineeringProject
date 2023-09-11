#pragma once
#include <string>
#include <map>
#include <vector>
#include <iostream>

#include "../../Constants/QPSPKB.h"
#include "../../DesignExtractor/Entity.h"

using namespace std;

/**
* This class is for the storage of entities.
* Database stores map of entity names to list of line numbers
*/
class EntityStorage {
public:
  EntityStorage() {
    statement_database = new map<ENTITY, vector<string>>();

    procedure_database = new map<string, vector<string>>();

    variable_database = new map<string, vector<string>>();

    constant_database = new map<string, vector<string>>();
    }

    // entities categorised as statement types and non-statement types.
    map<ENTITY, vector<string>>* EntityStorage::getStatementDatabase() {
        return this->statement_database;
    }

    EntityMapArg EntityStorage::getProcedureDatabase() {
        return this->procedure_database;
    }

    EntityMapArg EntityStorage::getVariableDatabase() {
        return this->variable_database;
    }

    EntityMapArg EntityStorage::getConstantDatabase() {
        return this->constant_database;
    }

    void EntityStorage::setStatementDatabase(EntityMapArg database) {
        for (auto const& [entity_string, value] : *database) {
            (*statement_database)[EntityEnumToString(entity_string)] = value;
        }
    }

    void EntityStorage::setProcedureDatabase(EntityMapArg database) {
        for (auto const& [varName, lines] : *database) {
            (*(this->procedure_database))[varName] = lines;
        }
    }

    void EntityStorage::setVariableDatabase(EntityMapArg database) {
        for (auto const& [varName, lines] : *database) {
            (*(this->variable_database))[varName] = lines;
        }
    }

    void EntityStorage::setConstantDatabase(EntityMapArg database) {
        for (auto const& [varName, lines] : *database) {
            (*(this->constant_database))[varName] = lines;
        }
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
    static inline map<ENTITY, vector<string>>* statement_database;

    static inline map<string, vector<string>>* procedure_database;

    static inline map<string, vector<string>>* variable_database;

    static inline map<string, vector<string>>* constant_database;
};
