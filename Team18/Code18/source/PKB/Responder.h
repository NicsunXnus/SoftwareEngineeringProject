#pragma once
#include <vector>
#include <variant>
#include <string>

#include "StorageManager.h"
#include "../Constants/QPSPKB.h"
#include "../DesignExtractor/Entity.h"

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
	// vector<string> Responder::getProcedure(string procedure) const {
	// 	shared_ptr<EntityStorage> entity_storage = StorageManager::getEntityStorage();
	//	shared_ptr<StringMap> proc_database = entity_storage->getProcedureDatabase();
		// check if procedure exists
	// 	if (proc_database->find(procedure) == proc_database->end()) {
	// 		return vector<string>();
	// 	}
	// 	return (*proc_database).at(procedure);
	// }

	StringMap Responder::getVariableMap() const {
		shared_ptr<EntityStorage> entity_storage = StorageManager::getEntityStorage();
		shared_ptr<StringMap> var_database = entity_storage->getVariableDatabase();
		return *var_database;
	}
	
	vector<string> Responder::getAllVariables() const {
		shared_ptr<EntityStorage> entity_storage = StorageManager::getEntityStorage();
		shared_ptr<StringMap> var_database = entity_storage->getVariableDatabase();
		return getKeys(var_database);
	}

	StringMap Responder::getConstantMap() const {
		shared_ptr<EntityStorage> entity_storage = StorageManager::getEntityStorage();
		shared_ptr<StringMap> const_database = entity_storage->getConstantDatabase();
		return *const_database;
	}

	vector<string> Responder::getAllConstants() const {
		shared_ptr<EntityStorage> entity_storage = StorageManager::getEntityStorage();
		shared_ptr<StringMap> const_database = entity_storage->getConstantDatabase();
		return getKeys(const_database);
	}

	StringMap Responder::getCallProcNameMap() const {
		shared_ptr<EntityStorage> entity_storage = StorageManager::getEntityStorage();
		shared_ptr<StringMap> call_procname_database = entity_storage->getCallProcnameDatabase();
		return *call_procname_database;
	}

	StringMap Responder::getReadVarNameMap() const {
		shared_ptr<EntityStorage> entity_storage = StorageManager::getEntityStorage();
		shared_ptr<StringMap> read_varname_database = entity_storage->getReadVarnameDatabase();
		return *read_varname_database;
	}

	StringMap Responder::getPrintVarNameMap() const {
		shared_ptr<EntityStorage> entity_storage = StorageManager::getEntityStorage();
		shared_ptr<StringMap> print_varname_database = entity_storage->getPrintVarnameDatabase();
		return *print_varname_database;
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
		shared_ptr<StringMap> abstraction_database = abstraction_storage->getDatabase();
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