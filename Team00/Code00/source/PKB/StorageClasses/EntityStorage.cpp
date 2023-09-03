#include "EntityStorage.h"

map<string, vector<int>>* EntityStorage::getDatabase()
{
	return database;
}

void EntityStorage::insert_one(const string& key, const vector<int>& value)
{
}

void EntityStorage::print_rows() const {
    for (const auto& pair : *database) {
        string result;
        for (const int& num : pair.second) {
            result += to_string(num);
        }
        cout << "Key: " << pair.first << ", Value: " << result << endl;
    }
}