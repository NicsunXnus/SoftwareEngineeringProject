#ifndef COMPARISONQUERYOBJECTFACTORY_H
#define COMPARISONQUERYOBJECTFACTORY_H

#include <string_view>
#include "ComparisonQueryObject.h"
#include "QueryObjectFactory.h"
#include "WithClauseObjectFactory.h"


class ComparisonQueryObjectFactory : public QueryObjectFactory {
private:

public:
	ComparisonQueryObjectFactory() : QueryObjectFactory{} {}

};

class StaticStaticComparisonFactory : public ComparisonQueryObjectFactory {
private:

public:
	StaticStaticComparisonFactory() : ComparisonQueryObjectFactory{} {}

	bool validateArgs(vector<shared_ptr<ClauseArg>>& arguments);

	shared_ptr<QueryObject> create(string_view clauseName, vector<shared_ptr<ClauseArg>> arguments) override;
};

class StaticAttrRefComparisonFactory : public ComparisonQueryObjectFactory {
private:

public:
	StaticAttrRefComparisonFactory() : ComparisonQueryObjectFactory{} {}

	bool validateArgs(vector<shared_ptr<ClauseArg>>& arguments);

	shared_ptr<QueryObject> createAttrRef(vector<shared_ptr<ClauseArg>> arguments);

	// arg[0] is synonym name, arg[1] is attrName, arg[2] is static value
	shared_ptr<QueryObject> create(string_view clauseName, vector<shared_ptr<ClauseArg>> arguments) override;
};

class AttrRefAttrRefComparisonFactory : public ComparisonQueryObjectFactory {
private:

public:
	AttrRefAttrRefComparisonFactory() : ComparisonQueryObjectFactory{} {}

	bool validateArgs(vector<shared_ptr<ClauseArg>>& arguments);

	shared_ptr<QueryObject> createAttrRef(vector<shared_ptr<ClauseArg>> arguments);

	shared_ptr<QueryObject> create(string_view clauseName, vector<shared_ptr<ClauseArg>> arguments) override;

};

#endif
