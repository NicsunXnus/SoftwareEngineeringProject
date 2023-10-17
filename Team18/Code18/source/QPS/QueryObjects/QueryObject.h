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
// API calls should be a function in the query object

/*
* This class represents a Query object; abstractions that are used to query the PKB
*/

class QueryObject {
private:
	string_view data; // stores information about the queryObject, such as its name, or arguments for clauses
	shared_ptr<QueryResultsTable> table;
public:
	QueryObject(string_view data)
		: data{ data } {};

	string_view getQueryObjectName() {
		return data;
	}

	// pure virtual function, for getting the data from PKB and processing it into a results table by filtering and remove columns
	virtual shared_ptr<QueryResultsTable> callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer) = 0;
	// Filters the Table for the specific synonym / IDENT / _ / others (PKB gives entire table, e.g. entire Follows Table)
	// synonyms comes from QueryParser, will be passed in QueryBuilder from QueryDriver

};


#endif