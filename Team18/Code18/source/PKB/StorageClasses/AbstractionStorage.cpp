#include "AbstractionStorage.h"

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