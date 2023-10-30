#ifndef ABSTRACTIONSTORAGE_H
#define ABSTRACTIONSTORAGE_H
#include <iostream>
#include <map>
#include <string>
#include <unordered_set>
#include <variant>
#include <vector>

#include "../../Constants/DesignEnums.h"

using namespace std;

/**
 * This class is the base class for abstraction storage.
 */
class AbstractionStorage {
 public:
  shared_ptr<StringMap> AbstractionStorage::getDatabase() {
    return this->database;
  }

  void AbstractionStorage::setAbstraction(
      shared_ptr<StringMap> database_to_insert) {
    StringMap new_database;
    this->database = make_shared<StringMap>(new_database);
    for (auto const& [key, lines] : *database_to_insert) {
      (*(this->database))[key] = lines;
    }
  }

 private:
  shared_ptr<StringMap> database;
};

/**
 * This class represents the storage for the Uses design abstractions.
 * key: variable v
 * set of values : line numbers(for non - procedure) or procedure names
 * essentially, in a typical Uses(x, v), v will be the key and x will be the
 * possible values in the vector.
 */
class UsesAbstractionStorage : public AbstractionStorage {};

/**
 * This class represents the storage for the Modifies design abstractions.
 * key: variable v
 * set of values : line numbers(for non - procedure) or procedure names
 * essentially, in a typical Modifies(x, v), v will be the key and x will be the
 * possible values in the vector.
 */
class ModifiesAbstractionStorage : public AbstractionStorage {};

/**
 * This class represents the storage for the Follows/Follows* design
 * abstractions. key: line number set of values : The one line number that
 * follows.
 */
class FollowsAbstractionStorage : public AbstractionStorage {};

/**
 * This class represents the storage for the Follows/Follows* design
 * abstractions. key: line number set of values : all line numbers that
 * follows*.
 */
class FollowsStarAbstractionStorage : public AbstractionStorage {};

/**
 * This class represents the storage for the Parent/Parent* design abstractions.
 * key: line number
 * set of values : The one line number that is the direct parent
 */
class ParentAbstractionStorage : public AbstractionStorage {};

/**
 * This class represents the storage for the Parent/Parent* design abstractions.
 * key: line number
 * set of values : all line numbers that are parent* (parent, grandparent etc.).
 */
class ParentStarAbstractionStorage : public AbstractionStorage {};

/**
 * This class represents the storage for the Calls/Calls* design abstractions.
 * key: procedure name
 * set of values : The procedure names of procedures directly called.
 */
class CallsAbstractionStorage : public AbstractionStorage {};

/**
 * This class represents the storage for the Calls/Calls* design abstractions.
 * key: procedure name
 * set of values : all procedures that calls*.
 */
class CallsStarAbstractionStorage : public AbstractionStorage {};

/**
 * This class represents the storage for the Next design abstractions.
 * key: line number of statement
 * set of values : line number of statement that is next. vector has one element
 * only
 */
class NextAbstractionStorage : public AbstractionStorage {};

#endif