#ifndef RESULTSHANDLER_H
#define RESULTSHANDLER_H

#include <vector>
#include <list>
#include "QueryResultsTable.h"
#include "../HelperFunctions.h"

using namespace std;
/**
 * This class represents a ResultsHandler object that processes tuples from the PKB to generate
 * query results
 */
class ResultHandler {
private:
	bool isTuples(vector<shared_ptr<QueryResultsTable>> selectClause) {
		return selectClause.size() > 1;
	}
	bool isSingleSynonym(vector<shared_ptr<QueryResultsTable>> selectClause) {
		return selectClause.size() == 1;
	}
	bool isBoolean(vector<shared_ptr<QueryResultsTable>> selectClause) {
		return selectClause.size() < 1;
	}

	vector<string> tableToVectorForTuples(shared_ptr<QueryResultsTable> table) {
		vector<string> result;
		vector<string> headers = table->getHeaders();
		int totalRow = table->getNumberOfRows();
		int totalHeaders = headers.size();

		for (int i = 0; i < totalRow; i++) {
			string curr = "";
			for (int j = 0; j < totalHeaders; j++) {
				vector<string> column = table->getColumnData(headers[j]);
				if (j == totalHeaders - 1) { // last element
					curr += column[i];
					break;
				}
				curr += column[i] + " ";
			}
			result.push_back(curr);
		}
		return result;
	}
	shared_ptr<QueryResultsTable> joinIntermediateTables(vector<shared_ptr<QueryResultsTable>> tables);
	list<string> returnTuples(vector<shared_ptr<QueryResultsTable>> selectClauseTables);

	list<string> handleSingleSynonym(vector<shared_ptr<QueryResultsTable>> selectClauseTables, vector<shared_ptr<QueryResultsTable>> nonSelectClauseTables);
	list<string> handleTuples(vector<shared_ptr<QueryResultsTable>> selectClauseTables, vector<shared_ptr<QueryResultsTable>> nonSelectClauseTables);
	//list<string> handleBoolean(vector<shared_ptr<QueryResultsTable>> selectClauseTables, vector<shared_ptr<QueryResultsTable>> nonSelectClauseTables);

public:
	/**
	 * Constructor for the ResultHandler object
	 */
	ResultHandler() {};

	/**
	 * Destructor for the ResultHandler object
	 */
	~ResultHandler() {};
	
	list<string> processTables(vector<shared_ptr<QueryResultsTable>> selectClause, vector<shared_ptr<QueryResultsTable>> nonSelectClause);


};
#endif
