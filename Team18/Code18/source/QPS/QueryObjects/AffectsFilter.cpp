#include "AffectsFilter.h"
#include "ClauseTableFilter.h"

typedef tuple<string, unordered_set<string>> AffectsStackElement; // parent, set of childs

// returns element in an unordered set, use only if set has one element, else random element from set is returned
static string returnSingleElementFromSet(unordered_set<string> stringSet) {
	if (stringSet.size() <= 0) {
		return "";
	}
	auto it = stringSet.begin();
	string onlyElement = *it;
	return onlyElement;
}

unordered_set<string> AffectsFilter::filterAssignmentsInProcedure(unordered_set<string> assignments, int target, shared_ptr<DataAccessLayer> dataAccessLayer) {
	unordered_set<string> procedures = dataAccessLayer->getAllProcedures();
	// first determine the procedure that the target lies in
	string targetProcedure;
	bool procedureExists = false;
	for (string procedure : procedures) {
		pair<string, string> range = dataAccessLayer->getProcLines(procedure);
		int startInt = stoi(range.first);
		int endInt = stoi(range.second);
		if (startInt <= target && target <= endInt) {
			targetProcedure = procedure;
			procedureExists = true;
			break;
		}
	}
	if (!procedureExists) {
		return {};
	}
	pair<string, string> targetRange = dataAccessLayer->getProcLines(targetProcedure);
	int startRange = stoi(targetRange.first);
	int endRange = stoi(targetRange.second);
	unordered_set<string> filteredAssignments;
	for (string assignment : assignments) {
		int assignmentInt = stoi(assignment);
		if (startRange <= assignmentInt && assignmentInt <= endRange) {
			filteredAssignments.insert(assignment);
		}
	}
	return filteredAssignments;
}

/*
* Strictly assume PKB data is correct
* In general, affects logic is as follows:
* Start from an assignment statement (all assignments are modifies / uses, but the converse is not true like read / print/ call)
* Save the variable it modifies
* DFS from this assignment statement, to the other uses statements.
* If variable is modified along the way: terminate, return false
* If uses found for that variable, return true
*/
shared_ptr<QueryResultsTable> AffectsSynSyn::evaluate(shared_ptr<DataAccessLayer> dataAccessLayer, ABSTRACTION clause) {
	StringMap cfg = dataAccessLayer->getClause(clause); // next table (CFG)
	unordered_set<string> assignments = dataAccessLayer->getEntity(ASSIGN);
	StringMap results;

	for (string assignment : assignments) {
		string startingLine = assignment;
		unordered_set<string> childrenOfStartNode = filterMapKeyReturnSetValues(startingLine, dataAccessLayer, cfg);
		StringMap usesMap = dataAccessLayer->getClause(USES);
		StringMap modifiesMap = dataAccessLayer->getClause(MODIFIES);
		string modifiedVar = returnSingleElementFromSet(modifiesMap[startingLine]); // get variable modified
		StringMap usesInverseMap = dataAccessLayer->getClauseInverse(USES);

		// if no cfg, variable is not used in prog
		if (childrenOfStartNode.empty()
			|| !containerHasKey(usesInverseMap, modifiedVar)) {
			continue;
		}

		stack<AffectsStackElement> DFSStack;
		unordered_set<string> visited;

		DFSStack.push(make_tuple(startingLine, childrenOfStartNode));

		while (!DFSStack.empty()) {
			AffectsStackElement curr = DFSStack.top();
			DFSStack.pop();
			string parent = get<0>(curr);
			unordered_set<string> children = get<1>(curr);
			visited.insert(parent);

			// modified before reaching target, do not traverse further down that branch
			if (parent != startingLine
				&& containerHasKey(assignments, parent) && containerHasKey(modifiesMap[parent], modifiedVar)) {
				continue; // cannot break, as there may exist another path that does not modifies
			}
			for (string child : children) {
				auto it = visited.find(child);
				if (it == visited.end()) {
					unordered_set<string> nextChildren = filterMapKeyReturnSetValues(child, dataAccessLayer, cfg);
					DFSStack.push(make_tuple(child, nextChildren));
				}
				unordered_set<string> uses = usesMap[child]; // variables used at child node
				if (containerHasKey(assignments, child) && containerHasKey(uses, modifiedVar)) { // is a valid affects
					// if affects(a, a)
					if (arg1->getArgValue() == arg2->getArgValue()) {
						if (assignment == child) {
							results[assignment].insert(child);
						}
					}
					else {
						results[assignment].insert(child);
					}

				}
			}

		}
	}
	if (arg1->getArgValue() == arg2->getArgValue()) {
		return QueryResultsTable::createTable(svToString(arg1->getArgValue()), 
			getMapKeys(results));
	}
	vector<string> headers({ svToString(arg1->getArgValue()), svToString(arg2->getArgValue()) });
	return QueryResultsTable::createTable(headers, results);
}

