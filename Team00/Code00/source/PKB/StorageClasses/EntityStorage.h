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
    map<string, vector<int>>* getDatabase();

    void insertEntity(const string& key, const vector<int>& value);

    void printDatabase() const;

private:
    map<string, vector<int>>* database;
};
