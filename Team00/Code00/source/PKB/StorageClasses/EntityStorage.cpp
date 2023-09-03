#include "EntityStorage.h"

map<string, vector<int>>* EntityStorage::getDatabase()
{
	return database;
}

void EntityStorage::insertEntity(const string& key, const vector<int>& value) {
    (*database)[key] = value;
}

void EntityStorage::printDatabase() const {
    for (const auto& pair : *database) {
        string result;
        for (const int& num : pair.second) {
            result += to_string(num);
        }
        cout << "Key: " << pair.first << ", Value: " << result << endl;
    }
}