shared_ptr<QueryResultsTable> AffectsSynWildcard::evaluate(shared_ptr<DataAccessLayer> dataAccessLayer, ABSTRACTION clause) {
	StringMap cfg = dataAccessLayer->getClause(clause); // next table (CFG)
	unordered_set<string> assignments = dataAccessLayer->getEntity(ASSIGN);
	unordered_set<string> results;

	for (string assignment : assignments) {
		string startingLine = assignment;
		unordered_set<string> childrenOfStartNode = filterMapKeyReturnSetValues(startingLine, dataAccessLayer, cfg);
		StringMap usesMap = dataAccessLayer->getClause(USES);
		StringMap modifiesMap = dataAccessLayer->getClause(MODIFIES);
		string modifiedVar = returnSingleElementFromSet(modifiesMap[startingLine]); // get variable modified
		StringMap usesInverseMap = dataAccessLayer->getClauseInverse(USES);

		// if no cfg, variable is not used in prog
		if (childrenOfStartNode.empty()
			|| !containerHasKey(usesInverseMap, modifiedVar)) {
			continue;
		}

		stack<AffectsStackElement> DFSStack;
		unordered_set<string> visited;

		DFSStack.push(make_tuple(startingLine, childrenOfStartNode));

		while (!DFSStack.empty()) {
			AffectsStackElement curr = DFSStack.top();
			DFSStack.pop();
			string parent = get<0>(curr);
			unordered_set<string> children = get<1>(curr);
			visited.insert(parent);

			// modified before reaching target, do not traverse further down that branch
			if (parent != startingLine
				&& containerHasKey(assignments, parent) && containerHasKey(modifiesMap[parent], modifiedVar)) {
				continue; // cannot break, as there may exist another path that does not modifies
			}
			for (string child : children) {
				auto it = visited.find(child);
				if (it == visited.end()) {
					unordered_set<string> nextChildren = filterMapKeyReturnSetValues(child, dataAccessLayer, cfg);
					DFSStack.push(make_tuple(child, nextChildren));
				}
				unordered_set<string> uses = usesMap[child]; // variables used at child node
				if (containerHasKey(assignments, child) && containerHasKey(uses, modifiedVar)) { // is a valid affects
					results.insert(assignment);
					break; // no need to check further
				}
			}

		}
	}

	return QueryResultsTable::createTable(svToString(arg1->getArgValue()), results);
}


shared_ptr<QueryResultsTable> AffectsSynInt::evaluate(shared_ptr<DataAccessLayer> dataAccessLayer, ABSTRACTION clause) {
	StringMap cfg = dataAccessLayer->getClause(clause); // next table (CFG)
	unordered_set<string> unfilteredassignments = dataAccessLayer->getEntity(ASSIGN);
	unordered_set<string> results;
	unordered_set<string> assignments = filterAssignmentsInProcedure(unfilteredassignments, 
		stoi(svToString(arg2->getArgValue())), dataAccessLayer);
	for (string assignment : assignments) {
		string startingLine = assignment;
		unordered_set<string> childrenOfStartNode = filterMapKeyReturnSetValues(startingLine, dataAccessLayer, cfg);
		StringMap usesMap = dataAccessLayer->getClause(USES);
		StringMap modifiesMap = dataAccessLayer->getClause(MODIFIES);
		string modifiedVar = returnSingleElementFromSet(modifiesMap[startingLine]); // get variable modified
		StringMap usesInverseMap = dataAccessLayer->getClauseInverse(USES);

		// if no cfg, variable is not used in prog
		if (childrenOfStartNode.empty()
			|| !containerHasKey(usesInverseMap, modifiedVar)) {
			continue;
		}

		stack<AffectsStackElement> DFSStack;
		unordered_set<string> visited;
		
		DFSStack.push(make_tuple(startingLine, childrenOfStartNode));

		while (!DFSStack.empty()) {
			AffectsStackElement curr = DFSStack.top();
			DFSStack.pop();
			string parent = get<0>(curr);
			unordered_set<string> children = get<1>(curr);
			visited.insert(parent);

			// modified before reaching target, do not traverse further down that branch
			if (parent != startingLine
				&& containerHasKey(assignments, parent) && containerHasKey(modifiesMap[parent], modifiedVar)) {
				continue; // cannot break, as there may exist another path that does not modifies
			}
			for (string child : children) {
				auto it = visited.find(child);
				if (it == visited.end()) {
					unordered_set<string> nextChildren = filterMapKeyReturnSetValues(child, dataAccessLayer, cfg);
					DFSStack.push(make_tuple(child, nextChildren));
				}
				unordered_set<string> uses = usesMap[child]; // variables used at child node
				if (containerHasKey(assignments, child) && containerHasKey(uses, modifiedVar) 
					&& child == svToString(arg2->getArgValue())) { // is a valid affects
					results.insert(assignment);
					break; // no need to check further
				}
			}

		}
	}

	
	return QueryResultsTable::createTable(svToString(arg1->getArgValue()), results);
}

