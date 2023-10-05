#pragma once
#include "DataAccessLayer.h"
#include <list>


unordered_set<string> DataAccessLayer::getEntity(ENTITY type) {
	if (type == CONSTANT) {
		return getAllConstants();
	}
	else if (type == VARIABLE) {
		return getAllVariables();
	}
	else if (type == PROCEDURE) {
		return getAllProcedures();
	}
	unordered_set<string> PKBResponse = PKB::responder.getEntityStatement(type);
	return PKBResponse;
}

unordered_set<string> DataAccessLayer::getAllProcedures() {
	unordered_set<string> PKBResponse = PKB::responder.getAllProcedures();
	return PKBResponse;
}

unordered_set<string> DataAccessLayer::getAllVariables() {
	unordered_set<string> PKBResponse = PKB::responder.getAllVariables();
	return PKBResponse;
}

unordered_set<string> DataAccessLayer::getAllConstants() {
	unordered_set<string> PKBResponse = PKB::responder.getAllConstants();
	return PKBResponse;
}

map<string, unordered_set<string>> DataAccessLayer::getVariableMap() {
	StringMap PKBResponse = PKB::responder.getVariableMap();
	return PKBResponse;
}

map<string, unordered_set<string>> DataAccessLayer::getConstantMap() {
	StringMap PKBResponse = PKB::responder.getConstantMap();
	return PKBResponse;
}

StringMap DataAccessLayer::getClause(ABSTRACTION abstraction) {
	StringMap PKBResponse = PKB::responder.getAbstraction(abstraction);
	return PKBResponse;
}

StringMap DataAccessLayer::getClauseInverse(ABSTRACTION abstraction) {
	StringMap PKBResponse = PKB::responder.getAbstraction(abstraction, true);
	return PKBResponse;
}
