#include "AffectsFilter.h"
#include "ClauseTableFilter.h"

typedef tuple<string, unordered_set<string>> AffectsStackElement; // parent, set of childs

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
			|| !mapHasString(usesInverseMap, modifiedVar)) {
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
				&& setHasString(assignments, parent) && setHasString(modifiesMap[parent], modifiedVar)) {
				continue; // cannot break, as there may exist another path that does not modifies
			}
			for (string child : children) {
				auto it = visited.find(child);
				if (it == visited.end()) {
					unordered_set<string> nextChildren = filterMapKeyReturnSetValues(child, dataAccessLayer, cfg);
					DFSStack.push(make_tuple(child, nextChildren));
				}
				unordered_set<string> uses = usesMap[child]; // variables used at child node
				if (setHasString(assignments, child) && setHasString(uses, modifiedVar)) { // is a valid affects
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
			|| !mapHasString(usesInverseMap, modifiedVar)) {
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
				&& setHasString(assignments, parent) && setHasString(modifiesMap[parent], modifiedVar)) {
				continue; // cannot break, as there may exist another path that does not modifies
			}
			for (string child : children) {
				auto it = visited.find(child);
				if (it == visited.end()) {
					unordered_set<string> nextChildren = filterMapKeyReturnSetValues(child, dataAccessLayer, cfg);
					DFSStack.push(make_tuple(child, nextChildren));
				}
				unordered_set<string> uses = usesMap[child]; // variables used at child node
				if (setHasString(assignments, child) && setHasString(uses, modifiedVar)) { // is a valid affects
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
	unordered_set<string> assignments = dataAccessLayer->getEntity(ASSIGN);
	unordered_set<string> results;
	// TODO: filter assignments in the same procedure as int, in the future
	for (string assignment : assignments) {
		string startingLine = assignment;
		unordered_set<string> childrenOfStartNode = filterMapKeyReturnSetValues(startingLine, dataAccessLayer, cfg);
		StringMap usesMap = dataAccessLayer->getClause(USES);
		StringMap modifiesMap = dataAccessLayer->getClause(MODIFIES);
		string modifiedVar = returnSingleElementFromSet(modifiesMap[startingLine]); // get variable modified
		StringMap usesInverseMap = dataAccessLayer->getClauseInverse(USES);

		// if no cfg, variable is not used in prog
		if (childrenOfStartNode.empty()
			|| !mapHasString(usesInverseMap, modifiedVar)) {
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
				&& setHasString(assignments, parent) && setHasString(modifiesMap[parent], modifiedVar)) {
				continue; // cannot break, as there may exist another path that does not modifies
			}
			for (string child : children) {
				auto it = visited.find(child);
				if (it == visited.end()) {
					unordered_set<string> nextChildren = filterMapKeyReturnSetValues(child, dataAccessLayer, cfg);
					DFSStack.push(make_tuple(child, nextChildren));
				}
				unordered_set<string> uses = usesMap[child]; // variables used at child node
				if (setHasString(assignments, child) && setHasString(uses, modifiedVar) 
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
	if (childrenOfStartNode.empty() || !setHasString(assignments, startingLine) 
		||!mapHasString(usesInverseMap, modifiedVar)) {
		return QueryResultsTable::createEmptyTableWithHeader(svToString(arg2->getArgValue()));
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
			&& setHasString(assignments, parent) && setHasString(modifiesMap[parent], modifiedVar)) {
			continue; // cannot break, as there may exist another path that does not modifies
		}
		for (string child : children) {
			auto it = visited.find(child);
			if (it == visited.end()) {
				unordered_set<string> nextChildren = filterMapKeyReturnSetValues(child, dataAccessLayer, cfg);
				DFSStack.push(make_tuple(child, nextChildren));
			}
			unordered_set<string> uses = usesMap[child]; // variables used at child node
			if (setHasString(assignments, child) && setHasString(uses, modifiedVar)) { // is a valid affects
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
	if (childrenOfStartNode.empty() || !setHasString(assignments, startingLine)
		|| !mapHasString(usesInverseMap, modifiedVar)) {
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
			&& setHasString(assignments, parent) && setHasString(modifiesMap[parent], modifiedVar)) {
			continue; // cannot break, as there may exist another path that does not modifies
		}
		for (string child : children) {
			auto it = visited.find(child);
			if (it == visited.end()) {
				unordered_set<string> nextChildren = filterMapKeyReturnSetValues(child, dataAccessLayer, cfg);
				DFSStack.push(make_tuple(child, nextChildren));
			}
			unordered_set<string> uses = usesMap[child]; // variables used at child node
			if (setHasString(assignments, child) && setHasString(uses, modifiedVar)) { // is a valid affects
				shared_ptr<QueryResultsTable> table = QueryResultsTable::createEmptyTable();
				table->setSignificant(true);
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
	if (childrenOfStartNode.empty() || !setHasString(assignments, startingLine) || !setHasString(assignments, finishLine)
		|| !setHasString(uses, modifiedVar)) {
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
			&& setHasString(assignments, parent) && setHasString(modifiesMap[parent], modifiedVar)) {
			continue; // cannot break, as there may exist another path that does not modifies
		}
		for (string child : children) {
			auto it = visited.find(child);
			if (it == visited.end()) {
				unordered_set<string> nextChildren = filterMapKeyReturnSetValues(child, dataAccessLayer, cfg);
				DFSStack.push(make_tuple(child, nextChildren));
			}
			if (child == finishLine) { // at target node
				shared_ptr<QueryResultsTable> table = QueryResultsTable::createEmptyTable();
				table->setSignificant(true);
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
			|| !mapHasString(usesInverseMap, modifiedVar)) {
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
				&& setHasString(assignments, parent) && setHasString(modifiesMap[parent], modifiedVar)) {
				continue; // cannot break, as there may exist another path that does not modifies
			}
			for (string child : children) {
				auto it = visited.find(child);
				if (it == visited.end()) {
					unordered_set<string> nextChildren = filterMapKeyReturnSetValues(child, dataAccessLayer, cfg);
					DFSStack.push(make_tuple(child, nextChildren));
				}
				unordered_set<string> uses = usesMap[child]; // variables used at child node
				if (setHasString(assignments, child) && setHasString(uses, modifiedVar)) { // is a valid affects
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
			|| !mapHasString(usesInverseMap, modifiedVar)) {
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
				&& setHasString(assignments, parent) && setHasString(modifiesMap[parent], modifiedVar)) {
				continue; // cannot break, as there may exist another path that does not modifies
			}
			for (string child : children) {
				auto it = visited.find(child);
				if (it == visited.end()) {
					unordered_set<string> nextChildren = filterMapKeyReturnSetValues(child, dataAccessLayer, cfg);
					DFSStack.push(make_tuple(child, nextChildren));
				}
				unordered_set<string> uses = usesMap[child]; // variables used at child node
				if (setHasString(assignments, child) && setHasString(uses, modifiedVar)) { // is a valid affects
					shared_ptr<QueryResultsTable> table = QueryResultsTable::createEmptyTable();
					table->setSignificant(true);
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
			|| !mapHasString(usesInverseMap, modifiedVar)) {
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
				&& setHasString(assignments, parent) && setHasString(modifiesMap[parent], modifiedVar)) {
				continue; // cannot break, as there may exist another path that does not modifies
			}
			for (string child : children) {
				auto it = visited.find(child);
				if (it == visited.end()) {
					unordered_set<string> nextChildren = filterMapKeyReturnSetValues(child, dataAccessLayer, cfg);
					DFSStack.push(make_tuple(child, nextChildren));
				}
				unordered_set<string> uses = usesMap[child]; // variables used at child node
				if (setHasString(assignments, child) && setHasString(uses, modifiedVar) 
					&& child == svToString(arg2->getArgValue())) { // is a valid affects
					shared_ptr<QueryResultsTable> table = QueryResultsTable::createEmptyTable();
					table->setSignificant(true);
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