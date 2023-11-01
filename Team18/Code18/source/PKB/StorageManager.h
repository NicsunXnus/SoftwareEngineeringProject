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
   StorageManager();

  ~StorageManager() {}

  /*
   * This function gets the abstraction storage, based on the
   * abstraction type.
   *
   * @param abstraction_type the type of abstraction to be inserted
   * @return shared pointer to the abstraction storage
   */
  static shared_ptr<AbstractionStorage> StorageManager::getAbstractionStorage(
    const ABSTRACTION abstraction_type);

  /*
   * This function gets the entity storage.
   *
   * @return shared pointer to the entity storage
   */
  static shared_ptr<EntityStorage> StorageManager::getEntityStorage();

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