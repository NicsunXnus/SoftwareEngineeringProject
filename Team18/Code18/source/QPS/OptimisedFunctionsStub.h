#pragma once
#include <vector>
#include <list>
#include "QueryResultsTable.h"

using namespace std;
class OptimisedFunctionsStub
{
private:
	// Stores the count of the header in a group of clauses
	inline static map<string, int> countHeadersStore;
	// The "switch" to activate the opimisation of the processing of the tables
	bool isOptimised = false;

public:
	/**
	 * Constructor for the ResultHandlerStub object
	 */
	OptimisedFunctionsStub() {
		
	};

	/**
	 * Destructor for the ResultHandlerStub object
	 */
	~OptimisedFunctionsStub() {};

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
	static void optimiseStepA(vector<shared_ptr<QueryResultsTable>>& nonSelectClauseTables);
	static void optimiseStepB(vector<shared_ptr<QueryResultsTable>>& nonSelectClauseTables);
	static void optimiseStepC(vector< vector<shared_ptr<QueryResultsTable>> >& groups);
	static vector<shared_ptr<QueryResultsTable>> flatten2DArray(vector< vector<shared_ptr<QueryResultsTable>> > v2d);
};

