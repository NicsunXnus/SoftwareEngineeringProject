#pragma once
#ifndef QUERYOBJECT_H
#define QUERYOBJECT_H


using namespace std;
// API calls should be a function in the query object

/*
* This class represnts a Query object; abstractions that are used to query the PKB
*/
class QueryObject {
private:
	string_view tokenName;
public:
	QueryObject(string_view tokenName)
		: tokenName{ tokenName } {
	};

	string_view getQueryObjectName() {
		return tokenName;
	}
};


#endif