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

// parse trees (for querying patterns)

// entities
static EntityStorage entity_storage;// = new EntityStorage();

// abstractions
static UsesAbstractionStorage uses_abstractions;
static ModifiesAbstractionStorage modifies_abstractions;
static FollowsAbstractionStorage follows_abstractions;
static ParentAbstractionStorage parent_abstractions;


/**
* This class represents Storage for the PKB. Consists of the storages and AST
*/
class StorageManager {
public:

	static AbstractionStorage* StorageManager::getAbstractionStorage(const string abstraction) {
		if (abstraction == "uses") {
			return &uses_abstractions;
		}
		else if (abstraction == "modifies") {
			return &modifies_abstractions;
		}
		else if (abstraction == "follows") {
			return &follows_abstractions;
		}
		else if (abstraction == "parent") {
			return &parent_abstractions;
		}
		throw runtime_error("No such abstraction found");
	}

	static EntityStorage* StorageManager::getEntityStorage() {
		return &entity_storage;
	}

private:
};
#endif