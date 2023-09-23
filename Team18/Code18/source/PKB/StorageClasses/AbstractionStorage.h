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
        StringMap truncated_database;
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
        StringMap new_database;
        this->database = make_shared<StringMap>(new_database);
        for (auto const& [key, lines] : *database_to_insert) {
            (*(this->database))[key] = lines;
        }
    }

private:
    shared_ptr<StringMap> database;
};

// This class represents the storage for the Follows/Follows* design abstractions.
// key: line number
// vector of values : all line numbers that follows* .The first element of the vector will be the one that follows.
class FollowsAbstractionStorage : public AbstractionStorage {};

// This class represents the storage for the Modifies design abstractions.
// key: variable v
// vector of values : line numbers(for non - procedure) or procedure names
// essentially, in a typical Modifies(x, v), v will be the key and x will be the possible values in the vector.
class ModifiesAbstractionStorage : public AbstractionStorage {};

// This class represents the storage for the Parent/Parent* design abstractions.
// key: line number
// vector of values : all line numbers that are parent* (parent, grandparent etc.).The first element of the vector will be the one that is the direct parent.
class ParentAbstractionStorage : public AbstractionStorage {};

// This class represents the storage for the Uses design abstractions.
// key: variable v
// vector of values : line numbers(for non - procedure) or procedure names
// essentially, in a typical Uses(x, v), v will be the key and x will be the possible values in the vector.
class UsesAbstractionStorage : public AbstractionStorage {};

#endif