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

static shared_ptr<EntityStorage> entity_storage;
static shared_ptr<UsesAbstractionStorage> uses_abstractions;
static shared_ptr<ModifiesAbstractionStorage> modifies_abstractions;
static shared_ptr<FollowsAbstractionStorage> follows_abstractions;
static shared_ptr<ParentAbstractionStorage> parent_abstractions;

/**
* This class represents Storage for the PKB. Consists of the storages and AST
*/
class StorageManager {
public:

	static shared_ptr<AbstractionStorage> StorageManager::getAbstractionStorage(const string abstraction) {
		if (abstraction == "uses") {
			return uses_abstractions;
		}
		else if (abstraction == "modifies") {
			return modifies_abstractions;
		}
		else if (abstraction == "follows") {
			return follows_abstractions;
		}
		else if (abstraction == "parent") {
			return parent_abstractions;
		}
		throw runtime_error("No such abstraction found");
	}

	static shared_ptr<EntityStorage> StorageManager::getEntityStorage() {
		return entity_storage;
	}

private:
	// parse trees (for querying patterns)

	// entities
	//static inline shared_ptr<EntityStorage> entity_storage;

	// abstractions
	//static inline shared_ptr<UsesAbstractionStorage> uses_abstractions;
	//static inline shared_ptr<ModifiesAbstractionStorage> modifies_abstractions;
	//static inline shared_ptr<FollowsAbstractionStorage> follows_abstractions;
	//static inline shared_ptr<ParentAbstractionStorage> parent_abstractions;
};
#endif