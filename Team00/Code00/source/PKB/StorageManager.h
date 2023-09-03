#include <string>

#include "StorageClasses/AbstractionStorage.h"
#include "StorageClasses/EntityStorage.h"
#include "StorageClasses/UsesAbstractionStorage.h"
#include "StorageClasses/ModifiesAbstractionStorage.h"
#include "StorageClasses/FollowsAbstractionStorage.h"
#include "StorageClasses/ParentAbstractionStorage.h"

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
	static EntityStorage entities;
	static UsesAbstractionStorage uses_abstractions;
	static ModifiesAbstractionStorage modifies_abstractions;
	static FollowsAbstractionStorage follows_abstractions;
	static ParentAbstractionStorage parent_abstractions;
};