shared_ptr<QueryResultsTable> AffectsIntSyn::evaluate(shared_ptr<DataAccessLayer> dataAccessLayer, ABSTRACTION clause) {
	StringMap cfg = dataAccessLayer->getClause(clause); // next table (CFG)
	unordered_set<string> assignments = dataAccessLayer->getEntity(ASSIGN);

	unordered_set<string> childrenOfStartNode = filterMapKeyReturnSetValues(arg1, dataAccessLayer, cfg);
	string startingLine = svToString(arg1->getArgValue());
	StringMap usesMap = dataAccessLayer->getClause(USES);
	StringMap modifiesMap = dataAccessLayer->getClause(MODIFIES);
	string modifiedVar = returnSingleElementFromSet(modifiesMap[startingLine]); // get variable modified
	StringMap usesInverseMap = dataAccessLayer->getClauseInverse(USES);
	
	// if no cfg, start node is not assignment, variable is not used in program
	if (childrenOfStartNode.empty() || !containerHasKey(assignments, startingLine) 
		||!containerHasKey(usesInverseMap, modifiedVar)) {
		return QueryResultsTable::createEmptyTableWithHeaders({ svToString(arg2->getArgValue()) });
	}

	stack<AffectsStackElement> DFSStack;
	unordered_set<string> visited;
	unordered_set<string> results;
	DFSStack.push(make_tuple(startingLine, childrenOfStartNode));

	while (!DFSStack.empty()) {
		AffectsStackElement curr = DFSStack.top();
		DFSStack.pop();
		string parent = get<0>(curr);
		unordered_set<string> children = get<1>(curr);
		visited.insert(parent);

		// modified before reaching target, do not traverse further down that branch
		if (parent != startingLine
			&& containerHasKey(assignments, parent) && containerHasKey(modifiesMap[parent], modifiedVar)) {
			continue; // cannot break, as there may exist another path that does not modifies
		}
		for (string child : children) {
			auto it = visited.find(child);
			if (it == visited.end()) {
				unordered_set<string> nextChildren = filterMapKeyReturnSetValues(child, dataAccessLayer, cfg);
				DFSStack.push(make_tuple(child, nextChildren));
			}
			unordered_set<string> uses = usesMap[child]; // variables used at child node
			if (containerHasKey(assignments, child) && containerHasKey(uses, modifiedVar)) { // is a valid affects
				results.insert(child);
			}
		}
	}

	return QueryResultsTable::createTable(svToString(arg2->getArgValue()), results);
}

