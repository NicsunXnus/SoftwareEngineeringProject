#include "StorageManager.h"

TNode* StorageManager::getAST()
{
	return nullptr;
}

AbstractionStorage StorageManager::getAbstractions()
{
	return abstractions;
}

EntityStorage StorageManager::getEntities()
{
	return entities;
}
