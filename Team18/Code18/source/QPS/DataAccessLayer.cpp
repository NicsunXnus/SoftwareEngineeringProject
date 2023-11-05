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

shared_ptr<Node> DataAccessLayer::getPatternTree(string statement_number) {
    shared_ptr<Node> PKBResponse = PKB::responder.getPattern(statement_number);
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

StringMap DataAccessLayer::getCallProcNames() {
	StringMap PKBResponse = PKB::responder.getCallProcNameMap();
	return PKBResponse;
}

StringMap DataAccessLayer::getReadVarNames() {
	StringMap PKBResponse = PKB::responder.getReadVarNameMap();
	return PKBResponse;
}

StringMap DataAccessLayer::getPrintVarNames() {
	StringMap PKBResponse = PKB::responder.getPrintVarNameMap();
	return PKBResponse;
}

pair<string, string> DataAccessLayer::getProcLines(string procName) {
	pair<string, string> PKBResponse = PKB::responder.getProcLines(procName);
	return PKBResponse;
}

shared_ptr<ExtendedCFG> DataAccessLayer::getCFG() {
	if (cacheCFG->contains(CFG_CACHE_STRING)) {
		return cacheCFG->get(CFG_CACHE_STRING);
	}
	else {
		shared_ptr<ExtendedCFG> cfg = make_shared<ExtendedCFG>(shared_from_this());
		cacheCFG->insert(CFG_CACHE_STRING, cfg);
		return cfg;
	}
}
