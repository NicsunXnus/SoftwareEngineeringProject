#include <string>

#include "StorageClasses/AbstractionStorage.h"
#include "StorageClasses/EntityStorage.h"
#include "StorageClasses/UsesAbstractionStorage.h"

using namespace std;

class TNode;  // change to AST Node

/**
* This class represents Storage for the PKB. Consists of the storages and AST
*/
class StorageManager {
public:
	// Methods to insert into storages

	// Methods to query storages

private:
	static TNode* getAST();
	static AbstractionStorage* getAbstraction(string);
	static EntityStorage* getEntities();

	// storages and AST pointer
	static TNode* AST;
	static UsesAbstractionStorage uses_abstractions;
	static EntityStorage entities;
};