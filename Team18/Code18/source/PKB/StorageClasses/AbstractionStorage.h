#ifndef ABSTRACTIONSTORAGE_H
#define ABSTRACTIONSTORAGE_H
#include <map>
#include <vector>
#include <variant>
#include <iostream>

using namespace std;

/**
* This class is the base class for storage.
* Map has vector<int> as values, but keys can be any type.
*/
class AbstractionStorage {
public:
    map<string, vector<string>>* AbstractionStorage::getDatabase() {
        return database;
    }

    void AbstractionStorage::printDatabase() const {
        cout << "printing abstraction database";
        for (const auto& pair : *database) {
            string result;
            for (const string& num : pair.second) {
                result += num;
            }
            cout << "Key: " << pair.first << ", Value: " << result << endl;
        }
    }
    
    void AbstractionStorage::insertAbstraction(const string& key, const vector<string>& lines) {
        (*database)[key] = lines;
    }

private:
    map<string, vector<string>>* database;
};

#endif