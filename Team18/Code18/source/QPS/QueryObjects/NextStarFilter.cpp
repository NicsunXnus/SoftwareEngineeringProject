#include "ClauseTableFilter.h"
#include "NextStarFilter.h"

typedef tuple<string, unordered_set<string>> NextStackElement; // parent, set of childs

// DFS from each node: large runtime
shared_ptr<QueryResultsTable> NextStarSynSyn::evaluate(shared_ptr<DataAccessLayer> dataAccessLayer, ABSTRACTION clause) {
	shared_ptr<ExtendedCFG> cfg = dataAccessLayer->getCFG();
	StringMap cfgResults = cfg->nextStarSynSyn();
	StringMap results = filterMapKeyReturnMap(arg1, dataAccessLayer, cfgResults);
	if (arg1->getArgValue() == arg2->getArgValue()) {
		unordered_set<string> result = filterSynSyn(results);
		shared_ptr<QueryResultsTable> qrt = QueryResultsTable::createTable(svToString(arg1->getArgValue()), result);
		return qrt;
	}
	vector<string> headers({ svToString(arg1->getArgValue()), svToString(arg2->getArgValue()) });
	shared_ptr<QueryResultsTable> qrt = QueryResultsTable::createTable(headers, results);
	return qrt;
}


shared_ptr<QueryResultsTable> NextStarSynInt::evaluate(shared_ptr<DataAccessLayer> dataAccessLayer, ABSTRACTION clause) {
	shared_ptr<ExtendedCFG> cfg = dataAccessLayer->getCFG();
	unordered_set<string> cfgResults = cfg->nextStarSynInt(svToString(arg2->getArgValue()));
	unordered_set<string> results = filterSetReturnSet(arg1, dataAccessLayer, cfgResults);

	shared_ptr<QueryResultsTable> qrt = QueryResultsTable::createTable(svToString(arg1->getArgValue()), results);
	return qrt;
}

shared_ptr<QueryResultsTable> NextStarIntSyn::evaluate(shared_ptr<DataAccessLayer> dataAccessLayer, ABSTRACTION clause) {
	shared_ptr<ExtendedCFG> cfg = dataAccessLayer->getCFG();
	unordered_set<string> cfgResults = cfg->nextStarIntSyn(svToString(arg1->getArgValue()));
	unordered_set<string> results = filterSetReturnSet(arg2, dataAccessLayer, cfgResults);
	shared_ptr<QueryResultsTable> qrt = QueryResultsTable::createTable(svToString(arg2->getArgValue()), results);
	return qrt;
}


// DFS, the idea is to start from the first integer, then traverse until we hit the other integer node and return an empty, but significant table
// If node is not found, return an empty table
shared_ptr<QueryResultsTable> NextStarIntInt::evaluate(shared_ptr<DataAccessLayer> dataAccessLayer, ABSTRACTION clause) {
	shared_ptr<ExtendedCFG> cfg = dataAccessLayer->getCFG();
	bool results = cfg->nextStarIntInt(svToString(arg1->getArgValue()), svToString(arg2->getArgValue()));
	shared_ptr<QueryResultsTable> qrt = QueryResultsTable::createEmptyTable(results);
	return qrt;
}

shared_ptr<QueryEval> NextStarFilterFactory::create(shared_ptr<ClauseArg> argument1, shared_ptr<ClauseArg> argument2) {
	if (argument1->isSynonym() && argument2->isSynonym()) {
		return make_shared<NextStarSynSyn>(argument1, argument2);
	}
	else if (argument1->isSynonym() && argument2->isWildcard()) {
		return make_shared<ClauseSynWildcard>(argument1, argument2);
	}
	else if (argument1->isSynonym() && argument2->isInteger()) {
		return make_shared<NextStarSynInt>(argument1, argument2);
	}
	else if (argument1->isInteger() && argument2->isSynonym()) {
		return make_shared<NextStarIntSyn>(argument1, argument2);
	}
	else if (argument1->isInteger() && argument2->isWildcard()) {
		return make_shared<ClauseIntOrIdentWildcard>(argument1, argument2);
	}
	else if (argument1->isInteger() && argument2->isInteger()) {
		return make_shared<NextStarIntInt>(argument1, argument2);
	}
	else if (argument1->isWildcard() && argument2->isSynonym()) {
		return make_shared<ClauseWildcardSyn>(argument1, argument2);
	}
	else if (argument1->isWildcard() && argument2->isWildcard()) {
		return make_shared<ClauseWildcardWildcard>(argument1, argument2);
	}
	else if (argument1->isWildcard() && argument2->isInteger()) {
		return make_shared<ClauseWildcardIntOrIdent>(argument1, argument2);
	}
	throw SemanticErrorException("Error in nextstar filter factory, invalid argument combination");
}