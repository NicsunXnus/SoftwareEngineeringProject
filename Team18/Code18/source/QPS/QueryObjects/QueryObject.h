#pragma once
#ifndef QUERYOBJECT_H
#define QUERYOBJECT_H

#include "../DataAccessLayer.h"
#include "../../Constants/DesignEnums.h"
#include "../QueryResultsTable.h"
#include "../../HelperFunctions.h"
#include "../Errors/SemanticError.h"
#include <unordered_map>


using namespace std;

/*
* This class represents a Query object; abstractions that are used to query the PKB
*/

class QueryObject {
private:
	string data; // stores information about the queryObject, such as its name, or arguments for clauses

public:
	QueryObject(string_view data) {
		this->data = string(data);
	};

	string_view getQueryObjectName();

	int getSynCount();

	/*
	* Returns the synonyms in a query object
	*/
	virtual shared_ptr<unordered_set<string>> getSynonyms() = 0;

	/*
	* Returns The name of the object stored in cache
	*/
	virtual string getCacheName();

	/*
	* Whether this object should be cached or not. Only objects that require DFS are cached
	*/
	virtual bool shouldCache();

	// pure virtual function, for getting the data from PKB and processing it into a results table by filtering and remove columns
	virtual shared_ptr<QueryResultsTable> callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer) = 0;

};


#endif