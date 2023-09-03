#include <map>
#include <string>
#include <iostream>

#include "AbstractionStorage.h"


using namespace std;

void AbstractionStorage::insert_one(const KeyValue& key, const int& value)
{
	// to be implemented
}

map<KeyValue, vector<int>>* AbstractionStorage::getDatabase()
{
	return database;
}

void AbstractionStorage::print_rows() const {
    for (const auto& pair : *database) {
        string result;
        for (const int& num : pair.second) {
            result += to_string(num);
        }
        string key;
        if (holds_alternative<int>(pair.first)) {
            key = to_string(get<int>(pair.first));
        }
        else {
            key = get<std::string>(pair.first);
        }
        cout << "Key: " << key << ", Value: " << result << endl;
    }
}