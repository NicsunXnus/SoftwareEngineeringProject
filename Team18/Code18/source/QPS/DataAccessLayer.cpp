#pragma once
#include "DataAccessLayer.h"
#include <list>
#include <string>

vector<string> DataAccessLayer::getEntity(ENTITY type) {
	vector<string> PKBResponse = PKB::responder.getEntityStatement(type);
	return PKBResponse;
}
