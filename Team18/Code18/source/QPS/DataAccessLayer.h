#ifndef DATAACCESSLAYER_H
#define DATAACCESSLAYER_H

#include <vector>
#include<unordered_set>
#include <string>
#include <unordered_map>
#include "../PKB.h"
#include "../Constants/DesignEnums.h"
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
	virtual unordered_set<string> getEntity(ENTITY type);
	virtual unordered_set<string> getAllProcedures();
	virtual unordered_set<string> getAllVariables();
	virtual unordered_set<string> getAllConstants();

	/* For Patterns (Milestone 1) */
	virtual map<string, unordered_set<string>> getVariableMap();
	virtual map<string, unordered_set<string>> getConstantMap();

	/* For Design Abstractions / Clauses */
	virtual StringMap DataAccessLayer::getClause(ABSTRACTION abstraction);
	virtual StringMap DataAccessLayer::getClauseInverse(ABSTRACTION abstraction);

	/* Attributes */
	virtual StringMap DataAccessLayer::getCallProcNames();
	virtual StringMap DataAccessLayer::getReadVarNames();
	virtual StringMap DataAccessLayer::getPrintVarNames();
};
#endif
