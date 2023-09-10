#ifndef CLAUSEOBJECTFACTORY_H
#define CLAUSEOBJECTFACTORY_H

#include <string_view>
#include "ClauseObject.h"
#include "QueryObjectFactory.h"


class ClauseObjectFactory : public QueryObjectFactory {
public:
	ClauseObjectFactory() : QueryObjectFactory{} {

	}


	/*
	* This function takes in a string and returns the DesignObject related to the string. Throws exception otherwise
	*/
	static shared_ptr<ClauseObject> createClauseObject(string_view type);
};

#endif