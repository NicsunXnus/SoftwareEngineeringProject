#pragma once
#include "StorageManager.h"

/**
* This class represents Insertor for the PKB.
*/
class Insertor {
public:

	// parse trees (for querying patterns)

	void Insertor::setAST(TNode* r) {
		// set r as the root of AST by calling insertor to do work (set pointer in StorageManager)
	}

	void Insertor::addEntityStatements(map<string, vector<int>>* statements) {
		EntityStorage* entity_storage = StorageManager::getEntityStorage();
		(*entity_storage).setStatementDatabase(statements);
	}

	void Insertor::addProcedures(map<string, vector<int>>* procedures) {
		EntityStorage* entity_storage = StorageManager::getEntityStorage();
		(*entity_storage).setProcedureDatabase(procedures);
	}

	void Insertor::addVariables(map<string, vector<int>>* variables) {
		EntityStorage* entity_storage = StorageManager::getEntityStorage();
		(*entity_storage).setVariableDatabase(variables);
	}

	void Insertor::addConstants(map<string, vector<int>>* constants) {
		EntityStorage* entity_storage = StorageManager::getEntityStorage();
		(*entity_storage).setConstantDatabase(constants);
	}

	// abstractions include "uses", "follows" etc.
	// keys are the first argument for abstractions. include variable names or line numbers.
	void Insertor::addAbstraction(string abstraction, KeyValue key, vector<int> lines) {
		AbstractionStorage* abstraction_storage = StorageManager::getAbstractionStorage(abstraction);
		(*abstraction_storage).insertAbstraction(key, lines);
	}
};