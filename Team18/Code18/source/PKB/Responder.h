#pragma once
#include <vector>
#include <variant>
#include <string>

#include "StorageManager.h"
#include "../Constants/QPSPKB.h"

using namespace std;

/**
* This class represents Responder for the PKB.
*/
class Responder {
public:
	// methods to respond to queries. generally, call the storage manager to do work.
	// storage manager will get entity storage/abstraction storage, perform the query on that class via polymorphism, then return line numbers.
	
	vector<string> Responder::getEntityStatement(ENTITY entity) const {
		shared_ptr<EntityStorage> entity_storage = StorageManager::getEntityStorage();
		return (*(entity_storage->getStatementDatabase())).at(entity);
	}

	vector<string> Responder::getAllProcedures() const {
		shared_ptr<EntityStorage> entity_storage = StorageManager::getEntityStorage();
		shared_ptr<StringMap> proc_database = entity_storage->getProcedureDatabase();
		return getKeys(proc_database);
	}

	// unused now
	vector<string> Responder::getProcedure(string procedure) const {
		shared_ptr<EntityStorage> entity_storage = StorageManager::getEntityStorage();
		return (*(entity_storage)->getProcedureDatabase()).at(procedure);
	}

	vector<string> Responder::getAllVariables() const {
		shared_ptr<EntityStorage> entity_storage = StorageManager::getEntityStorage();
		shared_ptr<StringMap> var_database = entity_storage->getVariableDatabase();
		return getKeys(var_database);
	}

	// unused now
	vector<string> Responder::getVariable(string variable) const {
		shared_ptr<EntityStorage> entity_storage = StorageManager::getEntityStorage();
		return (*(entity_storage)->getVariableDatabase()).at(variable);
	}

	vector<string> Responder::getAllConstants() const {
		shared_ptr<EntityStorage> entity_storage = StorageManager::getEntityStorage();
		shared_ptr<StringMap> const_database = entity_storage->getConstantDatabase();
		return getKeys(const_database);
	}

	// unused now
	vector<string> Responder::getConstant(string constant) const {
		shared_ptr<EntityStorage> entity_storage = StorageManager::getEntityStorage();
		return (*(entity_storage)->getConstantDatabase()).at(constant);
	}

	StringMap Responder::getAbstraction(ABSTRACTION abstraction) const {
		shared_ptr<AbstractionStorage> abstraction_storage = StorageManager::getAbstractionStorage(abstraction);
		// note: for Follows* and Parent*, we return the whole database.
		// for Follows and Parent, we return a truncated database with the value just the direct follower/parent.

		if (abstraction == FOLLOWS || abstraction == PARENT) {
			return *(abstraction_storage->getTruncatedDatabase());
		}
		return *(abstraction_storage->getDatabase());
	}

	// unused now
	vector<string> Responder::getAbstractionVariable(ABSTRACTION abstraction, string key) const {
		shared_ptr<AbstractionStorage> abstraction_storage = StorageManager::getAbstractionStorage(abstraction);
		return (*(abstraction_storage->getDatabase())).at(key);
	}

private:
	vector<string> getKeys(shared_ptr<StringMap> db) const {
		vector<string> keys;
		for (const auto& [k, v] : *db) {
			keys.push_back(k);
		}
		return keys;
	}
};