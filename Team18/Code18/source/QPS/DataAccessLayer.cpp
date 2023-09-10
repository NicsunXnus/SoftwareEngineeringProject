#include "DataAccessLayer.h"
#include "../PKB.h"
#include <list>
#include <string>


vector<string> DataAccessLayer::getEntityStatement(ENTITY type) {
	vector<int> result; // TODO: Implement PBK call here
	vector<string> stringVector;
	// PKB::responder.getEntityStatement(type);
	for (const int& intValue : result) {
		// Convert each integer to a string and append it to the stringList
		stringVector.push_back(to_string(intValue));
	}
	return stringVector;
}

vector<string> DataAccessLayer::getAllProcedures() {
	vector<string> stringVector;
	return stringVector;
}

vector<string> DataAccessLayer::getAllVariables() {
	vector<string> stringVector;
	return stringVector;
}

vector<string> DataAccessLayer::getAllConstants() {
	vector<string> stringVector;
	return stringVector;
}

