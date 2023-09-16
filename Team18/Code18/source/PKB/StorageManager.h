#pragma once
#ifndef STORAGEMANAGER_H
#define STORAGEMANAGER_H

#include <string>

#include "StorageClasses/AbstractionStorage.h"
#include "StorageClasses/EntityStorage.h"

using namespace std;

/**
* This class represents Storage for the PKB. Consists of the storages and AST
*/
class StorageManager {
public:
	static shared_ptr<AbstractionStorage> StorageManager::getAbstractionStorage(const ABSTRACTION abstraction_type) {
		switch (abstraction_type) {
			case USES:
				return uses_abstractions;
			case MODIFIES:
				return modifies_abstractions;
			case PARENT:
				return parent_abstractions;
			case PARENTSTAR:
				return parent_abstractions; 
			case FOLLOWS:
				return follows_abstractions;
			case FOLLOWSSTAR:
				return follows_abstractions;
			default:
				cerr << "No such abstraction found" << endl;
		}
	}

	static shared_ptr<EntityStorage> StorageManager::getEntityStorage() {
		return entity_storage;
	}

private:
	// parse trees (for querying patterns)

	// entities
	static inline shared_ptr<EntityStorage> entity_storage;

	// abstractions
	static inline shared_ptr<UsesAbstractionStorage> uses_abstractions;
	static inline shared_ptr<ModifiesAbstractionStorage> modifies_abstractions;
	static inline shared_ptr<FollowsAbstractionStorage> follows_abstractions;
	static inline shared_ptr<ParentAbstractionStorage> parent_abstractions;
};
#endif