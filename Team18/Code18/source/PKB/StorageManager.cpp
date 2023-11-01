#include "StorageManager.h"


StorageManager::StorageManager() {
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

shared_ptr<AbstractionStorage> StorageManager::getAbstractionStorage(
    const ABSTRACTION abstraction_type) {
  return abstraction_storage_map.at(abstraction_type);
}

/*
  * This function gets the entity storage.
  *
  * @return shared pointer to the entity storage
  */
shared_ptr<EntityStorage> StorageManager::getEntityStorage() {
  return entity_storage;
}