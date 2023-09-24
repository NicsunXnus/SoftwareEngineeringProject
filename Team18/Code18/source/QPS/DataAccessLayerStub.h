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

public:
	/**
	 * Constructor for the DataAccessLayer object
	 */
	DataAccessLayerStub() {};

	/**
	 * Destructor for the DataAccessLayer object
	 */
	~DataAccessLayerStub() {};

	vector<string> getEntity(ENTITY type) override;

	vector<string> getAllProcedures() override;

	vector<string> getAllVariables() override;

	vector<string> getAllConstants() override;

	map<string, vector<string>> getVariableMap() override;

	map<string, vector<string>> getConstantMap() override;

	map<string, vector<string>> getClause(ABSTRACTION abstraction) override;


};
#endif
