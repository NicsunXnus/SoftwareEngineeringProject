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
    // entities categorised as statement types and non-statement types.
    map<ENTITY, vector<int>>* EntityStorage::getStatementDatabase() {
        return this->statement_database;
    }

    map<string, vector<int>>* EntityStorage::getProcedureDatabase() {
        return this->procedure_database;
    }

    map<string, vector<int>>* EntityStorage::getVariableDatabase() {
        return this->variable_database;
    }

    map<string, vector<int>>* EntityStorage::getConstantDatabase() {
        return this->constant_database;
    }

    void EntityStorage::setStatementDatabase(map<ENTITY, vector<int>>* database) {
        this->statement_database = database;
    }

    void EntityStorage::setProcedureDatabase(map<string, vector<int>>* database) {
        this->procedure_database = database;
    }

    void EntityStorage::setVariableDatabase(map<string, vector<int>>* database) {
        this->variable_database = database;
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
    map<ENTITY, vector<int>>* statement_database;

    map<string, vector<int>>* procedure_database;

    map<string, vector<int>>* variable_database;

    map<string, vector<int>>* constant_database;
};
