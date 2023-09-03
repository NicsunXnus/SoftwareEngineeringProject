#pragma once
#ifndef QUERYOBJECT_H
#define QUERYOBJECT_H

#include "../TokenClasses/Token.h"

// API calls should be a function in the query object

/*
* This class represnts a Query object; abstractions that are used to query the PKB
*/
class QueryObject {
private:
	std::shared_ptr<Token> token;
public:
	QueryObject() {
		
	}

};

/*
* This class represnts a Query object, for design entities
*/
class DesignObject : public QueryObject {
public:
	DesignObject() {

	}
};

/*
* This class represnts a Query object, for design entity stmt
*/
class StmtObject : public DesignObject {
public:
	StmtObject() {

	}
};




#endif