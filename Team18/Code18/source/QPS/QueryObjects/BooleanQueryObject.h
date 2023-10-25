#ifndef BOOLEANQUERYOBJECT_H
#define BOOLEANQUERYOBJECT_H

#include <string_view>
#include "QueryObject.h"
#include "ClauseArg.h"

/*
* This class represents a Query object, for the BOOLEAN result object
*/
class BooleanQueryObject : public QueryObject {
private:

public:
	BooleanQueryObject(string_view clauseName)
		: QueryObject{ clauseName } {
	}

	// shouldn't be called, since a boolean object does not require PKB access
	shared_ptr<QueryResultsTable> callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer) {
		return null_ptr;
	}

};

#endif