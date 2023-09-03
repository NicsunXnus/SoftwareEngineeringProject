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

void Insertor::AddEntityToPKB(string entity, vector<int> lines) {
	EntityStorage* entity_storage = StorageManager::getEntities();
	(*entity_storage).insertEntity(entity, lines);
}

void Insertor::AddAbstractionToPKB(string abstraction, KeyValue key, vector<int> lines) {
	AbstractionStorage* abstraction_storage = StorageManager::getAbstraction(abstraction);
	(*abstraction_storage).insertAbstraction(key, lines);
}
