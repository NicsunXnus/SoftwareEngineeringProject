#include "StorageClasses/AbstractionStorage.h"
#include "StorageClasses/EntityStorage.h"

class TNode;  // change to AST Node

/**
* This class represents Storage for the PKB. Consists of the storages and AST
*/
class StorageManager {
public:
	static TNode* getAST();
	static AbstractionStorage getAbstractions();
	static EntityStorage getEntities();

private:
	// storages and AST pointer
	static TNode* AST;
	static AbstractionStorage abstractions;
	static EntityStorage entities;
};