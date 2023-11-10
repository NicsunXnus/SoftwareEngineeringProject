#ifndef BOOLEANOBJECTFACTORY_H
#define BOOLEANOBJECTFACTORY_H

#include <string_view>
#include "BooleanQueryObject.h"
#include "QueryObjectFactory.h"

class BooleanObjectFactory : public QueryObjectFactory {
protected:

public:
	BooleanObjectFactory() : QueryObjectFactory{} {}

	shared_ptr<QueryObject> create(string_view clauseName, 
		vector<shared_ptr<ClauseArg>> arguments = vector<shared_ptr<ClauseArg>>()) {

		return make_shared<BooleanQueryObject>(clauseName);
	}
};
#endif