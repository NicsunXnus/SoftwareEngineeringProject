#pragma once
#include "StorageManager.h"

/**
* This class represents Insertor for the PKB.
*/
class Insertor {
public:

	// parse trees (for querying patterns)

	void Insertor::addEntity(shared_ptr<StringMap> entity_map, ENTITY entity_type = STMT) {
		shared_ptr<EntityStorage> entity_storage = StorageManager::getEntityStorage();

		switch (entity_type) {
			case VARIABLE:
				(*entity_storage).setVariableDatabase(entity_map);
				break;
			case CONSTANT:
				(*entity_storage).setConstantDatabase(entity_map);
				break;
			case PROCEDURE:
				(*entity_storage).setProcedureDatabase(entity_map);
				break;
			case STMT:  // other entities are statement types
				(*entity_storage).setStatementDatabase(entity_map);
				break;
			default:
				cerr << "Insertor::addEntity: Invalid entity type" << endl;
				break;
		}
	}

	// keys are the first argument for abstractions. include variable names or line numbers.
	void Insertor::addAbstraction(shared_ptr<StringMap> abstraction_map, ABSTRACTION abstraction_type) {
		shared_ptr<AbstractionStorage> abstraction_storage = StorageManager::getAbstractionStorage(abstraction_type);
		(*abstraction_storage).setAbstraction(abstraction_map);
	}
};