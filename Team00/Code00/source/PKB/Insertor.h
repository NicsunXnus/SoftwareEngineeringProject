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

	// abstractions include "uses", "follows" etc.
	// keys are the first argument for abstractions. include variable names or line numbers.
	void Insertor::addAbstraction(string abstraction, KeyValue key, vector<int> lines) {
		AbstractionStorage* abstraction_storage = StorageManager::getAbstractionStorage(abstraction);
		(*abstraction_storage).insertAbstraction(key, lines);
	}
};