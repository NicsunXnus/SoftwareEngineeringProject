#pragma once
#ifndef STORAGEMANAGER_H
#define STORAGEMANAGER_H

#include <string>

#include "StorageClasses/AbstractionStorage.h"
#include "StorageClasses/EntityStorage.h"
#include "StorageClasses/UsesAbstractionStorage.h"
#include "StorageClasses/ModifiesAbstractionStorage.h"
#include "StorageClasses/FollowsAbstractionStorage.h"
#include "StorageClasses/ParentAbstractionStorage.h"

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
			case FOLLOWS:
				return follows_abstractions;
			case PARENTSTAR:  // not for insertion
				cerr << "No storage for this abstraction" << endl;
				return nullptr;
			case FOLLOWSSTAR:  // not for insertion
				cerr << "No storage for this abstraction" << endl;
				return nullptr;
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