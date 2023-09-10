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
	
	vector<int> Responder::getEntityStatement(ENTITY entity) const {
		EntityStorage* entity_storage = StorageManager::getEntityStorage();
		return (*(entity_storage->getStatementDatabase())).at(entity);
	}

	vector<string> Responder::getAllProcedures() const {
		EntityStorage* entity_storage = StorageManager::getEntityStorage();
		map<string, vector<int>>* proc_database = entity_storage->getProcedureDatabase();
		return getKeys(proc_database);
	}

	vector<int> Responder::getProcedure(string procedure) const {
		EntityStorage* entity_storage = StorageManager::getEntityStorage();
		return (*(entity_storage->getProcedureDatabase())).at(procedure);
	}

	vector<string> Responder::getAllVariables() const {
		//std::cout << "Responder.h:getAllVariables 1" << std::endl;
		EntityStorage* entity_storage = StorageManager::getEntityStorage();
		//std::cout << "Responder.h:getAllVariables 2" << std::endl;
		map<string, vector<int>>* var_database = entity_storage->getVariableDatabase();
		//cout << "Responder.::getAllVariables addr:" << var_database << endl;
		//std::cout << "Responder.h:getAllVariables 3" << std::endl;
		//for (auto [a, b] : *var_database) {
		//	std::cout << a << endl;
		//	for (auto c : b) {
		//		std::cout << c << endl;
		//	}
		//}
		//std::cout << "Responder.cpp:getAllVariables 4" << std::endl;
		return getKeys(var_database);
	}

	vector<int> Responder::getVariable(string variable)	const {
		EntityStorage* entity_storage = StorageManager::getEntityStorage();
		return (*(entity_storage->getVariableDatabase())).at(variable);
	}

	vector<string> Responder::getAllConstants() const {
		EntityStorage* entity_storage = StorageManager::getEntityStorage();
		map<string, vector<int>>* const_database = entity_storage->getConstantDatabase();
		return getKeys(const_database);
	}

	vector<int> Responder::getConstant(string constant)	const {
		EntityStorage* entity_storage = StorageManager::getEntityStorage();
		return (*(entity_storage->getConstantDatabase())).at(constant);
	}

	map<KeyValue, vector<int>> Responder::getAbstraction(string abstraction) const {
		AbstractionStorage* abstraction_storage = StorageManager::getAbstractionStorage(abstraction);
		return *(abstraction_storage->getDatabase());
	}

	vector<int> Responder::getAbstractionVariable(string abstraction, string variable) const {
		AbstractionStorage* abstraction_storage = StorageManager::getAbstractionStorage(abstraction);
		return (*(abstraction_storage->getDatabase())).at(variable);
	}

private:
	vector<string> getKeys(map<string, vector<int>>* db) const {
		vector<string> keys;
		for (const auto& [k, v] : *db) {
			keys.push_back(k);
		}
		return keys;
	}
};