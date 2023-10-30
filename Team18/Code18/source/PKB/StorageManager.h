#pragma once
#ifndef STORAGEMANAGER_H
#define STORAGEMANAGER_H

#include "StorageClasses/AbstractionStorage.h"
#include "StorageClasses/EntityStorage.h"

using namespace std;

/**
 * This class represents Storage for the PKB. Consists of entity/abstraction
 * storages.
 */
class StorageManager {
 public:
  StorageManager() {
    this->entity_storage = make_shared<EntityStorage>();
    this->uses_abstractions = make_shared<UsesAbstractionStorage>();
    this->modifies_abstractions = make_shared<ModifiesAbstractionStorage>();
    this->follows_abstractions = make_shared<FollowsAbstractionStorage>();
    this->follows_star_abstractions =
        make_shared<FollowsStarAbstractionStorage>();
    this->parent_abstractions = make_shared<ParentAbstractionStorage>();
    this->parent_star_abstractions =
        make_shared<ParentStarAbstractionStorage>();
    this->calls_abstractions = make_shared<CallsAbstractionStorage>();
    this->calls_star_abstractions = make_shared<CallsStarAbstractionStorage>();
    this->next_abstractions = make_shared<NextAbstractionStorage>();

    // store all abstraction storages in a map for easy retrieval
    this->abstraction_storage_map = {{USES, uses_abstractions},
                                     {MODIFIES, modifies_abstractions},
                                     {FOLLOWS, follows_abstractions},
                                     {FOLLOWSSTAR, follows_star_abstractions},
                                     {PARENT, parent_abstractions},
                                     {PARENTSTAR, parent_star_abstractions},
                                     {CALLS, calls_abstractions},
                                     {CALLSSTAR, calls_star_abstractions},
                                     {NEXT, next_abstractions}};
  }

  ~StorageManager() {}

  /*
   * This function gets the abstraction storage, based on the
   * abstraction type.
   *
   * @param abstraction_type the type of abstraction to be inserted
   * @return shared pointer to the abstraction storage
   */
  static shared_ptr<AbstractionStorage> StorageManager::getAbstractionStorage(
      const ABSTRACTION abstraction_type) {
    return abstraction_storage_map.at(abstraction_type);
  }

  /*
   * This function gets the entity storage.
   *
   * @return shared pointer to the entity storage
   */
  static shared_ptr<EntityStorage> StorageManager::getEntityStorage() {
    return entity_storage;
  }

 private:
  // entities
  static inline shared_ptr<EntityStorage> entity_storage;

  // abstractions
  static inline shared_ptr<UsesAbstractionStorage> uses_abstractions;
  static inline shared_ptr<ModifiesAbstractionStorage> modifies_abstractions;
  static inline shared_ptr<FollowsAbstractionStorage> follows_abstractions;
  static inline shared_ptr<FollowsStarAbstractionStorage>
      follows_star_abstractions;
  static inline shared_ptr<ParentAbstractionStorage> parent_abstractions;
  static inline shared_ptr<ParentStarAbstractionStorage>
      parent_star_abstractions;
  static inline shared_ptr<CallsAbstractionStorage> calls_abstractions;
  static inline shared_ptr<CallsStarAbstractionStorage> calls_star_abstractions;
  static inline shared_ptr<NextAbstractionStorage> next_abstractions;

  // map of abstraction storages for retrieval based on enums
  static inline map<ABSTRACTION, shared_ptr<AbstractionStorage>>
      abstraction_storage_map;
};
#endif