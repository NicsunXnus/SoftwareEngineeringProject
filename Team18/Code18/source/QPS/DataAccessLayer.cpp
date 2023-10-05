#pragma once
#include "DataAccessLayer.h"
#include <list>


vector<string> DataAccessLayer::getEntity(ENTITY type) {
	unordered_set<string> PKBResponse = PKB::responder.getEntityStatement(type);
	// return PKBResponse;
	return vector<string>();
}

vector<string> DataAccessLayer::getAllProcedures() {
	unordered_set<string> PKBResponse = PKB::responder.getAllProcedures();
	// return PKBResponse;
	return vector<string>();
}

vector<string> DataAccessLayer::getAllVariables() {
	unordered_set<string> PKBResponse = PKB::responder.getAllVariables();
	// return PKBResponse;
	return vector<string>();
}

vector<string> DataAccessLayer::getAllConstants() {
	unordered_set<string> PKBResponse = PKB::responder.getAllConstants();
	// return PKBResponse;
	return vector<string>();
}

map<string, vector<string>> DataAccessLayer::getVariableMap() {
	StringMap PKBResponse = PKB::responder.getVariableMap();
	// return PKBResponse;
	return map<string, vector<string>>();
}

map<string, vector<string>> DataAccessLayer::getConstantMap() {
	StringMap PKBResponse = PKB::responder.getConstantMap();
	// return PKBResponse;
	return map<string, vector<string>>();
}

map<string, vector<string>> DataAccessLayer::getClause(ABSTRACTION abstraction) {
	StringMap PKBResponse = PKB::responder.getAbstraction(abstraction);
	// return PKBResponse;
	return map<string, vector<string>>();
}
