#ifndef QUERYDRIVER_H
#define QUERYDRIVER_H

#include <string>
#include <string_view>
#include <vector>

#include "QueryParser.h"
#include "PQLTokenizer.h"
#include "DataAccessLayer.h"
#include "QueryBuilder.h"

using namespace std;

/**
 * This class represents a Query Driver. The Query Driver's responsibility is to 
 * take in a query and return the results of the query.
 */
class QueryDriver {
private:
	string query;
	//shared_ptr<QueryParser> queryParser;

public:
	/**
	 * Constructor of the QueryDriver object
	 */
	QueryDriver(string query)
		: query { query } {
		//queryParser = make_shared<QueryParser>();
	};
	
	/**
	 * Destructor of the QueryDriver object
	 */
	~QueryDriver() {};

	list<string> execute();


};
#endif