shared_ptr<QueryResultsTable> AffectsIntWildcard::evaluate(shared_ptr<DataAccessLayer> dataAccessLayer, ABSTRACTION clause) {
	StringMap cfg = dataAccessLayer->getClause(clause); // next table (CFG)
	unordered_set<string> assignments = dataAccessLayer->getEntity(ASSIGN);

	unordered_set<string> childrenOfStartNode = filterMapKeyReturnSetValues(arg1, dataAccessLayer, cfg);
	string startingLine = svToString(arg1->getArgValue());
	StringMap usesMap = dataAccessLayer->getClause(USES);
	StringMap modifiesMap = dataAccessLayer->getClause(MODIFIES);
	string modifiedVar = returnSingleElementFromSet(modifiesMap[startingLine]); // get variable modified
	StringMap usesInverseMap = dataAccessLayer->getClauseInverse(USES);

	// if no cfg, start node is not assignment, variable is not used in program
	if (childrenOfStartNode.empty() || !containerHasKey(assignments, startingLine)
		|| !containerHasKey(usesInverseMap, modifiedVar)) {
		return QueryResultsTable::createEmptyTable();
	}

	stack<AffectsStackElement> DFSStack;
	unordered_set<string> visited;
	unordered_set<string> results;
	DFSStack.push(make_tuple(startingLine, childrenOfStartNode));

	while (!DFSStack.empty()) {
		AffectsStackElement curr = DFSStack.top();
		DFSStack.pop();
		string parent = get<0>(curr);
		unordered_set<string> children = get<1>(curr);
		visited.insert(parent);

		// modified before reaching target, do not traverse further down that branch
		if (parent != startingLine
			&& containerHasKey(assignments, parent) && containerHasKey(modifiesMap[parent], modifiedVar)) {
			continue; // cannot break, as there may exist another path that does not modifies
		}
		for (string child : children) {
			auto it = visited.find(child);
			if (it == visited.end()) {
				unordered_set<string> nextChildren = filterMapKeyReturnSetValues(child, dataAccessLayer, cfg);
				DFSStack.push(make_tuple(child, nextChildren));
			}
			unordered_set<string> uses = usesMap[child]; // variables used at child node
			if (containerHasKey(assignments, child) && containerHasKey(uses, modifiedVar)) { // is a valid affects
				shared_ptr<QueryResultsTable> table = QueryResultsTable::createEmptyTable(true);
				return table;
			}
		}
	}

	return QueryResultsTable::createEmptyTable();
}

shared_ptr<QueryResultsTable> AffectsIntInt::evaluate(shared_ptr<DataAccessLayer> dataAccessLayer, ABSTRACTION clause) {
	StringMap cfg = dataAccessLayer->getClause(clause); // next table (CFG)
	unordered_set<string> assignments = dataAccessLayer->getEntity(ASSIGN);
	
	unordered_set<string> childrenOfStartNode = filterMapKeyReturnSetValues(arg1, dataAccessLayer, cfg);
	string startingLine = svToString(arg1->getArgValue());
	string finishLine = svToString(arg2->getArgValue());
	StringMap usesMap = dataAccessLayer->getClause(USES);
	StringMap modifiesMap = dataAccessLayer->getClause(MODIFIES);
	string modifiedVar = returnSingleElementFromSet(modifiesMap[startingLine]); // get variable modified
	unordered_set<string> uses = usesMap[finishLine]; // variables used at last line

	// if no cfg, start node is not assignment, end node not assignment, uses does not use the var
	if (childrenOfStartNode.empty() || !containerHasKey(assignments, startingLine) || !containerHasKey(assignments, finishLine)
		|| !containerHasKey(uses, modifiedVar)) {
		return QueryResultsTable::createEmptyTable();
	}
	
	stack<AffectsStackElement> DFSStack;
	unordered_set<string> visited;
	DFSStack.push(make_tuple(startingLine, childrenOfStartNode));

	while (!DFSStack.empty()) {
		AffectsStackElement curr = DFSStack.top();
		DFSStack.pop();
		string parent = get<0>(curr);
		unordered_set<string> children = get<1>(curr);
		visited.insert(parent);

		// modified before reaching target, do not traverse further down that branch
		if (parent != finishLine && parent != startingLine 
			&& containerHasKey(assignments, parent) && containerHasKey(modifiesMap[parent], modifiedVar)) {
			continue; // cannot break, as there may exist another path that does not modifies
		}
		for (string child : children) {
			auto it = visited.find(child);
			if (it == visited.end()) {
				unordered_set<string> nextChildren = filterMapKeyReturnSetValues(child, dataAccessLayer, cfg);
				DFSStack.push(make_tuple(child, nextChildren));
			}
			if (child == finishLine) { // at target node
				shared_ptr<QueryResultsTable> table = QueryResultsTable::createEmptyTable(true);
				return table;
			}
		}
	}

	return QueryResultsTable::createEmptyTable();
}

