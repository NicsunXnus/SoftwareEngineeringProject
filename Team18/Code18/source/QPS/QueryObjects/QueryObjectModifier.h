#ifndef QUERYOBJECTMODIFIER_H
#define QUERYOBJECTMODIFIER_H

#include <iostream>
#include <string_view>
#include <vector>
#include "QueryObject.h"
#include "ClauseArg.h"
#include "../Errors/SemanticError.h"
#include "../Errors/SyntaxError.h"

/**
* This abstract class represents all Query Object Modifiers
*/
class QueryObjectModifier {
private:

public:
	QueryObjectModifier() {};

	/*
	* This function modifies a QueryObject and returns a modified QueryObject
	*/
	virtual inline shared_ptr<QueryObject> modify(shared_ptr<QueryObject> queryObject, shared_ptr<vector<QueryObject>> synonymDesignObjects = {}) = 0;

	/*
	* This function creates the factory for each type (design entities, clauses etc.)
	*/
	static shared_ptr<QueryObjectModifier> createModifier(string_view type);

};

/*
* This class represents a Not modifier object
*/
class NotModifier : public QueryObjectModifier {
private:

public:
	NotModifier() : QueryObjectModifier() {};

	/*
	* Modifies a query object and returns a negated version of the query object
	*/
	shared_ptr<QueryObject> modify(shared_ptr<QueryObject> queryObject, shared_ptr<vector<QueryObject>> synonymDesignObjects) override;
};

#endif