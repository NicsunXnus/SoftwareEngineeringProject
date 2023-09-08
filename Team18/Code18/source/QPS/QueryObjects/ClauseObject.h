#ifndef CLAUSEOBJECT_H
#define CLAUSEOBJECT_H

#include <string_view>
#include "QueryObject.h"

/*
* This class represents a Query object, for clause entities
*/
class ClauseObject : public QueryObject {
public:
	ClauseObject(string_view tokenName)
		: QueryObject{ tokenName } {

	}
};

#endif
