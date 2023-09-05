#include <string>
#include "StorageManager.h"

using namespace std;

TNode* StorageManager::getAST() {
	return nullptr;
}

AbstractionStorage* StorageManager::getAbstractionStorage(const string abstraction) {
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
	return nullptr;
}

EntityStorage* StorageManager::getEntityStorage() {
	return &entity_storage;
}
