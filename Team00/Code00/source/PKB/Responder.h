#include <vector>
#include <variant>
#include <string>

#include "StorageManager.h"

using namespace std;

/**
* This class represents Responder for the PKB.
*/
class Responder {
public:
	// methods to respond to queries. generally, call the storage manager to do work (static)
	// storage manager will get entity storage/abstraction storage, perform the query on that class via polymorphism, then return line numbers
	// responder returns line numbers to caller (QPS)
	
	vector<int> Responder::getEntity(string entity)	{
		// check which database... if "variable/procedure/constant"
		// else use statement_database
		EntityStorage* entity_storage = StorageManager::getEntityStorage();
		return (*(entity_storage->getStatementDatabase())).at(entity);
	}

	map<string, vector<int>>* Responder::getAllProcedures()	{
		EntityStorage* entity_storage = StorageManager::getEntityStorage();
		return entity_storage->getProcedureDatabase();
	}

	vector<int> Responder::getProcedure(string procedure) {
		EntityStorage* entity_storage = StorageManager::getEntityStorage();
		return (*(entity_storage->getProcedureDatabase())).at(procedure);
	}

	map<string, vector<int>>* Responder::getAllVariables() {
		EntityStorage* entity_storage = StorageManager::getEntityStorage();
		return entity_storage->getVariableDatabase();
	}

	vector<int> Responder::getVariable(string variable)	{
		EntityStorage* entity_storage = StorageManager::getEntityStorage();
		return (*(entity_storage->getVariableDatabase())).at(variable);
	}

	map<string, vector<int>>* Responder::getAllConstants() {
		EntityStorage* entity_storage = StorageManager::getEntityStorage();
		return entity_storage->getConstantDatabase();
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
};