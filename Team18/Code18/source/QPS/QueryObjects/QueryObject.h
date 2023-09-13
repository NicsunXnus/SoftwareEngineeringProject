#pragma once
#ifndef QUERYOBJECT_H
#define QUERYOBJECT_H

#include "../DataAccessLayer.h"
#include "../../Constants/QPSPKB.h"
#include <variant>


using namespace std;
// API calls should be a function in the query object

/*
* This class represents a Query object; abstractions that are used to query the PKB
*/

class QueryObject {
private:
	vector<string_view> data; // stores information about the queryObject, such as its name, or arguments for clauses
public:
	QueryObject(vector<string_view> dataVector)
		: data{ dataVector } {
	};

	// pure virtual function, for calling PKB
	virtual void call(shared_ptr<DataAccessLayer> dataAccessLayer) = 0;

	string_view getQueryObjectName() {
		return data[0];
	}

	vector<string_view> getQueryObjectData() {
		return data;
	}

	// variant: design entities, clauses
	// Setting the results of the PKB call
	virtual void setResult(variant<vector<string>, map<string, vector<string>>> result) = 0;

	virtual variant<vector<string>, map<string, vector<string>>> getResult() = 0;


};


#endif