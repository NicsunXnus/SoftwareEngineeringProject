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

    map<string, vector<int>>* getStatementDatabase();

    map<string, vector<int>>* getProcedureDatabase();

    map<string, vector<int>>* getVariableDatabase();

    map<string, vector<int>>* getConstantDatabase();

    void insertEntityStatement(const string& statement, const vector<int>& lines);

    void insertProcedure(const string& procedure, const vector<int>& lines);

    void insertVariable(const string& variable, const vector<int>& lines);

    void insertConstant(const string& constant, const vector<int>& lines);

    void printDatabase() const;

private:
    map<string, vector<int>>* statement_database;

    map<string, vector<int>>* procedure_database;

    map<string, vector<int>>* variable_database;

    map<string, vector<int>>* constant_database;
};
