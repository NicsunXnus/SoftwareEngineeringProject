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
	// storage manager will get entity storage/abstraction storage, perform the query on that class via polymorphism, then return line numbers
	// responder returns line numbers to caller (QPS)
	
	vector<int> Responder::getEntityStatement(ENTITY entity) {
		EntityStorage* entity_storage = StorageManager::getEntityStorage();
		return (*(entity_storage->getStatementDatabase())).at(entity);
	}

	vector<string> Responder::getAllProcedures() {
		EntityStorage* entity_storage = StorageManager::getEntityStorage();
		map<string, vector<int>>* proc_database = entity_storage->getProcedureDatabase();
		return getKeys(proc_database);
	}

	vector<int> Responder::getProcedure(string procedure) {
		EntityStorage* entity_storage = StorageManager::getEntityStorage();
		return (*(entity_storage->getProcedureDatabase())).at(procedure);
	}

	vector<string> Responder::getAllVariables() {
		EntityStorage* entity_storage = StorageManager::getEntityStorage();

		std::cout << "In Responder, entity_storage address: " << entity_storage << '\n';

		map<string, vector<int>>* var_database = entity_storage->getVariableDatabase();
		return getKeys(var_database);
	}

	vector<int> Responder::getVariable(string variable)	{
		EntityStorage* entity_storage = StorageManager::getEntityStorage();
		return (*(entity_storage->getVariableDatabase())).at(variable);
	}

	vector<string> Responder::getAllConstants() {
		EntityStorage* entity_storage = StorageManager::getEntityStorage();
		map<string, vector<int>>* const_database = entity_storage->getConstantDatabase();
		return getKeys(const_database);
	}

	vector<int> Responder::getConstant(string constant)	{
		EntityStorage* entity_storage = StorageManager::getEntityStorage();
		return (*(entity_storage->getConstantDatabase())).at(constant);
	}

	map<KeyValue, vector<int>> Responder::getAbstraction(string abstraction) {
		AbstractionStorage* abstraction_storage = StorageManager::getAbstractionStorage(abstraction);
		return *(abstraction_storage->getDatabase());
	}

	vector<int> Responder::getAbstractionVariable(string abstraction, string variable) {
		AbstractionStorage* abstraction_storage = StorageManager::getAbstractionStorage(abstraction);
		return (*(abstraction_storage->getDatabase())).at(variable);
	}

private:
	vector<string> getKeys(map<string, vector<int>>* db) {
		vector<string> keys;

		std::cout << "In Responder getKeys, db size: " << db->size() << '\n';

		for (const auto& [k, v] : *db) {

			keys.push_back(k);
		}
		return keys;
	}
};