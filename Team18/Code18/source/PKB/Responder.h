#pragma once
#include <vector>
#include <variant>
#include <string>
#include <unordered_set>

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
	
	unordered_set<string> Responder::getEntityStatement(ENTITY entity) const {
		shared_ptr<EntityStorage> entity_storage = StorageManager::getEntityStorage();
		shared_ptr<map<ENTITY, unordered_set<string>>> entity_database = entity_storage->getStatementDatabase();
		// check if entity exists
		if (entity_database->find(entity) == entity_database->end()) {
			return unordered_set<string>();
		}
		return (*entity_database).at(entity);
	}

	unordered_set<string> Responder::getAllProcedures() const {
		shared_ptr<EntityStorage> entity_storage = StorageManager::getEntityStorage();
		shared_ptr<StringMap> proc_database = entity_storage->getProcedureDatabase();
		return getKeys(proc_database);
	}

	// unused now
	unordered_set<string> Responder::getProcedure(string procedure) const {
	 	shared_ptr<EntityStorage> entity_storage = StorageManager::getEntityStorage();
		shared_ptr<StringMap> proc_database = entity_storage->getProcedureDatabase();
		// check if procedure exists
	 	if (proc_database->find(procedure) == proc_database->end()) {
	 		return unordered_set<string>();
	 	}
	 	return (*proc_database).at(procedure);
	}

	StringMap Responder::getVariableMap() const {
		shared_ptr<EntityStorage> entity_storage = StorageManager::getEntityStorage();
		shared_ptr<StringMap> var_database = entity_storage->getVariableDatabase();
		return *var_database;
	}
	
	unordered_set<string> Responder::getAllVariables() const {
		shared_ptr<EntityStorage> entity_storage = StorageManager::getEntityStorage();
		shared_ptr<StringMap> var_database = entity_storage->getVariableDatabase();
		return getKeys(var_database);
	}

	StringMap Responder::getConstantMap() const {
		shared_ptr<EntityStorage> entity_storage = StorageManager::getEntityStorage();
		shared_ptr<StringMap> const_database = entity_storage->getConstantDatabase();
		return *const_database;
	}

	unordered_set<string> Responder::getAllConstants() const {
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

	map<string, shared_ptr<Node>> Responder::getAllPatterns() const {
		shared_ptr<EntityStorage> entity_storage = StorageManager::getEntityStorage();
		shared_ptr<map<string, shared_ptr<Node>>> pattern_database = entity_storage->getPatternDatabase();
		return *pattern_database;
	}

	shared_ptr<Node> Responder::getPattern(string statement_number) const {
		shared_ptr<EntityStorage> entity_storage = StorageManager::getEntityStorage();
		shared_ptr<map<string, shared_ptr<Node>>> pattern_database = entity_storage->getPatternDatabase();
		// check if pattern exists
		if (pattern_database->find(statement_number) == pattern_database->end()) {
			return nullptr;
		}
		return (*pattern_database).at(statement_number);
	}

	StringMap Responder::getAbstraction(ABSTRACTION abstraction, bool inverse = false) const {
		shared_ptr<AbstractionStorage> abstraction_storage = StorageManager::getAbstractionStorage(abstraction);
		if (inverse ^ isFlippedStorage(abstraction)) {
			return createInverseMap(abstraction_storage->getDatabase());
		}
		return *(abstraction_storage->getDatabase());
	}

	// unused now
	unordered_set<string> Responder::getAbstractionVariable(ABSTRACTION abstraction, string key) const {
		shared_ptr<AbstractionStorage> abstraction_storage = StorageManager::getAbstractionStorage(abstraction);
		shared_ptr<StringMap> abstraction_database = abstraction_storage->getDatabase();
		if (abstraction_database->find(key) == abstraction_database->end()) {
			return unordered_set<string>();
		}
		return (*abstraction_database).at(key);
	}

private:
	unordered_set<string> getKeys(shared_ptr<StringMap> db) const {
		unordered_set<string> keys;
		for (const auto& [k, v] : *db) {
			keys.insert(k);
		}
		return keys;
	}

	StringMap createInverseMap(shared_ptr<StringMap> originalMap) const {
		StringMap inverseMap;
		for (const auto& pair : *originalMap) {
			const string& key = pair.first;
			const unordered_set<string>& values = pair.second;
			for (const string& value : values) {
				inverseMap[value].insert(key);
			}
		}
		return inverseMap;
	}
};