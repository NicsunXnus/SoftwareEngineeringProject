#ifndef QUERYOBJECTFACTORY_H
#define QUERYOBJECTFACTORY_H

#include <iostream>
#include <string_view>
#include "QueryObject.h"

/**
* This abstract class represents all Query Object factories
*/
class QueryObjectFactory {
private: 
	// string_view factoryType;
public:
	QueryObjectFactory() {

	};
	/*
	* This function creates the QueryObject, overriden in the subclasses for each type (design entities, clauses etc.)
	*/
	virtual shared_ptr<QueryObject> create(string_view type) = 0;

	/*
	* This function creates the factory for each type (design entities, clauses etc.)
	*/
	static shared_ptr<QueryObjectFactory> createFactory(string_view type);
		
};
#endif
