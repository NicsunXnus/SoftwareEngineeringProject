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

	// can overload methods with other conditions (beyond Sprint 1)
	
	static vector<int> getEntity(string entity);

	map<string, vector<int>>* getProcedures();
	vector<int> getProcedure(string procedure);

	map<string, vector<int>>* getVariables();
	vector<int> getVariable(string variable);

	map<string, vector<int>>* getConstants();
	vector<int> getConstant(string constant);

	// beyond Sprint 1: methods for getting abstractions
	static map<KeyValue, vector<int>> getAbstraction(string abstraction);
	static vector<int> getAbstraction(string abstraction, string variable);
	
};