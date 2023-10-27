#ifndef NOTQUERYOBJECT_H
#define NOTQUERYOBJECT_H

#include "QueryObject.h"

/*
* This class represents a negated query object
*/
class NotQueryObject : public QueryObject {
private:
	shared_ptr<QueryObject> originalQueryObject;
	
	/*
	* Contains the synonym query objects found inside the original query object.
	* Excludes duplicated synonyms. E.g., not Follows(s, s) will only have 1 "s" query object in this vector.
	*/
	vector<shared_ptr<QueryObject>> synonyms;

public:
	NotQueryObject(string_view data, shared_ptr<QueryObject> originalQueryObject, vector<shared_ptr<QueryObject>> synonyms)
		: QueryObject(data), originalQueryObject{ originalQueryObject }, synonyms{ synonyms }  {};


	shared_ptr<QueryResultsTable> callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer) {
		// TODO
		return QueryResultsTable::createEmptyTable();
	}


};


#endif