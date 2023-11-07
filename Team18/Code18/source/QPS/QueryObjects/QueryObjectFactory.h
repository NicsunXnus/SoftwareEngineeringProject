#ifndef QUERYOBJECTFACTORY_H
#define QUERYOBJECTFACTORY_H

#include <iostream>
#include <string_view>
#include <vector>
#include "QueryObject.h"
#include "ClauseArg.h"
#include "../Errors/SemanticError.h"
#include "../Errors/SyntaxError.h"

/**
* This abstract class represents all Query Object factories
*/
class QueryObjectFactory {
private: 

public:
	QueryObjectFactory() {

	};
	/*
	* This function creates the QueryObject, overriden in the subclasses for design entities
	*/
	virtual inline shared_ptr<QueryObject> create(string_view name, vector<shared_ptr<ClauseArg>> arguments = {}) = 0;

	/*
	* This function creates the factory for each type (design entities, clauses etc.)
	*/
	static shared_ptr<QueryObjectFactory> createFactory(string_view type);

};
#endif
