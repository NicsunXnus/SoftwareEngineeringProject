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
/*
* This function takes in the PKB map, and filters the map key based on the supplied arg (a design entity) and its type
* The same Map is returned, after being filtered
*/
inline StringMap filterMapKeyReturnMap(shared_ptr<ClauseArg> arg, shared_ptr<DataAccessLayer> dataAccessLayer, StringMap PKBMap) {
	ENTITY argType = arg->getSynonym()->getEntityType();
	unordered_set<string> argEntitySet = dataAccessLayer->getEntity(argType);
	StringMap filteredMapKey;
	for (const auto& entry : PKBMap) {
		string leftTuple = entry.first;
		auto it = argEntitySet.find(leftTuple);
		if (it != argEntitySet.end()) {
			if (entry.second.size() > 0) {
				filteredMapKey[entry.first] = entry.second;
			}
		}
	}
	return filteredMapKey;
}

/*
* This function takes in the PKB map, and filters the map values based on the supplied arg(a design entity) and its type
* The same Map is returned, after being filtered
*/
inline StringMap filterMapValueReturnMap(shared_ptr<ClauseArg> arg, shared_ptr<DataAccessLayer> dataAccessLayer, StringMap PKBMap) {
	ENTITY argType = arg->getSynonym()->getEntityType();
	unordered_set<string> argEntitySet = dataAccessLayer->getEntity(argType);
	StringMap filteredMapValue;
	for (const auto& entry : PKBMap) {
		unordered_set<string> intersection;
		unordered_set<string> rightTuple = entry.second;
		for (const string& element : rightTuple) {
			if (argEntitySet.find(element) != argEntitySet.end()) {
				intersection.insert(element);
			}
		}
		filteredMapValue[entry.first] = intersection;
	}
	return filteredMapValue;
}

/*
* This function takes in the PKB set, and filters the set values based on the supplied arg (a design entity) and its type
* The same set is returned, after being filtered
*/
inline unordered_set<string> filterSetReturnSet(shared_ptr<ClauseArg> arg, shared_ptr<DataAccessLayer> dataAccessLayer, unordered_set<string> PKBSet) {
	ENTITY argType = arg->getSynonym()->getEntityType();
	unordered_set<string> argEntitySet = dataAccessLayer->getEntity(argType);
	unordered_set<string> filteredSet;
	for (const string& element : PKBSet) {
		if (argEntitySet.find(element) != argEntitySet.end()) {
			filteredSet.insert(element);
		}
	}
	return filteredSet;
}

/*
* This function takes in the PKB Map, and filters the map values based on the supplied arg (which is an ident or syn)
* The specific row is returned if found
*/
inline unordered_set<string> filterMapKeyReturnSetValues(shared_ptr<ClauseArg> arg, StringMap PKBMap) {
	auto it = PKBMap.find(svToString(arg->getArgValue()));
	if (it != PKBMap.end()) {
		return PKBMap[svToString(arg->getArgValue())];
	}
	return {};
}

/*
* This function takes in the PKB Map, and filters the map values based on the supplied arg (which is an ident or syn)
* The specific row is returned if found
* This function is an overriden version
*/
inline unordered_set<string> filterMapKeyReturnSetValues(string arg, StringMap PKBMap) {
	unordered_set<string> filteredSet;
	auto it = PKBMap.find(arg);
	if (it != PKBMap.end()) {
		return PKBMap[arg];
	}
	return filteredSet;
}

/*
* This function takes in the PKB set, and filters the set values based on the supplied arg
* if the arg is found, return true, else return false
*/
inline bool filterSetReturnBool(shared_ptr<ClauseArg> arg, unordered_set<string> PKBSet) {
	auto it = PKBSet.find(svToString(arg->getArgValue()));
	if (it != PKBSet.end()) {
		return true;
	}

	return false;
}

/*
* This function takes in the PKB Map, and simply drops all the values and returns the set of keys
*/
inline unordered_set<string> removeMapValuesReturnSet(shared_ptr<ClauseArg> arg, StringMap PKBMap) {
	unordered_set<string> keySet;

	// Extract keys from the map and insert them into the set
	for (const auto& pair : PKBMap) {
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