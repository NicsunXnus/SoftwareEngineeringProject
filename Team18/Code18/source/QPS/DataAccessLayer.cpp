#pragma once
#include "DataAccessLayer.h"
#include <list>
#include <string>

vector<string> DataAccessLayer::getEntity(ENTITY type) {
	vector<string> PKBResponse = PKB::responder.getEntityStatement(type);
	return PKBResponse;
}

map<string, vector<string>> DataAccessLayer::getClause() {
	map<string, vector<string>> result;
	return result;
}
