#ifndef DATAACCESSLAYERAFFECTSSTUB_H
#define DATAACCESSLAYERAFFECTSSTUB_H

#include <string>
#include <unordered_map>
#include <vector>

#include "../Constants/DesignEnums.h"
#include "../PKB/PKB.h"
#include "../QPS/QueryObjects/ClauseArg.h"
#include "../QPS/QueryResultsTable.h"
#include "DataAccessLayer.h"

using namespace std;

class QueryObject;
/**
 * This class represents an interface object that calls APIs from the PKB
 */
class DataAccessLayerAffectsStub : public DataAccessLayer {
 private:
 public:
  /**
   * Constructor for the DataAccessLayer object
   */
  DataAccessLayerAffectsStub(){};

  /**
   * Destructor for the DataAccessLayer object
   */
  ~DataAccessLayerAffectsStub(){};

  unordered_set<string> getEntity(ENTITY type) override;

  unordered_set<string> getAllProcedures() override;

  unordered_set<string> getAllVariables() override;

  unordered_set<string> getAllConstants() override;

  StringMap getVariableMap() override;

  StringMap getConstantMap() override;

  shared_ptr<Node> getPatternTree(string statement_number) override;

  StringMap getClause(ABSTRACTION abstraction) override;

  StringMap getClauseInverse(ABSTRACTION abstraction) override;

  StringMap getCallProcNames() override;
  StringMap getReadVarNames() override;
  StringMap getPrintVarNames() override;

  pair<string, string> getProcLines(string procName) override;
};
#endif
