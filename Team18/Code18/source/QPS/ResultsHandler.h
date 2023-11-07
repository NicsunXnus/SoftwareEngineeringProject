#ifndef RESULTSHANDLER_H
#define RESULTSHANDLER_H

#include <vector>
#include <list>
#include "QueryResultsTable.h"
#include "../HelperFunctions.h"
#include "../Constants/QPSConstants.h"

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
		vector<vector<string>> values = table->getVectorizedRows();
		vector<string> result;
		for (int i = 1; i < values.size(); i++) {
			string temp = "";
			for (string s : values[i]) {
				temp += s + " ";
			}
			temp.erase(temp.end() - 1);
			result.emplace_back(temp);
		}
		return result;
	}
	shared_ptr<QueryResultsTable> joinIntermediateTables(vector<shared_ptr<QueryResultsTable>> tables);
	list<string> returnTuples(vector<shared_ptr<QueryResultsTable>> selectClauseTables);

	list<string> handleSingleSynonym(vector<shared_ptr<QueryResultsTable>> selectClauseTables, vector<shared_ptr<QueryResultsTable>> nonSelectClauseTables);
	list<string> handleTuples(vector<shared_ptr<QueryResultsTable>> selectClauseTables, vector<shared_ptr<QueryResultsTable>> nonSelectClauseTables);
	list<string> handleBoolean(vector<shared_ptr<QueryResultsTable>> selectClauseTables, vector<shared_ptr<QueryResultsTable>> nonSelectClauseTables);

public:
	/**
	 * Constructor for the ResultHandler object
	 */
	ResultHandler() {};

	/**
	 * Destructor for the ResultHandler object
	 */
	~ResultHandler() {};
	
	// The QueryResultTables of the clauses are processed, and through a series of cross-products and/or inner joins, the final result
	// of the PQL is obtained.
	list<string> processTables(vector<shared_ptr<QueryResultsTable>> selectClause, vector<shared_ptr<QueryResultsTable>> nonSelectClause);
};
#endif
