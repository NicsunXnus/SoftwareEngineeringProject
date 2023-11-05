#ifndef DESIGNENUMS_H
#define DESIGNENUMS_H

#include <functional>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>

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

//// Custom hash function specialization
namespace std {
template <>
struct hash<ENTITY> {
  size_t operator()(const ENTITY& e) const { return static_cast<size_t>(e); }
};
}  // namespace std
//
//struct EnumClassHash {
//  template <typename T>
//  std::size_t operator()(T t) const {
//    return static_cast<std::size_t>(t);
//  }
//};

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
};

const unordered_map<string, ABSTRACTION> abstractionMap = {
    {"uses", USES},       {"modifies", MODIFIES},
    {"parent", PARENT},   {"parentstar", PARENTSTAR},
    {"follows", FOLLOWS}, {"followsstar", FOLLOWSSTAR},
    {"calls", CALLS},     {"callsstar", CALLSSTAR},
    {"next", NEXT},
};

static ABSTRACTION AbstractionStringToEnum(string abstraction) {
  if (abstractionMap.count(abstraction)) {
    return abstractionMap.at(abstraction);
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