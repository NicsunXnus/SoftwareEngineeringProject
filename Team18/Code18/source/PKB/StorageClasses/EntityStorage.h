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
    map<ENTITY, vector<string>> statement_db;
    this->statement_database = make_shared<map<ENTITY, vector<string>>>(statement_db);

    EntityMapArg procedure_db;
    this->procedure_database = make_shared<EntityMapArg>(procedure_db);

    EntityMapArg variable_db;
    this->variable_database = make_shared<EntityMapArg>(variable_db);

    EntityMapArg constant_db;
    this->constant_database = make_shared<EntityMapArg>(constant_db);
  }

    // entities categorised as statement types and non-statement types.
    shared_ptr<map<ENTITY, vector<string>>> EntityStorage::getStatementDatabase() {
        return this->statement_database;
    }

    shared_ptr<EntityMapArg> EntityStorage::getProcedureDatabase() {
        return this->procedure_database;
    }

    shared_ptr<EntityMapArg> EntityStorage::getVariableDatabase() {
        return this->variable_database;
    }

    shared_ptr<EntityMapArg> EntityStorage::getConstantDatabase() {
        return this->constant_database;
    }

    void EntityStorage::setStatementDatabase(shared_ptr<EntityMapArg> database) {
        for (auto const& [entity_string, value] : *database) {
            (*statement_database)[EntityEnumToString(entity_string)] = value;
        }
    }

    void EntityStorage::setProcedureDatabase(shared_ptr<EntityMapArg> database) {
        for (auto const& [varName, lines] : *database) {
            (*(this->procedure_database))[varName] = lines;
        }
    }

    void EntityStorage::setVariableDatabase(shared_ptr<EntityMapArg> database) {
        for (auto const& [varName, lines] : *database) {
            (*(this->variable_database))[varName] = lines;
        }
    }

    void EntityStorage::setConstantDatabase(shared_ptr<EntityMapArg> database) {
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
    static shared_ptr<map<ENTITY, vector<string>>> statement_database;

    static shared_ptr<EntityMapArg> procedure_database;
  
    static shared_ptr<EntityMapArg> variable_database;

    static shared_ptr<EntityMapArg> constant_database;
};
