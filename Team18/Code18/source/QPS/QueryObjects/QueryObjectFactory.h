#ifndef QUERYOBJECTFACTORY_H
#define QUERYOBJECTFACTORY_H

#include <iostream>
#include <string_view>
#include "QueryObject.h"

/**
* This class represents all Query Object factories
*/
class QueryObjectFactory {
private: 
	// string_view factoryType;
public:
	QueryObjectFactory() {

	};

	virtual shared_ptr<QueryObject> create(string_view str) = 0;
};
#endif
