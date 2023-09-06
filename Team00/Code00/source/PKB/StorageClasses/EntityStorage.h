#include <string>
#include <map>
#include <vector>
#include <iostream>

using namespace std;

/**
* This class is for the storage of entities.
* Database stores map of entity names to list of line numbers
*/
class EntityStorage {
public:
    // entities categorised as statement types and non-statement types.
    map<string, vector<int>>* EntityStorage::getStatementDatabase() {
        return statement_database;
    }

    map<string, vector<int>>* EntityStorage::getProcedureDatabase() {
        return procedure_database;
    }

    map<string, vector<int>>* EntityStorage::getVariableDatabase() {
        return variable_database;
    }

    map<string, vector<int>>* EntityStorage::getConstantDatabase() {
        return constant_database;
    }

    void EntityStorage::insertEntityStatement(const string& statement, const vector<int>& lines) {
        (*statement_database)[statement] = lines;
    }

    void EntityStorage::insertProcedure(const string& procedure, const vector<int>& lines) {
        (*procedure_database)[procedure] = lines;
    }

    void EntityStorage::insertVariable(const string& variable, const vector<int>& lines) {
        (*variable_database)[variable] = lines;
    }

    void EntityStorage::insertConstant(const string& constant, const vector<int>& lines) {
        (*constant_database)[constant] = lines;
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
    map<string, vector<int>>* statement_database;

    map<string, vector<int>>* procedure_database;

    map<string, vector<int>>* variable_database;

    map<string, vector<int>>* constant_database;
};
