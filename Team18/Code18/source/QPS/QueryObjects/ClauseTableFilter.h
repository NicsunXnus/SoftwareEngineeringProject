#pragma once
#ifndef CLAUSETABLEFILTER_H
#define CLAUSETABLEFILTER_H

#include "QueryEval.h"



/*
* This class encapsulates the logic handler for clauses (follows(*), modifies(P), uses(P), parent(*), next, calls(*) only)
* Only for clauses that pull data from the PKB and convert it to a table through FILTERING the PKB data
*/
class ClauseFilterHandler : public QueryEval {
protected:
	shared_ptr<ClauseArg> arg1;
	shared_ptr<ClauseArg> arg2;
public:
	ClauseFilterHandler(shared_ptr<ClauseArg> argument1, shared_ptr<ClauseArg> argument2) : arg1(argument1), arg2(argument2) {

	}
};

/*
* For Synonym, Synonym arguments
*/
class ClauseSynSyn : public ClauseFilterHandler {
public:
	ClauseSynSyn(shared_ptr<ClauseArg> argument1, shared_ptr<ClauseArg> argument2) : ClauseFilterHandler{ argument1 , argument2 } {

	}
	shared_ptr<QueryResultsTable> evaluate(shared_ptr<DataAccessLayer> dataAccessLayer, ABSTRACTION clause) override;
};

/*
* For Synonym, integer/identifier arguments
*/
class ClauseSynIntOrIdent : public ClauseFilterHandler {
public:
	ClauseSynIntOrIdent(shared_ptr<ClauseArg> argument1, shared_ptr<ClauseArg> argument2) : ClauseFilterHandler{ argument1 , argument2 } {

	}
	shared_ptr<QueryResultsTable> evaluate(shared_ptr<DataAccessLayer> dataAccessLayer, ABSTRACTION clause) override;
};

/*
* For Synonym, wildcard arguments
*/
class ClauseSynWildcard : public ClauseFilterHandler {
public:
	ClauseSynWildcard(shared_ptr<ClauseArg> argument1, shared_ptr<ClauseArg> argument2) : ClauseFilterHandler{ argument1 , argument2 } {

	}
	shared_ptr<QueryResultsTable> evaluate(shared_ptr<DataAccessLayer> dataAccessLayer, ABSTRACTION clause) override;
};

/*
* For Integer/identifier, synonym arguments
*/
class ClauseIntOrIdentSyn : public ClauseFilterHandler {
public:
	ClauseIntOrIdentSyn(shared_ptr<ClauseArg> argument1, shared_ptr<ClauseArg> argument2) : ClauseFilterHandler{ argument1 , argument2 } {

	}
	shared_ptr<QueryResultsTable> evaluate(shared_ptr<DataAccessLayer> dataAccessLayer, ABSTRACTION clause) override;
};

/*
* For Integer/identifier, Integer/identifier arguments
*/
class ClauseIntOrIdentIntOrIdent : public ClauseFilterHandler {
public:
	ClauseIntOrIdentIntOrIdent(shared_ptr<ClauseArg> argument1, shared_ptr<ClauseArg> argument2) : ClauseFilterHandler{ argument1 , argument2 } {

	}
	shared_ptr<QueryResultsTable> evaluate(shared_ptr<DataAccessLayer> dataAccessLayer, ABSTRACTION clause) override;
};

/*
* For Integer/identifier, wildcard arguments
*/
class ClauseIntOrIdentWildcard : public ClauseFilterHandler {
public:
	ClauseIntOrIdentWildcard(shared_ptr<ClauseArg> argument1, shared_ptr<ClauseArg> argument2) : ClauseFilterHandler{ argument1 , argument2 } {

	}
	shared_ptr<QueryResultsTable> evaluate(shared_ptr<DataAccessLayer> dataAccessLayer, ABSTRACTION clause) override;
};

/*
* For wildcard, syn arguments
*/
class ClauseWildcardSyn : public ClauseFilterHandler {
public:
	ClauseWildcardSyn(shared_ptr<ClauseArg> argument1, shared_ptr<ClauseArg> argument2) : ClauseFilterHandler{ argument1 , argument2 } {

	}
	shared_ptr<QueryResultsTable> evaluate(shared_ptr<DataAccessLayer> dataAccessLayer, ABSTRACTION clause) override;
};

/*
* For wildcard, Integer/identifier arguments
*/
class ClauseWildcardIntOrIdent : public ClauseFilterHandler {
public:
	ClauseWildcardIntOrIdent(shared_ptr<ClauseArg> argument1, shared_ptr<ClauseArg> argument2) : ClauseFilterHandler{ argument1 , argument2 } {

	}
	shared_ptr<QueryResultsTable> evaluate(shared_ptr<DataAccessLayer> dataAccessLayer, ABSTRACTION clause) override;
};

