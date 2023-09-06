#include "StorageManager.h"

/**
* This class represents Insertor for the PKB.
*/
class Insertor {
public:
    static void setAST(TNode* r);

	// parse trees (for querying patterns)

	static void addEntityStatement(string statement, vector<int> lines);

	static void addProcedure(string procedure, vector<int> lines);

	static void addVariable(string variable, vector<int> lines);

	static void addConstant(string constant, vector<int> lines);

	static void addAbstraction(string abstraction, KeyValue key, vector<int> lines);
};