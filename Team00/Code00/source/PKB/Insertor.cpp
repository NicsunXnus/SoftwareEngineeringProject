#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>

#include "Insertor.h"
#include "StorageManager.h"
#include "../TNode.h"  // replace with ASTNode.h

using namespace std; 

void Insertor::setAST(TNode* r) {
	// set r as the root of AST by calling insertor to do work (set pointer in StorageManager)
}

void Insertor::addEntityStatement(string entity, vector<int> lines) {
	EntityStorage* entity_storage = StorageManager::getEntityStorage();
	(*entity_storage).insertEntity(entity, lines);
}

void Insertor::addAbstraction(string abstraction, KeyValue key, vector<int> lines) {
	AbstractionStorage* abstraction_storage = StorageManager::getAbstractionStorage(abstraction);
	(*abstraction_storage).insertAbstraction(key, lines);
}
