#ifndef DATAACCESSLAYER_H
#define DATAACCESSLAYER_H

#include <vector>
#include <string>
#include <unordered_map>
#include "../PKB.h"
#include "../Constants/QPSPKB.h"
#include "QueryResultsTable.h"
#include "QueryObjects/ClauseArg.h"

using namespace std;

class QueryObject;
/**
* This class represents an interface object that calls APIs from the PKB
*/
class DataAccessLayer {

public:
	/**
	 * Constructor for the DataAccessLayer object
	 */
	DataAccessLayer() {};

	/**
	 * Destructor for the DataAccessLayer object
	 */
	~DataAccessLayer() {};

	/* For Design Entities */
	vector<string> getEntity(ENTITY type);
	vector<string> getAllProcedures();
	vector<string> getAllVariables();
	vector<string> getAllConstants();

	/* For Patterns (Milestone 1) */
	map<string, vector<string>> getVariableMap();
	map<string, vector<string>> getConstantMap();

	/* For Design Abstractions / Clauses */
	map<string, vector<string>> DataAccessLayer::getClause(ABSTRACTION abstraction);
};
#endif
