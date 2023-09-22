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
        this->statement_database = make_shared<map<ENTITY, vector<string>>>();
        this->procedure_database = make_shared<StringMap>();
        this->variable_database = make_shared<StringMap>();
        this->constant_database = make_shared<StringMap>();
    }

    ~EntityStorage() {}

    // entities categorised as statement types and non-statement types.
    shared_ptr<map<ENTITY, vector<string>>> EntityStorage::getStatementDatabase() {
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

    void EntityStorage::setStatementDatabase(shared_ptr<StringMap> database) {
        static map<ENTITY, vector<string>> statement_db;
        this->statement_database = make_shared<map<ENTITY, vector<string>>>(statement_db);
        for (auto const& [entity_string, value] : *database) {
            (*statement_database)[EntityEnumToString(entity_string)] = value;
        }
    }

    void EntityStorage::setProcedureDatabase(shared_ptr<StringMap> database) {
        static StringMap procedure_db;
        this->procedure_database = make_shared<StringMap>(procedure_db);
        for (auto const& [varName, lines] : *database) {
            (*(this->procedure_database))[varName] = lines;
        }
    }

    void EntityStorage::setVariableDatabase(shared_ptr<StringMap> database) {
        cout << "setVariableDatabase" << endl;
        cout << "database size: " << database->size() << endl;
        static StringMap variable_db;
        this->variable_database = make_shared<StringMap>(variable_db);
        for (auto const& [varName, lines] : *database) {
            (*(this->variable_database))[varName] = lines;
        }
    }

    void EntityStorage::setConstantDatabase(shared_ptr<StringMap> database) {
        static StringMap constant_db;
        this->constant_database = make_shared<StringMap>(constant_db);
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
    static inline shared_ptr<map<ENTITY, vector<string>>> statement_database;

    static inline shared_ptr<StringMap> procedure_database;
  
    static inline shared_ptr<StringMap> variable_database;

    static inline shared_ptr<StringMap> constant_database;
};
