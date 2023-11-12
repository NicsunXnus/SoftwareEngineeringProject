#include "ClauseTableFilter.h"

shared_ptr<QueryResultsTable> ClauseSynSyn::evaluate(shared_ptr<DataAccessLayer> dataAccessLayer, ABSTRACTION clause) {
	if (arg1->getArgValue() == arg2->getArgValue()) {
		return QueryResultsTable::createEmptyTableWithHeaders({ svToString(arg1->getArgValue()) });
	}
	StringMap PKBClauseData = dataAccessLayer->getClause(clause);

	StringMap filteredPKBClauseDataArg1 = filterMapKeyReturnMap(arg1, dataAccessLayer, PKBClauseData);
	StringMap filteredPKBClauseDataArg2 = filterMapValueReturnMap(arg2, dataAccessLayer, filteredPKBClauseDataArg1);

	vector<string> headers({ svToString(arg1->getArgValue()), svToString(arg2->getArgValue()) });
	return QueryResultsTable::createTable(headers, filteredPKBClauseDataArg2);
}

shared_ptr<QueryResultsTable> ClauseSynIntOrIdent::evaluate(shared_ptr<DataAccessLayer> dataAccessLayer, ABSTRACTION clause) {
	StringMap PKBClauseData = dataAccessLayer->getClauseInverse(clause);
	unordered_set<string> filteredPKBClauseDataArg2 = filterMapKeyReturnSetValues(arg2, PKBClauseData);
	unordered_set<string> filteredPKBClauseDataArg1 = filterSetReturnSet(arg1, dataAccessLayer, filteredPKBClauseDataArg2);
	return QueryResultsTable::createTable(svToString(arg1->getArgValue()), filteredPKBClauseDataArg1);
}

shared_ptr<QueryResultsTable> ClauseSynWildcard::evaluate(shared_ptr<DataAccessLayer> dataAccessLayer, ABSTRACTION clause) {
	StringMap PKBClauseData = dataAccessLayer->getClause(clause);
	StringMap filteredPKBClauseDataArg1 = filterMapKeyReturnMap(arg1, dataAccessLayer, PKBClauseData);
	unordered_set<string> filteredPKBClauseDataArg2 = removeMapValuesReturnSet(arg2, filteredPKBClauseDataArg1);
	return QueryResultsTable::createTable(svToString(arg1->getArgValue()), filteredPKBClauseDataArg2);
}

shared_ptr<QueryResultsTable> ClauseIntOrIdentSyn::evaluate(shared_ptr<DataAccessLayer> dataAccessLayer, ABSTRACTION clause) {
	StringMap PKBClauseData = dataAccessLayer->getClause(clause);
	unordered_set<string> filteredPKBClauseDataArg1 = filterMapKeyReturnSetValues(arg1, PKBClauseData);
	unordered_set<string> filteredPKBClauseDataArg2 = filterSetReturnSet(arg2, dataAccessLayer, filteredPKBClauseDataArg1);
	return QueryResultsTable::createTable(svToString(arg2->getArgValue()), filteredPKBClauseDataArg2);
}

shared_ptr<QueryResultsTable> ClauseIntOrIdentIntOrIdent::evaluate(shared_ptr<DataAccessLayer> dataAccessLayer, ABSTRACTION clause) {
	StringMap PKBClauseData = dataAccessLayer->getClause(clause);

	unordered_set<string> filteredPKBClauseDataArg1 = filterMapKeyReturnSetValues(arg1, PKBClauseData);
	bool filteredPKBClauseDataArg2 = filterSetReturnBool(arg2, filteredPKBClauseDataArg1);
	shared_ptr<QueryResultsTable> table = make_shared<QueryResultsTable>();
	table->setSignificant(filteredPKBClauseDataArg2);
	return table;
}

