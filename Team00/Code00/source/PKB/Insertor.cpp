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

void Insertor::addEntityStatement(string statement, vector<int> lines) {
	EntityStorage* entity_storage = StorageManager::getEntityStorage();
	(*entity_storage).insertEntityStatement(statement, lines);
}

void Insertor::addProcedure(string procedure, vector<int> lines) {
	EntityStorage* entity_storage = StorageManager::getEntityStorage();
	(*entity_storage).insertProcedure(procedure, lines);
}

void Insertor::addVariable(string variable, vector<int> lines) {
	EntityStorage* entity_storage = StorageManager::getEntityStorage();
	(*entity_storage).insertVariable(variable, lines);
}

void Insertor::addConstant(string constant, vector<int> lines) {
	EntityStorage* entity_storage = StorageManager::getEntityStorage();
	(*entity_storage).insertConstant(constant, lines);
}

void Insertor::addAbstraction(string abstraction, KeyValue key, vector<int> lines) {
	AbstractionStorage* abstraction_storage = StorageManager::getAbstractionStorage(abstraction);
	(*abstraction_storage).insertAbstraction(key, lines);
}
