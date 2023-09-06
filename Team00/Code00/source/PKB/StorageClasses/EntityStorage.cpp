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
    for (const auto& pair : *statement_database) {
        string result;
        for (const int& num : pair.second) {
            result += to_string(num);
        }
        cout << "Key: " << pair.first << ", Value: " << result << endl;
    }
}