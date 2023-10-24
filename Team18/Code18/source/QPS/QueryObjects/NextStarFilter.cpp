#include "ClauseTableFilter.h"
#include "NextStarFilter.h"

typedef tuple<string, unordered_set<string>> NextStackElement; // parent, set of childs

// DFS from each node: large runtime
shared_ptr<QueryResultsTable> NextStarSynSyn::evaluate(shared_ptr<DataAccessLayer> dataAccessLayer, ABSTRACTION clause) {
	StringMap PKBClauseDataArg1 = dataAccessLayer->getClause(clause);
	StringMap filteredPKBClauseDataArg1 = filterMapKeyReturnMap(arg1, dataAccessLayer, PKBClauseDataArg1);
	unordered_set<string> filteredPKBClauseDataKeepArg1 = removeMapValuesReturnSet(arg2, dataAccessLayer, filteredPKBClauseDataArg1); // nodes that have children

	StringMap PKBClauseDataArg2 = dataAccessLayer->getClauseInverse(clause);
	StringMap filteredPKBClauseDataArg2 = filterMapKeyReturnMap(arg2, dataAccessLayer, PKBClauseDataArg2);
	unordered_set<string> filteredPKBClauseDataKeepArg2 = removeMapValuesReturnSet(arg1, dataAccessLayer, filteredPKBClauseDataArg2); // nodes that have parent

	if (filteredPKBClauseDataKeepArg1.empty() || filteredPKBClauseDataKeepArg2.empty()) {
		StringMap empty;
		vector<string> headers({ svToString(arg1->getArgValue()), svToString(arg2->getArgValue()) });
		return QueryResultsTable::createTable(headers, empty);
	}

	StringMap nextStarTable; // stores all results, key = parent, value = children
	for (string parent : filteredPKBClauseDataKeepArg1) {
		stack<NextStackElement> nextStack; // parent, set of childs
		unordered_set<string> visited;
		nextStack.push(make_tuple(parent, PKBClauseDataArg1[parent]));

		unordered_set<string> resultSet = {};
		while (!nextStack.empty())
		{
			NextStackElement curr = nextStack.top();
			nextStack.pop();
			string parent = get<0>(curr);
			unordered_set<string> children = get<1>(curr);

			for (string child : children) {
				auto visitedIt = visited.find(child);
				if (visitedIt == visited.end()) {
					visited.insert(child); // add here to ensure that Next* does not add back intial node, unless it really can be reached via a loop
					unordered_set<string> nextChildren = filterMapKeyReturnSetValues(child, dataAccessLayer, PKBClauseDataArg1);
					nextStack.push(make_tuple(child, nextChildren));
				}
				auto arg2It = filteredPKBClauseDataKeepArg2.find(child);
				if (arg2It != filteredPKBClauseDataKeepArg2.end()) {  // current node is syn2, an answer we want
					resultSet.insert(child);
				}

			}
		}
		if (resultSet.size() > 0) {
			if (arg1->getArgValue() == arg2->getArgValue()) { // edge case where syn = syn e.g. Next*(s, s)
				auto it = resultSet.find(parent);
				if (it != resultSet.end()) {
					unordered_set<string> arg2Set = { parent };
					nextStarTable[parent] = arg2Set;
				}
			}
			else {
				nextStarTable[parent] = resultSet;
			}

		}
	}
	vector<string> headers({ svToString(arg1->getArgValue()), svToString(arg2->getArgValue()) });
	return QueryResultsTable::createTable(headers, nextStarTable);
}

// DFS, the idea is to start from the integer, then traverse and add each node visited to a set. This set represents the visited nodes
inline shared_ptr<QueryResultsTable> handleNextStarIntSynCombination(shared_ptr<ClauseArg> arg1, shared_ptr<ClauseArg> arg2, shared_ptr<DataAccessLayer> dataAccessLayer, StringMap PKBClauseData) {
	stack<NextStackElement> nextStack; // parent, set of childs
	unordered_set<string> visited;
	unordered_set<string> filteredPKBClauseDataArg1 = filterMapKeyReturnSetValues(arg1, dataAccessLayer, PKBClauseData);

	if (filteredPKBClauseDataArg1.empty()) {
		return QueryResultsTable::createTable(svToString(arg2->getArgValue()), visited);
	}
	nextStack.push(make_tuple(svToString(arg1->getArgValue()), filteredPKBClauseDataArg1));

	while (!nextStack.empty())
	{
		NextStackElement curr = nextStack.top();
		nextStack.pop();
		string parent = get<0>(curr);
		unordered_set<string> children = get<1>(curr);

		for (string child : children) {
			auto it = visited.find(child);
			if (it == visited.end()) {
				visited.insert(child); // add here to ensure that Next* does not add back intial node, unless it really can be reached via a loop
				unordered_set<string> nextChildren = filterMapKeyReturnSetValues(child, dataAccessLayer, PKBClauseData);
				nextStack.push(make_tuple(child, nextChildren));
			}

		}
	}
	unordered_set<string> filteredPKBClauseDataArg2 = filterSetReturnSet(arg2, dataAccessLayer, visited); // visited set is all the children nodes, filter out the relevant synonyms
	return QueryResultsTable::createTable(svToString(arg2->getArgValue()), filteredPKBClauseDataArg2);


}

shared_ptr<QueryResultsTable> NextStarSynInt::evaluate(shared_ptr<DataAccessLayer> dataAccessLayer, ABSTRACTION clause) {
	StringMap PKBClauseData = dataAccessLayer->getClauseInverse(clause);
	return handleNextStarIntSynCombination(arg2, arg1, dataAccessLayer, PKBClauseData);
}

shared_ptr<QueryResultsTable> NextStarIntSyn::evaluate(shared_ptr<DataAccessLayer> dataAccessLayer, ABSTRACTION clause) {
	StringMap PKBClauseData = dataAccessLayer->getClause(clause);
	return handleNextStarIntSynCombination(arg1, arg2, dataAccessLayer, PKBClauseData);
}


// DFS, the idea is to start from the first integer, then traverse until we hit the other integer node and return an empty, but significant table
// If node is not found, return an empty table
shared_ptr<QueryResultsTable> NextStarIntInt::evaluate(shared_ptr<DataAccessLayer> dataAccessLayer, ABSTRACTION clause) {
	StringMap PKBClauseData = dataAccessLayer->getClause(clause);
	stack<NextStackElement> nextStack; // parent, set of childs
	unordered_set<string> visited;
	unordered_set<string> filteredPKBClauseDataArg1 = filterMapKeyReturnSetValues(arg1, dataAccessLayer, PKBClauseData);

	if (filteredPKBClauseDataArg1.empty()) {
		return QueryResultsTable::createEmptyTable();
	}
	nextStack.push(make_tuple(svToString(arg1->getArgValue()), filteredPKBClauseDataArg1));
	string target = svToString(arg2->getArgValue());
	while (!nextStack.empty())
	{
		NextStackElement curr = nextStack.top();
		nextStack.pop();
		string parent = get<0>(curr);
		visited.insert(parent);
		unordered_set<string> children = get<1>(curr);

		for (string child : children) {
			auto it = visited.find(child);
			if (it == visited.end()) {
				unordered_set<string> nextChildren = filterMapKeyReturnSetValues(child, dataAccessLayer, PKBClauseData);
				nextStack.push(make_tuple(child, nextChildren));
			}
			if (child == target) {
				shared_ptr<QueryResultsTable> table = QueryResultsTable::createEmptyTable();
				table->setSignificant(true);
				return table;
			}
		}
	}
	return QueryResultsTable::createEmptyTable();
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