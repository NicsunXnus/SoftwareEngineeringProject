#pragma once
#include "DataAccessLayer.h"
#include <list>
#include <string>

vector<string> DataAccessLayer::getEntityStatement(ENTITY type) {
	vector<string> stringVector;
	vector<string> PKBResponse = responder.getEntityStatement(type);
	for (const string& intValue : PKBResponse) {
		// Convert each integer to a string and append it to the stringVector
		stringVector.push_back(intValue);
	}
	return stringVector;
}

vector<string> DataAccessLayer::getAllProcedures() {
	vector<string> PKBResponse = responder.getAllProcedures();
	return PKBResponse;
}

vector<string> DataAccessLayer::getAllVariables() {
	vector<string> PKBResponse = responder.getAllVariables();
	return PKBResponse;
}

vector<string> DataAccessLayer::getAllConstants() {
	vector<string> PKBResponse = responder.getAllConstants();
	return PKBResponse;
}

