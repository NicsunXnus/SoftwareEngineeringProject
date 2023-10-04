#pragma once
#include "DataAccessLayerStub.h"
#include <list>
unordered_set<string> entities = { "1", "2", "3", "4"};
unordered_set<string> procedures = {"main"};
unordered_set<string> variables = { "a", "b", "c" };
unordered_set<string> constants = {"100", "300"};
map<string, unordered_set<string>> constantMap = { {"100", {"1"}}, {"300", {"3", "4"}} };
map<string, unordered_set<string>> variableMap = { {"a", {"1"}}, {"b", {"2", "3"}}, {"c", {"3"}} };
map<string, unordered_set<string>> clauses = { {"1", {"2"}}, {"2", {"3"}}, {"3", {"4"}}};
map<string, unordered_set<string>> clausesMult = { {"1", {"2","3"}}, {"2", {"3", "4"}}, {"3", {"4"}}};
map<string, unordered_set<string>> clausesEnts = { {"a", {"1"}}, {"b", {"2", "3"}}, {"c", {"3"}}};


unordered_set<string> DataAccessLayerStub::getEntity(ENTITY type) {
	if (type == VARIABLE) {
		return variables;
	}
	return entities;
}

unordered_set<string> DataAccessLayerStub::getAllProcedures() {
	return procedures;
}

unordered_set<string> DataAccessLayerStub::getAllVariables() {
	return variables;
}

unordered_set<string> DataAccessLayerStub::getAllConstants() {
	return constants;
}

map<string, unordered_set<string>> DataAccessLayerStub::getVariableMap() {
	return variableMap;
}

map<string, unordered_set<string>> DataAccessLayerStub::getConstantMap() {
	return constantMap;
}

map<string, unordered_set<string>> DataAccessLayerStub::getClause(ABSTRACTION abstraction) {
	if (abstraction == USES || abstraction == MODIFIES) {
		return clausesEnts;
	}
	if (abstraction == FOLLOWSSTAR || abstraction == PARENTSTAR) {
		return clausesMult;
	}
	return clauses;
}
