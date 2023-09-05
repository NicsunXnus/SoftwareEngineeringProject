#include "EntityStorage.h"

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

void EntityStorage::insertEntityStatement(const string& key, const vector<int>& value) {
    // check which database... if "variable/procedure/constant"
    // else use statement_database
    if (key == "variable")

    (*statement_database)[key] = value;
}

void EntityStorage::printDatabase() const {
    for (const auto& pair : *statement_database) {
        string result;
        for (const int& num : pair.second) {
            result += to_string(num);
        }
        cout << "Key: " << pair.first << ", Value: " << result << endl;
    }
}