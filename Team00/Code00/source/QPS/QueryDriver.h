#ifndef QUERYDRIVER_H
#define QUERYDRIVER_H

#include <string>
#include <string_view>
#include <vector>

#include "QueryParser.h"

/**
 * This class represents a Query Driver. The Query Driver's responsibility is to 
 * take in a query and return the results of the query.
 */
class QueryDriver {
private:
	std::string query;
	QueryParser queryParser;

public:
	/**
	 * Constructor of the QueryDriver object
	 */
	QueryDriver(std::string_view query)
		: query { query } {
		queryParser = new QueryParser();
	};
	
	/**
	 * Destructor of the QueryDriver object
	 */
	~QueryDriver() {};

	/**
	 * Passes the query down the query parser for parsing
	 */
	generateQueryObjects(std::string_view query);


};
#endif
