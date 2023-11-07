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

	/*
	* This function returns the select clause headers, including non primary keys
	*/
	unordered_set<string> getSetOfSelectClauseHeaders(vector<shared_ptr<QueryResultsTable>> selectClauseTables);

	/*
	* This function returns the attribute synonym. e.g. p.procname will return "p"
	*/
	string getAttributeSynonym(string attribute);

	/*
	* This function returns all primary keys in the select clause (i.e. synoynms part of the final result)
	*/
	vector<string> getVectorOfSelectClausePkey(vector<shared_ptr<QueryResultsTable>> selectClauseTables);

	/*
	* This function drops irrelavant columns in the QRT, not needed in results
	* It returns the tables that are not in the QRT, and a QRT with columns in the select clause
	*/
	tuple<vector<shared_ptr<QueryResultsTable>>,
		shared_ptr<QueryResultsTable>> processIntermediateTable(vector<shared_ptr<QueryResultsTable>> selectClauseTables,
			shared_ptr<QueryResultsTable> intermediateTable);

	/*
	* This function converts a QRT to a vector of string, before conversion to list<string>
	*/
	vector<string> tableToVectorForTuples(shared_ptr<QueryResultsTable> table);

	/*
	* This function joins all the QRTs, by cross or inner joins
	*/
	shared_ptr<QueryResultsTable> joinIntermediateTables(vector<shared_ptr<QueryResultsTable>> tables);

	/*
	* This function handles the returning of tuple values, by cross join or duplication of columns
	*/
	list<string> returnTuples(vector<shared_ptr<QueryResultsTable>> selectClauseTables);

	/*
	* This function handles results with a single synonym
	*/
	list<string> handleSingleSynonym(vector<shared_ptr<QueryResultsTable>> selectClauseTables, vector<shared_ptr<QueryResultsTable>> nonSelectClauseTables);

	/*
	* This function handles results for tuples
	*/
	list<string> handleTuples(vector<shared_ptr<QueryResultsTable>> selectClauseTables, vector<shared_ptr<QueryResultsTable>> nonSelectClauseTables);

	/*
	* This function handles results for booleans
	*/
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
