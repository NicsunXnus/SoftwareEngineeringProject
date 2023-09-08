#ifndef RESULTSHANDLER_H
#define RESULTSHANDLER_H

#include <string_view>
#include <vector>

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

	std::string_view processTuples(std::vector<Tuples> tuples);


};
#endif
