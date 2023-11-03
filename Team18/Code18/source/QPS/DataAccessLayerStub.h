#ifndef DATAACCESSLAYERSTUB_H
#define DATAACCESSLAYERSTUB_H

#include <vector>
#include <string>
#include <unordered_map>
#include "../PKB/PKB.h"
#include "../Constants/DesignEnums.h"
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

	unordered_set<string> getEntity(ENTITY type) override;

	unordered_set<string> getAllProcedures() override;

	unordered_set<string> getAllVariables() override;

	unordered_set<string> getAllConstants() override;

	map<string, unordered_set<string>> getVariableMap() override;

	map<string, unordered_set<string>> getConstantMap() override;

	shared_ptr<Node> getPatternTree(string statement_number) override;

	map<string, unordered_set<string>> getClause(ABSTRACTION abstraction) override;

	map<string, unordered_set<string>> getClauseInverse(ABSTRACTION abstraction) override;

	StringMap getCallProcNames() override;
	StringMap getReadVarNames() override;
	StringMap getPrintVarNames() override;


};
#endif
