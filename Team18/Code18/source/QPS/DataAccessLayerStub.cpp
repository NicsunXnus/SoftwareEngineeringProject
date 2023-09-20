#pragma once
#include "DataAccessLayerStub.h"
#include <list>


vector<string> DataAccessLayerStub::getEntity(ENTITY type) {
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

map<string, vector<string>> DataAccessLayerStub::getClause(ABSTRACTION abstraction) {
	return clauses;
}


void DataAccessLayerStub::insertEntity(vector<string> data) {
	entities = data;
}
void DataAccessLayerStub::insertProcedures(vector<string> data) {
	procedures = data;
}
void DataAccessLayerStub::insertVariables(vector<string> data) {
	variables = data;
}
void DataAccessLayerStub::insertConstants(vector<string> data) {
	constants = data;
}

void DataAccessLayerStub::insertClauses(map<string, vector<string>> clause) {
	clauses = clause;
}
