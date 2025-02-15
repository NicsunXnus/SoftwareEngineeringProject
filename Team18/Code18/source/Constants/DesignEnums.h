#ifndef DESIGNENUMS_H
#define DESIGNENUMS_H

#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <map>

using namespace std;

typedef unordered_map<string, unordered_set<string>> StringMap;

enum ENTITY {
  STMT,
  READ,
  PRINT,
  CALL,
  WHILE,
  IF,
  ASSIGN,
  VARIABLE,
  CONSTANT,
  PROCEDURE
};

const unordered_map<string, ENTITY> entityMap = {
    {"stmt", STMT},           {"read", READ},         {"print", PRINT},
    {"call", CALL},           {"while", WHILE},       {"if", IF},
    {"assign", ASSIGN},       {"variable", VARIABLE}, {"constant", CONSTANT},
    {"procedure", PROCEDURE},
};

static ENTITY EntityStringToEnum(string entity) {
  if (entityMap.count(entity)) {
    return entityMap.at(entity);
  }
  throw runtime_error("entity is invalid!");
}

enum ABSTRACTION {
  USES,
  MODIFIES,
  PARENT,
  PARENTSTAR,
  FOLLOWS,
  FOLLOWSSTAR,
  CALLS,
  CALLSSTAR,
  NEXT,
  NEXTSTAR,
  AFFECTS,
};

const unordered_map<string, ABSTRACTION> abstractionMap = {
    {"uses", USES},       {"modifies", MODIFIES},
    {"parent", PARENT},   {"parentstar", PARENTSTAR},
    {"follows", FOLLOWS}, {"followsstar", FOLLOWSSTAR},
    {"calls", CALLS},     {"callsstar", CALLSSTAR},
    {"next", NEXT},       {"nextstar", NEXTSTAR},
    {"affects", AFFECTS}};

static ABSTRACTION AbstractionStringToEnum(string abstraction) {
  if (abstractionMap.count(abstraction)) {
    return abstractionMap.at(abstraction);
  }
  throw runtime_error("abstraction is invalid!");
}

const unordered_map<string, ABSTRACTION> queryObjAbstractionMap = {
    {"Uses", USES},       {"Modifies", MODIFIES},
    {"Parent", PARENT},   {"Parent*", PARENTSTAR},
    {"Follows", FOLLOWS}, {"Follows*", FOLLOWSSTAR},
    {"Calls", CALLS},     {"Calls*", CALLSSTAR},
    {"Next", NEXT},       {"Next*", NEXTSTAR},
    {"Affects", AFFECTS}};

static ABSTRACTION QueryObjStringToEnum(string abstraction) {
  if (queryObjAbstractionMap.count(abstraction)) {
    return queryObjAbstractionMap.at(abstraction);
  }
  throw runtime_error("abstraction is invalid!");
}

// list of storages that need to be 'flipped' for return.
// e.g. Parent(x, y) is stored as {y: {x}} in the storage, but we want to return
// {x: {y}} to follow argument order
const unordered_set<ABSTRACTION> flippedStorages = {PARENT, PARENTSTAR, USES,
                                                    MODIFIES};

static bool isFlippedStorage(ABSTRACTION abstraction) {
  return flippedStorages.count(abstraction);
}

#endif