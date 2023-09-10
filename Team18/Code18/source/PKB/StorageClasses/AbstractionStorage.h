#ifndef ABSTRACTIONSTORAGE_H
#define ABSTRACTIONSTORAGE_H
#include <map>
#include <vector>
#include <variant>
#include <iostream>

using namespace std;

typedef variant<int, string> KeyValue;

/**
* This class is the base class for storage.
* Map has vector<int> as values, but keys can be any type.
*/
class AbstractionStorage {
public:
    map<KeyValue, vector<int>>* AbstractionStorage::getDatabase() {
        return database;
    }

    void AbstractionStorage::printDatabase() const {
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
            //std::cout << "Key: " << key << ", Value: " << result << std::endl;
        }
    }
    
    void AbstractionStorage::insertAbstraction(const KeyValue& key, const vector<int>& lines) {
        (*database)[key] = lines;
    }

private:
    map<KeyValue, vector<int>>* database;
};

#endif