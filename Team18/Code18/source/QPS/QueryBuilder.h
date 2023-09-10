#ifndef QUERYBUILDER_H
#define QUERYBUILDER_H

#include <iostream>
#include <string_view>
#include <vector>

#include "Token.h"

/**
* This class a query builder object
*/
class QueryBuilder {
private:
	std::vector<QueryObject> queryObjects;
public:
	/**
	 * Constructor for the QueryBuilder object
	 */
	QueryBuilder() {};

	/**
	 * Destructor for the QueryBuilder object
	 */
	~QueryBuilder() {};

	/**
	 * Builds the query results given a list of query objects
	 */
	buildQuery(std::vector<QueryObjects> queryObjects);
	  
	
};
#endif
