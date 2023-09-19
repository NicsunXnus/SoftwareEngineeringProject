#pragma once
#include "DataAccessLayer.h"
#include <list>


vector<string> DataAccessLayer::getEntity(ENTITY type) {
	vector<string> PKBResponse = PKB::responder.getEntityStatement(type);
	return PKBResponse;
}

vector<string> DataAccessLayer::getAllProcedures() {
	vector<string> PKBResponse = PKB::responder.getAllProcedures();
	return PKBResponse;
}

vector<string> DataAccessLayer::getAllVariables() {
	vector<string> PKBResponse = PKB::responder.getAllVariables();
	return PKBResponse;
}

vector<string> DataAccessLayer::getAllConstants() {
	vector<string> PKBResponse = PKB::responder.getAllConstants();
	return PKBResponse;
}

map<string, vector<string>> DataAccessLayer::getClause(ABSTRACTION abstraction) {
	map<string, vector<string>> PKBResponse = PKB::responder.getAbstraction(abstraction);
	return PKBResponse;
}