shared_ptr<QueryResultsTable> AffectsWildcardSyn::evaluate(shared_ptr<DataAccessLayer> dataAccessLayer, ABSTRACTION clause) {
	StringMap cfg = dataAccessLayer->getClause(clause); // next table (CFG)
	unordered_set<string> assignments = dataAccessLayer->getEntity(ASSIGN);
	unordered_set<string> results;

	for (string assignment : assignments) {
		string startingLine = assignment;
		unordered_set<string> childrenOfStartNode = filterMapKeyReturnSetValues(startingLine, dataAccessLayer, cfg);
		StringMap usesMap = dataAccessLayer->getClause(USES);
		StringMap modifiesMap = dataAccessLayer->getClause(MODIFIES);
		string modifiedVar = returnSingleElementFromSet(modifiesMap[startingLine]); // get variable modified
		StringMap usesInverseMap = dataAccessLayer->getClauseInverse(USES);

		// if no cfg, variable is not used in prog
		if (childrenOfStartNode.empty()
			|| !containerHasKey(usesInverseMap, modifiedVar)) {
			continue;
		}

		stack<AffectsStackElement> DFSStack;
		unordered_set<string> visited;

		DFSStack.push(make_tuple(startingLine, childrenOfStartNode));

		while (!DFSStack.empty()) {
			AffectsStackElement curr = DFSStack.top();
			DFSStack.pop();
			string parent = get<0>(curr);
			unordered_set<string> children = get<1>(curr);
			visited.insert(parent);

			// modified before reaching target, do not traverse further down that branch
			if (parent != startingLine
				&& containerHasKey(assignments, parent) && containerHasKey(modifiesMap[parent], modifiedVar)) {
				continue; // cannot break, as there may exist another path that does not modifies
			}
			for (string child : children) {
				auto it = visited.find(child);
				if (it == visited.end()) {
					unordered_set<string> nextChildren = filterMapKeyReturnSetValues(child, dataAccessLayer, cfg);
					DFSStack.push(make_tuple(child, nextChildren));
				}
				unordered_set<string> uses = usesMap[child]; // variables used at child node
				if (containerHasKey(assignments, child) && containerHasKey(uses, modifiedVar)) { // is a valid affects
					results.insert(child);
					break; // i think its ok to not check further here
				}
			}

		}
	}

	return QueryResultsTable::createTable(svToString(arg2->getArgValue()), results);
}

shared_ptr<QueryResultsTable> AffectsWildcardWildcard::evaluate(shared_ptr<DataAccessLayer> dataAccessLayer, ABSTRACTION clause) {
	StringMap cfg = dataAccessLayer->getClause(clause); // next table (CFG)
	unordered_set<string> assignments = dataAccessLayer->getEntity(ASSIGN);

	for (string assignment : assignments) {
		string startingLine = assignment;
		unordered_set<string> childrenOfStartNode = filterMapKeyReturnSetValues(startingLine, dataAccessLayer, cfg);
		StringMap usesMap = dataAccessLayer->getClause(USES);
		StringMap modifiesMap = dataAccessLayer->getClause(MODIFIES);
		string modifiedVar = returnSingleElementFromSet(modifiesMap[startingLine]); // get variable modified
		StringMap usesInverseMap = dataAccessLayer->getClauseInverse(USES);

		// if no cfg, variable is not used in prog
		if (childrenOfStartNode.empty()
			|| !containerHasKey(usesInverseMap, modifiedVar)) {
			continue;
		}

		stack<AffectsStackElement> DFSStack;
		unordered_set<string> visited;

		DFSStack.push(make_tuple(startingLine, childrenOfStartNode));

		while (!DFSStack.empty()) {
			AffectsStackElement curr = DFSStack.top();
			DFSStack.pop();
			string parent = get<0>(curr);
			unordered_set<string> children = get<1>(curr);
			visited.insert(parent);

			// modified before reaching target, do not traverse further down that branch
			if (parent != startingLine
				&& containerHasKey(assignments, parent) && containerHasKey(modifiesMap[parent], modifiedVar)) {
				continue; // cannot break, as there may exist another path that does not modifies
			}
			for (string child : children) {
				auto it = visited.find(child);
				if (it == visited.end()) {
					unordered_set<string> nextChildren = filterMapKeyReturnSetValues(child, dataAccessLayer, cfg);
					DFSStack.push(make_tuple(child, nextChildren));
				}
				unordered_set<string> uses = usesMap[child]; // variables used at child node
				if (containerHasKey(assignments, child) && containerHasKey(uses, modifiedVar)) { // is a valid affects
					shared_ptr<QueryResultsTable> table = QueryResultsTable::createEmptyTable(true);
					return table;
				}
			}

		}
	}

	return QueryResultsTable::createEmptyTable();
}

