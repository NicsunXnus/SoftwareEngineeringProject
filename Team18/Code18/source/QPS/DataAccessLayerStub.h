#ifndef DATAACCESSLAYERSTUB_H
#define DATAACCESSLAYERSTUB_H

#include <vector>
#include <string>
#include <unordered_map>
#include "../PKB.h"
#include "../Constants/QPSPKB.h"
#include "DataAccessLayer.h"
#include "../QPS/QueryResultsTable.h"
#include "../QPS/QueryObjects/ClauseArg.h"

using namespace std;

class QueryObject;
/**
* This class represents an interface object that calls APIs from the PKB
*/
class DataAccessLayerStub : public DataAccessLayer {
private:
	vector<string> entities;
	vector<string> procedures;
	vector<string> variables;
	vector<string> constants;
	map<string, vector<string>> clauses;

public:
	/**
	 * Constructor for the DataAccessLayer object
	 */
	DataAccessLayerStub() {};

	/**
	 * Destructor for the DataAccessLayer object
	 */
	~DataAccessLayerStub() {};

	/* For Design Entities */
	vector<string> getEntity(ENTITY type);
	vector<string> getAllProcedures();
	vector<string> getAllVariables();
	vector<string> getAllConstants();

	///* For Design Entities */
	void insertEntity(vector<string> data);
	void insertProcedures(vector<string> data);
	void insertVariables(vector<string> data);
	void insertConstants(vector<string> data);
	void insertClauses(map<string, vector<string>> clause);

	/* For Design Abstractions / Clauses */
	map<string, vector<string>> DataAccessLayerStub::getClause(ABSTRACTION abstraction);


};
#endif
