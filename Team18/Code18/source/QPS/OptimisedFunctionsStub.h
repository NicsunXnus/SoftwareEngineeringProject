#pragma once
#include <vector>
#include <list>
#include <algorithm>
#include "QueryResultsTable.h"
#include "GroupClause.h"
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
	 * Constructor for the OptimisedFunctionsStub object
	 */
	OptimisedFunctionsStub() {
		
	};

	/**
	 * Destructor for the OptimisedFunctionsStub object
	 */
	~OptimisedFunctionsStub() {};

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
	static void optimiseStepA(vector<shared_ptr<QueryResultsTable>>& nonSelectClauseTables, bool isParallel);
	//static void optimiseStepA(shared_ptr<GroupClause> group);
	//static void optimiseStepB(vector<shared_ptr<QueryResultsTable>>& nonSelectClauseTables);
	static vector<shared_ptr<GroupClause>> optimiseStepB(vector<shared_ptr<QueryResultsTable>>& nonSelectClauseTables, bool isParallel);
	static void optimiseStepC(vector<shared_ptr<GroupClause>>& groups);
	static void optimiseStepD(vector<shared_ptr<GroupClause>>& groups);
	static vector<shared_ptr<QueryResultsTable>> revert1DTables(vector< shared_ptr<GroupClause> > groups);
	static bool sortMostUniqueHeadersFirst(const std::shared_ptr<QueryResultsTable>& a, const std::shared_ptr<QueryResultsTable>& b);
};

