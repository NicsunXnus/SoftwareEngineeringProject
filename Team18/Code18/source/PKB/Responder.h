#pragma once
#include <vector>
#include <variant>
#include <string>

#include "StorageManager.h"
#include "../Constants/DesignEnums.h"

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
		shared_ptr<map<ENTITY, vector<string>>> entity_database = entity_storage->getStatementDatabase();
		// check if entity exists
		if (entity_database->find(entity) == entity_database->end()) {
			return vector<string>();
		}
		return (*entity_database).at(entity);
	}

	vector<string> Responder::getAllProcedures() const {
		shared_ptr<EntityStorage> entity_storage = StorageManager::getEntityStorage();
		shared_ptr<StringMap> proc_database = entity_storage->getProcedureDatabase();
		return getKeys(proc_database);
	}

	// unused now
	vector<string> Responder::getProcedure(string procedure) const {
		shared_ptr<EntityStorage> entity_storage = StorageManager::getEntityStorage();
		shared_ptr<StringMap> proc_database = entity_storage->getProcedureDatabase();
		// check if procedure exists
		if (proc_database->find(procedure) == proc_database->end()) {
			return vector<string>();
		}
		return (*proc_database).at(procedure);
	}

	map<string, vector<string>> Responder::getVariableMap() const {
		shared_ptr<EntityStorage> entity_storage = StorageManager::getEntityStorage();
		shared_ptr<map<string, vector<string>>> var_database = entity_storage->getVariableDatabase();
		return *var_database;
	}
	
	vector<string> Responder::getAllVariables() const {
		shared_ptr<EntityStorage> entity_storage = StorageManager::getEntityStorage();
		shared_ptr<StringMap> var_database = entity_storage->getVariableDatabase();
		return getKeys(var_database);
	}

	// unused now
	vector<string> Responder::getVariable(string variable) const {
		shared_ptr<EntityStorage> entity_storage = StorageManager::getEntityStorage();
		shared_ptr<StringMap> var_database = entity_storage->getVariableDatabase();
		// check if variable exists
		if (var_database->find(variable) == var_database->end()) {
			return vector<string>();
		}
		return (*var_database).at(variable);
	}

	map<string, vector<string>> Responder::getConstantMap() const {
		shared_ptr<EntityStorage> entity_storage = StorageManager::getEntityStorage();
		shared_ptr<map<string, vector<string>>> const_database = entity_storage->getConstantDatabase();
		return *const_database;
	}

	vector<string> Responder::getAllConstants() const {
		shared_ptr<EntityStorage> entity_storage = StorageManager::getEntityStorage();
		shared_ptr<StringMap> const_database = entity_storage->getConstantDatabase();
		return getKeys(const_database);
	}

	// unused now
	vector<string> Responder::getConstant(string constant) const {
		shared_ptr<EntityStorage> entity_storage = StorageManager::getEntityStorage();
		shared_ptr<StringMap> constant_database = entity_storage->getConstantDatabase();
		// check if constant exists
		if (constant_database->find(constant) == constant_database->end()) {
			return vector<string>();
		}
		return (*constant_database).at(constant);
	}

	map<string, vector<string>> Responder::getAbstraction(ABSTRACTION abstraction) const {
		shared_ptr<AbstractionStorage> abstraction_storage = StorageManager::getAbstractionStorage(abstraction);
		// note: for Follows* and Parent*, we return the whole database.
		// for Follows and Parent, we return a truncated database with the value just the direct follower/parent.
		if (abstraction == FOLLOWS || abstraction == PARENT || abstraction == CALLS) {
			return *(abstraction_storage->getTruncatedDatabase());
		}
		return *(abstraction_storage->getDatabase());
	}

	// unused now
	vector<string> Responder::getAbstractionVariable(ABSTRACTION abstraction, string key) const {
		shared_ptr<AbstractionStorage> abstraction_storage = StorageManager::getAbstractionStorage(abstraction);
		shared_ptr<StringMap> abstraction_database = abstraction_storage->getDatabase();
		// check if key exists
		if (abstraction_database->find(key) == abstraction_database->end()) {
			return vector<string>();
		}
		return (*abstraction_database).at(key);
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