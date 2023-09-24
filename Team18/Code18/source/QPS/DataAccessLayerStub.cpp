#pragma once
#include "DataAccessLayerStub.h"
#include <list>
vector<string> entities = { "1", "2", "3", "4"};
vector<string> procedures = {"main"};
vector<string> variables = { "a", "b", "c" };
vector<string> constants = {"100", "300"};
map<string, vector<string>> constantMap = { {"100", {"1"}}, {"300", {"3", "4"}} };
map<string, vector<string>> variableMap = { {"a", {"1"}}, {"b", {"2", "3"}}, {"c", {"3"}} };
map<string, vector<string>> clauses = { {"1", {"2"}}, {"2", {"3", "4"}}, {"3", {"4"}}};
map<string, vector<string>> clausesEnts = { {"a", {"1"}}, {"b", {"2", "3"}}, {"c", {"3"}}};


vector<string> DataAccessLayerStub::getEntity(ENTITY type) {
	if (type == VARIABLE) {
		return variables;
	}
	return entities;
}

vector<string> DataAccessLayerStub::getAllProcedures() {
	return procedures;
}

vector<string> DataAccessLayerStub::getAllVariables() {
	return variables;
}

vector<string> DataAccessLayerStub::getAllConstants() {
	return constants;
}

map<string, vector<string>> DataAccessLayerStub::getVariableMap() {
	return variableMap;
}

map<string, vector<string>> DataAccessLayerStub::getConstantMap() {
	return constantMap;
}

map<string, vector<string>> DataAccessLayerStub::getClause(ABSTRACTION abstraction) {
	if (abstraction == USES || abstraction == MODIFIES) {
		return clausesEnts;
	}
	return clauses;
}
