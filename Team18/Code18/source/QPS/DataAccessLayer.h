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
	virtual vector<string> getEntity(ENTITY type);
	virtual vector<string> getAllProcedures();
	virtual vector<string> getAllVariables();
	virtual vector<string> getAllConstants();

	/* For Patterns (Milestone 1) */
	virtual map<string, vector<string>> getVariableMap();
	virtual map<string, vector<string>> getConstantMap();

	/* For Design Abstractions / Clauses */
	virtual map<string, vector<string>> DataAccessLayer::getClause(ABSTRACTION abstraction);
};
#endif
