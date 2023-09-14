#ifndef ABSTRACTIONSTORAGE_H
#define ABSTRACTIONSTORAGE_H
#include <map>
#include <vector>
#include <variant>
#include <iostream>

#include "../../DesignExtractor/Entity.h"

using namespace std;

/**
* This class is the base class for abstraction storage.
*/
class AbstractionStorage {
public:
    shared_ptr<StringMap> AbstractionStorage::getDatabase() {
        return this->database;
    }
    
    shared_ptr<StringMap> AbstractionStorage::getTruncatedDatabase() {
        static StringMap truncated_database;
        for (auto const& [key, lines] : *database) {
            truncated_database[key] = vector<string>({ lines.front() });
        }
        return make_shared<StringMap>(truncated_database);
    }

    void AbstractionStorage::printDatabase() const {
        cout << "printing abstraction database" << endl;
        for (const auto& pair : *database) {
            string result;
            for (const string& num : pair.second) {
                result += num;
            }
            cout << "Key: " << pair.first << ", Value: " << result << endl;
        }
    }
    
    void AbstractionStorage::setAbstraction(shared_ptr<StringMap> database_to_insert) {
        static StringMap new_database;
        this->database = make_shared<StringMap>(new_database);
        for (auto const& [key, lines] : *database_to_insert) {
            (*(this->database))[key] = lines;
        }
    }

private:
    static inline shared_ptr<StringMap> database;
};

#endif