shared_ptr<QueryResultsTable> ClauseIntOrIdentWildcard::evaluate(shared_ptr<DataAccessLayer> dataAccessLayer, ABSTRACTION clause) {
	StringMap PKBClauseData = dataAccessLayer->getClause(clause);

	unordered_set<string> filteredPKBClauseDataArg1 = filterMapKeyReturnSetValues(arg1, PKBClauseData);
	shared_ptr<QueryResultsTable> table = make_shared<QueryResultsTable>();
	table->setSignificant(filteredPKBClauseDataArg1.size() > 0);
	return table;
}

shared_ptr<QueryResultsTable> ClauseWildcardSyn::evaluate(shared_ptr<DataAccessLayer> dataAccessLayer, ABSTRACTION clause) {
	StringMap PKBClauseData = dataAccessLayer->getClauseInverse(clause);
	StringMap filteredPKBClauseDataArg2 = filterMapKeyReturnMap(arg2, dataAccessLayer, PKBClauseData);
	unordered_set<string> filteredPKBClauseDataArg1 = removeMapValuesReturnSet(arg1, filteredPKBClauseDataArg2);
	return QueryResultsTable::createTable(svToString(arg2->getArgValue()), filteredPKBClauseDataArg1);
}

shared_ptr<QueryResultsTable> ClauseWildcardIntOrIdent::evaluate(shared_ptr<DataAccessLayer> dataAccessLayer, ABSTRACTION clause) {
	StringMap PKBClauseData = dataAccessLayer->getClauseInverse(clause);
	unordered_set<string> filteredPKBClauseDataArg2 = filterMapKeyReturnSetValues(arg2, PKBClauseData);
	shared_ptr<QueryResultsTable> table = make_shared<QueryResultsTable>();
	table->setSignificant(filteredPKBClauseDataArg2.size() > 0);
	return table;
}

shared_ptr<QueryResultsTable> ClauseWildcardWildcard::evaluate(shared_ptr<DataAccessLayer> dataAccessLayer, ABSTRACTION clause) {
	StringMap PKBClauseData = dataAccessLayer->getClause(clause);
	shared_ptr<QueryResultsTable> table = make_shared<QueryResultsTable>();
	table->setSignificant(PKBClauseData.size() > 0);
	return table;
}

shared_ptr<QueryEval> ClauseFilterFactory::create(shared_ptr<ClauseArg> argument1, shared_ptr<ClauseArg> argument2) {
	if (argument1->isSynonym() && argument2->isSynonym()) {
		return make_shared<ClauseSynSyn>(argument1, argument2);
	}
	else if (argument1->isSynonym() && argument2->isWildcard()) {
		return make_shared<ClauseSynWildcard>(argument1, argument2);
	}
	else if (argument1->isSynonym() && (argument2->isInteger() || argument2->isIdentifier())) {
		return make_shared<ClauseSynIntOrIdent>(argument1, argument2);
	}
	else if ((argument1->isInteger() || argument1->isIdentifier()) && argument2->isSynonym()) {
		return make_shared<ClauseIntOrIdentSyn>(argument1, argument2);
	}
	else if ((argument1->isInteger() || argument1->isIdentifier()) && argument2->isWildcard()) {
		return make_shared<ClauseIntOrIdentWildcard>(argument1, argument2);
	}
	else if ((argument1->isInteger() || argument1->isIdentifier()) && (argument2->isInteger() || argument2->isIdentifier())) {
		return make_shared<ClauseIntOrIdentIntOrIdent>(argument1, argument2);
	}
	else if (argument1->isWildcard() && argument2->isSynonym()) {
		return make_shared<ClauseWildcardSyn>(argument1, argument2);
	}
	else if (argument1->isWildcard() && argument2->isWildcard()) {
		return make_shared<ClauseWildcardWildcard>(argument1, argument2);
	}
	else if (argument1->isWildcard() && (argument2->isInteger() || argument2->isIdentifier())) {
		return make_shared<ClauseWildcardIntOrIdent>(argument1, argument2);
	}
	throw SemanticErrorException("Error in clause filter factory, invalid argument combination");
}