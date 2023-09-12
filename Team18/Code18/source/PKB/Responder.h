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
	// methods to respond to queries. generally, call the storage manager to do work (static)
	// storage manager will get entity storage/abstraction storage, perform the query on that class via polymorphism, then return line numbers.
	// responder returns line numbers to caller (QPS)
	
	vector<string> Responder::getEntityStatement(ENTITY entity) const {
		shared_ptr<EntityStorage> entity_storage = StorageManager::getEntityStorage();
		return (*(entity_storage->getStatementDatabase())).at(entity);
	}

	vector<string> Responder::getAllProcedures() const {
		shared_ptr<EntityStorage> entity_storage = StorageManager::getEntityStorage();
		EntityMapArg proc_database = *(entity_storage)->getProcedureDatabase();
		return getKeys(proc_database);
	}

	vector<string> Responder::getProcedure(string procedure) const {
		shared_ptr<EntityStorage> entity_storage = StorageManager::getEntityStorage();
		return (*(entity_storage)->getProcedureDatabase()).at(procedure);
	}

	vector<string> Responder::getAllVariables() const {
		shared_ptr<EntityStorage> entity_storage = StorageManager::getEntityStorage();
		EntityMapArg var_database = *(entity_storage)->getVariableDatabase();
		return getKeys(var_database);
	}

	vector<string> Responder::getVariable(string variable) const {
		shared_ptr<EntityStorage> entity_storage = StorageManager::getEntityStorage();
		return (*(entity_storage)->getVariableDatabase()).at(variable);
	}

	vector<string> Responder::getAllConstants() const {
		shared_ptr<EntityStorage> entity_storage = StorageManager::getEntityStorage();
		EntityMapArg const_database = *(entity_storage)->getConstantDatabase();
		return getKeys(const_database);
	}

	vector<string> Responder::getConstant(string constant) const {
		shared_ptr<EntityStorage> entity_storage = StorageManager::getEntityStorage();
		return (*(entity_storage)->getConstantDatabase()).at(constant);
	}

	map<string, vector<string>> Responder::getAbstraction(string abstraction) const {
		shared_ptr<AbstractionStorage> abstraction_storage = StorageManager::getAbstractionStorage(abstraction);
		return *(abstraction_storage->getDatabase());
	}

	vector<string> Responder::getAbstractionVariable(string abstraction, string variable) const {
		shared_ptr<AbstractionStorage> abstraction_storage = StorageManager::getAbstractionStorage(abstraction);
		return (*(abstraction_storage->getDatabase())).at(variable);
	}

private:
	vector<string> getKeys(EntityMapArg db) const {
		vector<string> keys;
		for (const auto& [k, v] : db) {
			keys.push_back(k);
		}
		return keys;
	}
};