/*
* For wildcard, wildcard arguments
*/
class ClauseWildcardWildcard : public ClauseFilterHandler {
public:
	ClauseWildcardWildcard(shared_ptr<ClauseArg> argument1, shared_ptr<ClauseArg> argument2) : ClauseFilterHandler{ argument1 , argument2 } {

	}
	shared_ptr<QueryResultsTable> evaluate(shared_ptr<DataAccessLayer> dataAccessLayer, ABSTRACTION clause) override;
};

/* HELPER FUNCTIONS */
inline StringMap filterMapKeyReturnMap(shared_ptr<ClauseArg> arg, shared_ptr<DataAccessLayer> dataAccessLayer, StringMap PKBClauseData) {
	ENTITY typeArg1 = arg->getSynonym()->getEntityType();
	unordered_set<string> PKBArg1EntityData = dataAccessLayer->getEntity(typeArg1);
	StringMap filteredPKBClauseData;
	for (const auto& entry : PKBClauseData) {
		string leftTuple = entry.first;
		auto it = PKBArg1EntityData.find(leftTuple);
		if (it != PKBArg1EntityData.end()) {
			if (entry.second.size() > 0) {
				filteredPKBClauseData[entry.first] = entry.second;
			}
		}
	}
	return filteredPKBClauseData;
}

inline StringMap filterMapValueReturnMap(shared_ptr<ClauseArg> arg, shared_ptr<DataAccessLayer> dataAccessLayer, StringMap PKBClauseData) {
	ENTITY typeArg2 = arg->getSynonym()->getEntityType();
	unordered_set<string> PKBArg2EntityData = dataAccessLayer->getEntity(typeArg2);
	StringMap filteredPKBClauseData;
	for (const auto& entry : PKBClauseData) {
		unordered_set<string> intersection;
		unordered_set<string> rightTuple = entry.second;
		for (const string& element : rightTuple) {
			if (PKBArg2EntityData.find(element) != PKBArg2EntityData.end()) {
				intersection.insert(element);
			}
		}
		filteredPKBClauseData[entry.first] = intersection;
	}
	return filteredPKBClauseData;
}


inline unordered_set<string> filterSetReturnSet(shared_ptr<ClauseArg> arg, shared_ptr<DataAccessLayer> dataAccessLayer, unordered_set<string> PKBClauseData) {
	ENTITY typeArg2 = arg->getSynonym()->getEntityType();
	unordered_set<string> PKBArg2EntityData = dataAccessLayer->getEntity(typeArg2);
	unordered_set<string> filteredPKBClauseData;
	for (const string& element : PKBClauseData) {
		if (PKBArg2EntityData.find(element) != PKBArg2EntityData.end()) {
			filteredPKBClauseData.insert(element);
		}
	}
	return filteredPKBClauseData;
}

inline unordered_set<string> filterMapKeyReturnSetValues(shared_ptr<ClauseArg> arg, shared_ptr<DataAccessLayer> dataAccessLayer, StringMap PKBClauseData) {
	unordered_set<string> filteredPKBClauseData;
	auto it = PKBClauseData.find(svToString(arg->getArgValue()));
	if (it != PKBClauseData.end()) {
		return PKBClauseData[svToString(arg->getArgValue())];
	}
	return filteredPKBClauseData;
}

inline unordered_set<string> filterMapKeyReturnSetValues(string arg, shared_ptr<DataAccessLayer> dataAccessLayer, StringMap PKBClauseData) {
	unordered_set<string> filteredPKBClauseData;
	auto it = PKBClauseData.find(arg);
	if (it != PKBClauseData.end()) {
		return PKBClauseData[arg];
	}
	return filteredPKBClauseData;
}


inline bool filterSetReturnBool(shared_ptr<ClauseArg> arg, shared_ptr<DataAccessLayer> dataAccessLayer, unordered_set<string> PKBClauseData) {
	auto it = PKBClauseData.find(svToString(arg->getArgValue()));
	if (it != PKBClauseData.end()) {
		return true;
	}

	return false;
}

inline unordered_set<string> removeMapValuesReturnSet(shared_ptr<ClauseArg> arg, shared_ptr<DataAccessLayer> dataAccessLayer, StringMap PKBClauseData) {
	unordered_set<string> keySet;

	// Extract keys from the map and insert them into the set
	for (const auto& pair : PKBClauseData) {
		keySet.insert(pair.first);
	}
	return keySet;
}

inline unordered_set<string> filterSynSyn(StringMap results) {
	unordered_set<string> result;
	for (const auto& entry : results) {
		for (string val : entry.second) {
			if (val == entry.first) {
				result.insert(val);
			}
		}
	}
	return result;
}

inline unordered_set<string> keepMapValues(StringMap results) {
	unordered_set<string> result;
	for (const auto& entry : results) {
		for (string val : entry.second) {
			result.insert(val);
		}
	}
	return result;
}

/*
* Factory class for clause filters
*/
class ClauseFilterFactory {
public:
	static shared_ptr<QueryEval> create(shared_ptr<ClauseArg> argument1, shared_ptr<ClauseArg> argument2);
};


#endif