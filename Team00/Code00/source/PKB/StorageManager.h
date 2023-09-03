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

// class VarTable;  // no need to #include "VarTable.h" as all I need is pointer
	// fields
	// static VarTable* varTable;  // shift to private field of Storages