#include "StorageManager.h"

/**
* This class represents Insertor for the PKB.
*/
class Insertor {
public:
    static void setAST(TNode* r);

	static void AddEntityToPKB(string entity, vector<int>);

	static void AddAbstractionToPKB(string abstraction, KeyValue key, vector<int> lines);
};