#pragma once
#ifndef QUERYOBJECT_H
#define QUERYOBJECT_H

#include "../DataAccessLayer.h"


using namespace std;
// API calls should be a function in the query object

/*
* This class represents a Query object; abstractions that are used to query the PKB
*/

class QueryObject {
private:
	string_view tokenName;
	vector<int> res;
public:
	QueryObject(string_view tokenName)
		: tokenName{ tokenName } {
	};

	// pure virtual function, for calling PKB
	virtual void call(shared_ptr<DataAccessLayer> dataAccessLayer) = 0;

	string_view getQueryObjectName() {
		return tokenName;
	}

	void setResult(vector<int> result) {
		res = result;
	}

	vector<int> getResult() {
		return res;
	}


};


#endif