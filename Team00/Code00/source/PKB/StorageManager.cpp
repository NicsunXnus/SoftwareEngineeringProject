#include <string>
#include "StorageManager.h"

using namespace std;

TNode* StorageManager::getAST()
{
	return nullptr;
}

AbstractionStorage* StorageManager::getAbstraction(const string abstraction)
{
	if (abstraction == "uses") {
		return &uses_abstractions;
	}
	return nullptr;
}

EntityStorage* StorageManager::getEntities()
{
	return &entities;
}
