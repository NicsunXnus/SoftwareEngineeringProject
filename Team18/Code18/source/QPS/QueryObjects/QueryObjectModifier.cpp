#include "QueryObjectModifier.h"
#include "NotQueryObject.h"


shared_ptr<QueryObjectModifier> QueryObjectModifier::createModifier(string_view type) {
	if (type == "not"sv) {
		return make_shared<NotModifier>();
	}

	throw SyntaxErrorException("unidentified query modifier type found");
}


shared_ptr<QueryObject> NotModifier::modify(shared_ptr<QueryObject> queryObject, vector<shared_ptr<QueryObject>> synonymDesignObjects) {
	string query(queryObject->getQueryObjectName());
	string notName = "not" + query;
	return make_shared<NotQueryObject>(notName, queryObject, synonymDesignObjects);
}