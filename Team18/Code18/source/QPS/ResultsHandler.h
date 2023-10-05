#ifndef RESULTSHANDLER_H
#define RESULTSHANDLER_H

#include <vector>
#include <list>
#include "QueryResultsTable.h"
#include "../HelperFunctions.h"
/**
 * This class represents a ResultsHandler object that processes tuples from the PKB to generate
 * query results
 */
class ResultHandler {
private:
public:
	/**
	 * Constructor for the ResultHandler object
	 */
	ResultHandler() {};

	/**
	 * Destructor for the ResultHandler object
	 */
	~ResultHandler() {};
	
	list<string> processTables(std::vector<std::shared_ptr<QueryResultsTable>>);


};
#endif