shared_ptr<QueryResultsTable> AffectsWildcardInt::evaluate(shared_ptr<DataAccessLayer> dataAccessLayer, ABSTRACTION clause) {
	StringMap cfg = dataAccessLayer->getClause(clause); // next table (CFG)
	unordered_set<string> assignments = dataAccessLayer->getEntity(ASSIGN);

	for (string assignment : assignments) {
		string startingLine = assignment;
		unordered_set<string> childrenOfStartNode = filterMapKeyReturnSetValues(startingLine, dataAccessLayer, cfg);
		StringMap usesMap = dataAccessLayer->getClause(USES);
		StringMap modifiesMap = dataAccessLayer->getClause(MODIFIES);
		string modifiedVar = returnSingleElementFromSet(modifiesMap[startingLine]); // get variable modified
		StringMap usesInverseMap = dataAccessLayer->getClauseInverse(USES);

		// if no cfg, variable is not used in prog
		if (childrenOfStartNode.empty()
			|| !containerHasKey(usesInverseMap, modifiedVar)) {
			continue;
		}

		stack<AffectsStackElement> DFSStack;
		unordered_set<string> visited;

		DFSStack.push(make_tuple(startingLine, childrenOfStartNode));

		while (!DFSStack.empty()) {
			AffectsStackElement curr = DFSStack.top();
			DFSStack.pop();
			string parent = get<0>(curr);
			unordered_set<string> children = get<1>(curr);
			visited.insert(parent);

			// modified before reaching target, do not traverse further down that branch
			if (parent != startingLine
				&& containerHasKey(assignments, parent) && containerHasKey(modifiesMap[parent], modifiedVar)) {
				continue; // cannot break, as there may exist another path that does not modifies
			}
			for (string child : children) {
				auto it = visited.find(child);
				if (it == visited.end()) {
					unordered_set<string> nextChildren = filterMapKeyReturnSetValues(child, dataAccessLayer, cfg);
					DFSStack.push(make_tuple(child, nextChildren));
				}
				unordered_set<string> uses = usesMap[child]; // variables used at child node
				if (containerHasKey(assignments, child) && containerHasKey(uses, modifiedVar) 
					&& child == svToString(arg2->getArgValue())) { // is a valid affects
					shared_ptr<QueryResultsTable> table = QueryResultsTable::createEmptyTable(true);
					return table;
				}
			}

		}
	}

	return QueryResultsTable::createEmptyTable();
}

shared_ptr<QueryEval> AffectsFilterFactory::create(shared_ptr<ClauseArg> argument1, shared_ptr<ClauseArg> argument2) {
	if (argument1->isSynonym() && argument2->isSynonym()) {
		return make_shared<AffectsSynSyn>(argument1, argument2);
	}
	else if (argument1->isSynonym() && argument2->isWildcard()) {
		return make_shared<AffectsSynWildcard>(argument1, argument2);
	}
	else if (argument1->isSynonym() && argument2->isInteger()) {
		return make_shared<AffectsSynInt>(argument1, argument2);
	}
	else if (argument1->isInteger() && argument2->isSynonym()) {
		return make_shared<AffectsIntSyn>(argument1, argument2);
	}
	else if (argument1->isInteger() && argument2->isWildcard()) {
		return make_shared<AffectsIntWildcard>(argument1, argument2);
	}
	else if (argument1->isInteger() && argument2->isInteger()) {
		return make_shared<AffectsIntInt>(argument1, argument2);
	}
	else if (argument1->isWildcard() && argument2->isSynonym()) {
		return make_shared<AffectsWildcardSyn>(argument1, argument2);
	}
	else if (argument1->isWildcard() && argument2->isWildcard()) {
		return make_shared<AffectsWildcardWildcard>(argument1, argument2);
	}
	else if (argument1->isWildcard() && argument2->isInteger()) {
		return make_shared<AffectsWildcardInt>(argument1, argument2);
	}
	throw SemanticErrorException("Error in affects filter factory, invalid argument combination");
}