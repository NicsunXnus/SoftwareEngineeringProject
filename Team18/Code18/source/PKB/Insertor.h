#pragma once
#include "StorageManager.h"

/**
* This class represents Insertor for the PKB.
*/
class Insertor {
public:

	// parse trees (for querying patterns)

	void Insertor::addEntityStatements(shared_ptr<EntityMapArg> statements) {
		shared_ptr<EntityStorage> entity_storage = StorageManager::getEntityStorage();
		(*entity_storage).setStatementDatabase(statements);
	}

	void Insertor::addProcedures(shared_ptr<EntityMapArg> procedures) {
		shared_ptr<EntityStorage> entity_storage = StorageManager::getEntityStorage();
		(*entity_storage).setProcedureDatabase(procedures);
	}

	void Insertor::addVariables(shared_ptr<EntityMapArg> variables) {
		//cout << "Insertor::addVariables 1" << endl;
		shared_ptr<EntityStorage> entity_storage = StorageManager::getEntityStorage();
		//cout << "Insertor::addVariables 2" << endl;
		(*entity_storage).setVariableDatabase(variables);
		//cout << "Insertor::addVariables 3" << endl;
	}

	void Insertor::addConstants(shared_ptr<EntityMapArg> constants) {
		shared_ptr<EntityStorage> entity_storage = StorageManager::getEntityStorage();
		(*entity_storage).setConstantDatabase(constants);
	}

	// abstractions include "uses", "follows" etc.
	// keys are the first argument for abstractions. include variable names or line numbers.
	void Insertor::addAbstraction(string abstraction, string key, vector<string> lines) {
		shared_ptr<AbstractionStorage> abstraction_storage = StorageManager::getAbstractionStorage(abstraction);
		(*abstraction_storage).insertAbstraction(key, lines);
	}
};