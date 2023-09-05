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

class TNode;  // change to AST Node

/**
* This class represents Storage for the PKB. Consists of the storages and AST
*/
class StorageManager {
public:
	static TNode* getAST();  // to be removed
	static AbstractionStorage* getAbstractionStorage(string);
	static EntityStorage* getEntityStorage();

private:
	static TNode* AST;  // to be removed

	// parse trees (for querying patterns)
	
	// variables, procedures, constants

	// entities
	static EntityStorage entity_storage;

	// abstractions
	static UsesAbstractionStorage uses_abstractions;
	static ModifiesAbstractionStorage modifies_abstractions;
	static FollowsAbstractionStorage follows_abstractions;
	static ParentAbstractionStorage parent_abstractions;
};
#endif