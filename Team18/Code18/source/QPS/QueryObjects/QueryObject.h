#pragma once
#ifndef QUERYOBJECT_H
#define QUERYOBJECT_H

#include "../DataAccessLayer.h"
#include <variant>


using namespace std;
// API calls should be a function in the query object

/*
* This class represents a Query object; abstractions that are used to query the PKB
*/

class QueryObject {
private:
	string_view tokenName;
public:
	QueryObject(string_view tokenName)
		: tokenName{ tokenName } {
	};

	// pure virtual function, for calling PKB
	virtual void call(shared_ptr<DataAccessLayer> dataAccessLayer) = 0;

	string_view getQueryObjectName() {
		return tokenName;
	}

	// variant: design entities, clauses
	// Setting the results of the PKB call
	virtual void setResult(variant<vector<string>, map<string, vector<string>>> result) = 0;

	virtual variant<vector<string>, map<string, vector<string>>> getResult() = 0;


};


#endif