#include "StorageManager.h"

/**
* This class represents Insertor for the PKB.
*/
class Insertor {
public:
    static void setAST(TNode* r);

	// parse trees (for querying patterns)

	static void addEntityStatement(string entity, vector<int>);

	// add proc/var/const

	static void addAbstraction(string abstraction, KeyValue key, vector<int> lines);
};