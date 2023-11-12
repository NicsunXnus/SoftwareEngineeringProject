#ifndef DATAACCESSLAYER_H
#define DATAACCESSLAYER_H

#include <vector>
#include<unordered_set>
#include <string>
#include <unordered_map>
#include "../PKB/PKB.h"
#include "../Constants/DesignEnums.h"
#include "QueryResultsTable.h"
#include "QueryObjects/ClauseArg.h"

using namespace std;

class QueryObject;
class DataAccessLayer;

/**
* This class represents an interface object that calls APIs from the PKB
*/
class DataAccessLayer : public enable_shared_from_this<DataAccessLayer> {

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

	/* For Patterns */
	virtual unordered_map<string, unordered_set<string>> getVariableMap();
	virtual unordered_map<string, unordered_set<string>> getConstantMap();
    virtual shared_ptr<Node> getPatternTree(string statement_number);

	/* For Design Abstractions / Clauses */
	virtual StringMap getClause(ABSTRACTION abstraction);
	virtual StringMap getClauseInverse(ABSTRACTION abstraction);

	/* Attributes */
	virtual StringMap getCallProcNames();
	virtual StringMap getReadVarNames();
	virtual StringMap getPrintVarNames();

	/* Misc */
	virtual pair<string, string> getProcLines(string procName);
};
#endif
