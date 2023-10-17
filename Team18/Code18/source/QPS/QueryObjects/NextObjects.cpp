#include "ClauseObject.h"
#include <stack>
#include <tuple>

typedef tuple<string, unordered_set<string>> NextStackElement; // parent, set of childs

inline shared_ptr<QueryResultsTable> handleNext(shared_ptr<ClauseArg> arg1, shared_ptr<ClauseArg> arg2, shared_ptr<DataAccessLayer> dataAccessLayer, ABSTRACTION clause) {
	if (arg1->isSynonym() && arg2->isSynonym()) {
		if (arg1->getArgValue() == arg2->getArgValue()) {
			return make_shared<QueryResultsTable>();
		}
		StringMap PKBClauseData = dataAccessLayer->getClause(clause);

		StringMap filteredPKBClauseDataArg1 = filterMapKeyReturnMap(arg1, dataAccessLayer, PKBClauseData);
		StringMap filteredPKBClauseDataArg2 = filterMapValueReturnMap(arg2, dataAccessLayer, filteredPKBClauseDataArg1);

		vector<string> headers({ svToString(arg1->getArgValue()), svToString(arg2->getArgValue()) });
		return QueryResultsTable::createTable(headers, filteredPKBClauseDataArg2);
	}
	else if (arg1->isSynonym() && arg2->isWildcard()) {
		StringMap PKBClauseData = dataAccessLayer->getClause(clause);
		StringMap filteredPKBClauseDataArg1 = filterMapKeyReturnMap(arg1, dataAccessLayer, PKBClauseData);
		unordered_set<string> filteredPKBClauseDataArg2 = removeMapValuesReturnSet(arg2, dataAccessLayer, filteredPKBClauseDataArg1);
		return QueryResultsTable::createTable(svToString(arg1->getArgValue()), filteredPKBClauseDataArg2);
	}
	else if (arg1->isSynonym() && arg2->isInteger()) {
		StringMap PKBClauseData = dataAccessLayer->getClauseInverse(clause);
		unordered_set<string> filteredPKBClauseDataArg2 = filterMapKeyReturnSetValues(arg2, dataAccessLayer, PKBClauseData);
		unordered_set<string> filteredPKBClauseDataArg1 = filterSetReturnSet(arg1, dataAccessLayer, filteredPKBClauseDataArg2);
		return QueryResultsTable::createTable(svToString(arg1->getArgValue()), filteredPKBClauseDataArg1);
	}
	else if (arg1->isInteger() && arg2->isSynonym()) {
		StringMap PKBClauseData = dataAccessLayer->getClause(clause);
		unordered_set<string> filteredPKBClauseDataArg1 = filterMapKeyReturnSetValues(arg1, dataAccessLayer, PKBClauseData);
		unordered_set<string> filteredPKBClauseDataArg2 = filterSetReturnSet(arg2, dataAccessLayer, filteredPKBClauseDataArg1);
		return QueryResultsTable::createTable(svToString(arg2->getArgValue()), filteredPKBClauseDataArg2);
	}
	else if (arg1->isInteger() && arg2->isWildcard()) {
		StringMap PKBClauseData = dataAccessLayer->getClause(clause);

		unordered_set<string> filteredPKBClauseDataArg1 = filterMapKeyReturnSetValues(arg1, dataAccessLayer, PKBClauseData);
		shared_ptr<QueryResultsTable> table = make_shared<QueryResultsTable>();
		table->setSignificant(filteredPKBClauseDataArg1.size() > 0);
		return table;
	}
	else if (arg1->isInteger() && arg2->isInteger()) {
		StringMap PKBClauseData = dataAccessLayer->getClause(clause);

		unordered_set<string> filteredPKBClauseDataArg1 = filterMapKeyReturnSetValues(arg1, dataAccessLayer, PKBClauseData);
		bool filteredPKBClauseDataArg2 = filterSetReturnBool(arg2, dataAccessLayer, filteredPKBClauseDataArg1);
		shared_ptr<QueryResultsTable> table = make_shared<QueryResultsTable>();
		table->setSignificant(filteredPKBClauseDataArg2);
		return table;
	}
	else if (arg1->isWildcard() && arg2->isSynonym()) {
		StringMap PKBClauseData = dataAccessLayer->getClauseInverse(clause);
		StringMap filteredPKBClauseDataArg2 = filterMapKeyReturnMap(arg2, dataAccessLayer, PKBClauseData);
		unordered_set<string> filteredPKBClauseDataArg1 = removeMapValuesReturnSet(arg1, dataAccessLayer, filteredPKBClauseDataArg2);
		return QueryResultsTable::createTable(svToString(arg2->getArgValue()), filteredPKBClauseDataArg1);
	}
	else if (arg1->isWildcard() && arg2->isWildcard()) {
		StringMap PKBClauseData = dataAccessLayer->getClause(clause);
		shared_ptr<QueryResultsTable> table = make_shared<QueryResultsTable>();
		table->setSignificant(PKBClauseData.size() > 0);
		return table;
	}
	else if (arg1->isWildcard() && arg2->isInteger()) {
		StringMap PKBClauseData = dataAccessLayer->getClauseInverse(clause);
		unordered_set<string> filteredPKBClauseDataArg2 = filterMapKeyReturnSetValues(arg2, dataAccessLayer, PKBClauseData);
		shared_ptr<QueryResultsTable> table = make_shared<QueryResultsTable>();
		table->setSignificant(filteredPKBClauseDataArg2.size() > 0);
		return table;
	}

	throw SemanticErrorException("Error evaluating clause: NEXT");
}

