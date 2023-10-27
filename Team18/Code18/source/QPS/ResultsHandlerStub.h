#pragma once
#include <vector>
#include <list>
#include "QueryResultsTable.h"
#include "../HelperFunctions.h"

using namespace std;
class ResultsHandlerStub
{
private:
	// Stores the count of the header in a group of clauses
	inline static map<string, int> countHeadersStore;
	// The "switch" to activate the opimisation of the processing of the tables
	bool isOptimised = false;

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
				vector<map<string, vector<string>>> cols = table->getColumns();
				vector<string> column = cols[j].begin()->second;
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
	 * Constructor for the ResultHandlerStub object
	 */
	ResultsHandlerStub() {
		
	};

	/**
	 * Destructor for the ResultHandlerStub object
	 */
	~ResultsHandlerStub() {};

	// The QueryResultTables of the clauses are processed, and through a series of cross-products and/or inner joins, the final result
	// of the PQL is obtained.
	list<string> processTables(vector<shared_ptr<QueryResultsTable>> selectClause, vector<shared_ptr<QueryResultsTable>> nonSelectClause);

	void setOptimiseSwitch() {
		isOptimised = !isOptimised;
	}

	bool getOptimisedSwitch() {
		return isOptimised;
	}

	static void updateCountHeaderStore(string header) {
		countHeadersStore[header]++;
	}

	static void resetCountHeaderStore() {
		countHeadersStore.clear();
	}

	int static getCount(string header) {
		return countHeadersStore[header];
	}

	static set<string> getHeadersOfTableAsSet(vector<shared_ptr<QueryResultsTable>> selectClauseTables);
	static void optimiseStepA(vector<shared_ptr<QueryResultsTable>> selectClauseTables, vector<shared_ptr<QueryResultsTable>>& nonSelectClauseTables);
	static vector<shared_ptr<QueryResultsTable>> optimiseStepB(vector<shared_ptr<QueryResultsTable>> nonSelectClauseTables);
	static void optimiseStepC(vector< vector<shared_ptr<QueryResultsTable>> >& groups);
	static vector<shared_ptr<QueryResultsTable>> flatten2DArray(vector< vector<shared_ptr<QueryResultsTable>> > v2d);
};

