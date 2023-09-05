#include "Responder.h"

vector<int> Responder::getEntity(string entity)
{
	// check which database... if "variable/procedure/constant"
	// else use statement_database
	EntityStorage* entity_storage = StorageManager::getEntityStorage();
	return (*(entity_storage->getStatementDatabase())).at(entity);
}

map<string, vector<int>>* Responder::getProcedures()
{
	EntityStorage* entity_storage = StorageManager::getEntityStorage();
	return entity_storage->getProcedureDatabase();
}

vector<int> Responder::getProcedure(string procedure)
{
	EntityStorage* entity_storage = StorageManager::getEntityStorage();
	return (*(entity_storage->getProcedureDatabase())).at(procedure);
}

map<string, vector<int>>* Responder::getVariables()
{
	EntityStorage* entity_storage = StorageManager::getEntityStorage();
	return entity_storage->getVariableDatabase();
}

vector<int> Responder::getVariable(string variable)
{
	EntityStorage* entity_storage = StorageManager::getEntityStorage();
	return (*(entity_storage->getVariableDatabase())).at(variable);
}

map<string, vector<int>>* Responder::getConstants()
{
	EntityStorage* entity_storage = StorageManager::getEntityStorage();
	return entity_storage->getConstantDatabase();
}

vector<int> Responder::getConstant(string constant)
{
	EntityStorage* entity_storage = StorageManager::getEntityStorage();
	return (*(entity_storage->getConstantDatabase())).at(constant);
}

map<KeyValue, vector<int>> Responder::getAbstraction(string abstraction)
{
	AbstractionStorage* abstraction_storage = StorageManager::getAbstractionStorage(abstraction);
	return *(abstraction_storage->getDatabase());
}

vector<int> Responder::getAbstraction(string abstraction, string variable)
{
	AbstractionStorage* abstraction_storage = StorageManager::getAbstractionStorage(abstraction);
	return (*(abstraction_storage->getDatabase())).at(variable);
}
