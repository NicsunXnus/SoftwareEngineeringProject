#include "DataAccessLayer.h"
#include <list>
#include <string>


vector<string> DataAccessLayer::getEntityStatement(ENTITY type) {
	vector<string> stringVector;
	vector<int> PKBResponse = PKB::responder.getEntityStatement(type);
	for (const int& intValue : PKBResponse) {
		// Convert each integer to a string and append it to the stringVector
		stringVector.push_back(to_string(intValue));
	}
	return stringVector;
}

vector<string> DataAccessLayer::getAllProcedures() {
	vector<string> PKBResponse = PKB::responder.getAllProcedures();
	return PKBResponse;
}

vector<string> DataAccessLayer::getAllVariables() {
	std::cout << "In DataAccessLayer, calling pkb responder getAllVariables()\n";

	vector<string> PKBResponse = PKB::responder.getAllVariables();
	return PKBResponse;
}

vector<string> DataAccessLayer::getAllConstants() {
	vector<string> PKBResponse = PKB::responder.getAllConstants();
	return PKBResponse;
}

