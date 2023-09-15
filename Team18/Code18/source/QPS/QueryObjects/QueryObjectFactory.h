#ifndef QUERYOBJECTFACTORY_H
#define QUERYOBJECTFACTORY_H

#include <iostream>
#include <string_view>
#include <vector>
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
	* This function creates the QueryObject, overriden in the subclasses for design entities
	*/
	virtual inline shared_ptr<QueryObject> create(vector<string_view> data) = 0;

	/*
	* This function checks if a given /

	/*
	* This function creates the factory for each type (design entities, clauses etc.)
	*/
	static shared_ptr<QueryObjectFactory> createFactory(string_view type);
		
};
#endif