inline shared_ptr<QueryResultsTable> handleNextStarIntInt(shared_ptr<ClauseArg> arg1, shared_ptr<ClauseArg> arg2, shared_ptr<DataAccessLayer> dataAccessLayer, ABSTRACTION clause) {
	StringMap PKBClauseData = dataAccessLayer->getClause(clause);
	stack<NextStackElement> nextStack;
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

inline shared_ptr<QueryResultsTable> handleNextStarIntSyn(shared_ptr<ClauseArg> arg1, shared_ptr<ClauseArg> arg2, shared_ptr<DataAccessLayer> dataAccessLayer, ABSTRACTION clause) {
	StringMap PKBClauseData = dataAccessLayer->getClause(clause);
	stack<NextStackElement> nextStack;
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
	unordered_set<string> filteredPKBClauseDataArg2 = filterSetReturnSet(arg2, dataAccessLayer, visited); // visited set is all the children nodes
	return QueryResultsTable::createTable(svToString(arg2->getArgValue()), filteredPKBClauseDataArg2);


}

inline shared_ptr<QueryResultsTable> handleNextStarSynInt(shared_ptr<ClauseArg> arg1, shared_ptr<ClauseArg> arg2, shared_ptr<DataAccessLayer> dataAccessLayer, ABSTRACTION clause) {
	StringMap PKBClauseData = dataAccessLayer->getClauseInverse(clause);
	stack<NextStackElement> nextStack;
	unordered_set<string> visited;
	unordered_set<string> filteredPKBClauseDataArg1 = filterMapKeyReturnSetValues(arg2, dataAccessLayer, PKBClauseData);

	if (filteredPKBClauseDataArg1.empty()) {
		return QueryResultsTable::createTable(svToString(arg1->getArgValue()), visited);
	}
	nextStack.push(make_tuple(svToString(arg2->getArgValue()), filteredPKBClauseDataArg1));

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
	unordered_set<string> filteredPKBClauseDataArg2 = filterSetReturnSet(arg1, dataAccessLayer, visited); // visited set is all the children nodes
	return QueryResultsTable::createTable(svToString(arg1->getArgValue()), filteredPKBClauseDataArg2);


}

// DFS from each node? large runtime

inline shared_ptr<QueryResultsTable> handleNextStarSynSyn(shared_ptr<ClauseArg> arg1, shared_ptr<ClauseArg> arg2, shared_ptr<DataAccessLayer> dataAccessLayer, ABSTRACTION clause) {
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
	
	StringMap nextStarTable;
	for (string parent : filteredPKBClauseDataKeepArg1) {
		stack<NextStackElement> nextStack;
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
				if (arg2It != filteredPKBClauseDataKeepArg2.end()) {
					resultSet.insert(child);
				}

			}
		}
		if (resultSet.size() > 0) {
			if (arg1->getArgValue() == arg2->getArgValue()) { // edge case
				auto it = resultSet.find(parent);
				if (it != resultSet.end()) {
					unordered_set<string> arg2Set = {parent};
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


// BEWARE OF LOOPS
// Traverse map using depth first search, and keep a visited set, helper methods are for those that require DFS
inline shared_ptr<QueryResultsTable> handleNextStar(shared_ptr<ClauseArg> arg1, shared_ptr<ClauseArg> arg2, shared_ptr<DataAccessLayer> dataAccessLayer, ABSTRACTION clause) {
	if (arg1->isSynonym() && arg2->isSynonym()) {
		return handleNextStarSynSyn(arg1, arg2, dataAccessLayer, clause);
	}
	else if (arg1->isSynonym() && arg2->isWildcard()) { 
		// same as next?? possible logical error, but next(s, _) seems to == next*(s, _)
		StringMap PKBClauseData = dataAccessLayer->getClause(clause);
		StringMap filteredPKBClauseDataArg1 = filterMapKeyReturnMap(arg1, dataAccessLayer, PKBClauseData);
		unordered_set<string> filteredPKBClauseDataArg2 = removeMapValuesReturnSet(arg2, dataAccessLayer, filteredPKBClauseDataArg1);
		return QueryResultsTable::createTable(svToString(arg1->getArgValue()), filteredPKBClauseDataArg2);
	}
	else if (arg1->isSynonym() && arg2->isInteger()) {
		return handleNextStarSynInt(arg1, arg2, dataAccessLayer, clause);
	}
	else if (arg1->isInteger() && arg2->isSynonym()) {
		return handleNextStarIntSyn(arg1, arg2, dataAccessLayer, clause);
	}
	else if (arg1->isInteger() && arg2->isWildcard()) {
		StringMap PKBClauseData = dataAccessLayer->getClause(clause);

		unordered_set<string> filteredPKBClauseDataArg1 = filterMapKeyReturnSetValues(arg1, dataAccessLayer, PKBClauseData);
		shared_ptr<QueryResultsTable> table = make_shared<QueryResultsTable>();
		table->setSignificant(filteredPKBClauseDataArg1.size() > 0);
		return table;
	}
	else if (arg1->isInteger() && arg2->isInteger()) { 
		return handleNextStarIntInt(arg1, arg2, dataAccessLayer, clause);

	}
	else if (arg1->isWildcard() && arg2->isSynonym()) {
		// same as next?? possible logical error, but next(_, s) seems to == next*(_, s)
		StringMap PKBClauseData = dataAccessLayer->getClauseInverse(clause);
		StringMap filteredPKBClauseDataArg2 = filterMapKeyReturnMap(arg2, dataAccessLayer, PKBClauseData);
		unordered_set<string> filteredPKBClauseDataArg1 = removeMapValuesReturnSet(arg1, dataAccessLayer, filteredPKBClauseDataArg2);
		return QueryResultsTable::createTable(svToString(arg2->getArgValue()), filteredPKBClauseDataArg1);
	}
	else if (arg1->isWildcard() && arg2->isWildcard()) {
		StringMap PKBClauseData = dataAccessLayer->getClause(clause);
		shared_ptr<QueryResultsTable> table = QueryResultsTable::createEmptyTable();
		table->setSignificant(PKBClauseData.size() > 0);
		return table;
	}
	else if (arg1->isWildcard() && arg2->isInteger()) {
		StringMap PKBClauseData = dataAccessLayer->getClauseInverse(clause);
		unordered_set<string> filteredPKBClauseDataArg2 = filterMapKeyReturnSetValues(arg2, dataAccessLayer, PKBClauseData);
		shared_ptr<QueryResultsTable> table = make_shared<QueryResultsTable>();
		table->setSignificant(filteredPKBClauseDataArg2.size() > 0);
		return table;
	}

	throw SemanticErrorException("Error evaluating clause: NEXTSTAR");
}

shared_ptr<QueryResultsTable> NextObject::callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer) {
	return handleNext(getArg1(), getArg2(), dataAccessLayer, NEXT);
}

shared_ptr<QueryResultsTable> NextStarObject::callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer) {
	return handleNextStar(getArg1(), getArg2(), dataAccessLayer, NEXT);
}
