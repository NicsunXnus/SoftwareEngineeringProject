#ifndef COMPARISONQUERYOBJECT_H
#define COMPARISONQUERYOBJECT_H

#include <string_view>
#include "QueryObject.h"
#include "ClauseArg.h"
#include "WithClauseObject.h"


// This class represents a comparison query object
class ComparisonQueryObject : public QueryObject {
private:

public:
	ComparisonQueryObject(string_view clauseName)
		: QueryObject{ clauseName } {};
};

// This class represents a comparison object between two static data types
class StaticStaticComparisonQueryObject : public ComparisonQueryObject {
private:
	shared_ptr<ClauseArg> ref1;
	shared_ptr<ClauseArg> ref2;

public:
	StaticStaticComparisonQueryObject(string_view clauseName, shared_ptr<ClauseArg> ref1, shared_ptr<ClauseArg> ref2) :
		ComparisonQueryObject{ clauseName }, ref1{ ref1 }, ref2{ ref2 } {};
};

// This class represents a comparison object between a synonym attribute reference and a static data type
class StaticAttrRefComparisonQueryObject : public ComparisonQueryObject {
private:
	shared_ptr<ClauseArg> ref;
	shared_ptr<WithObject> attrRef;

public:
	StaticAttrRefComparisonQueryObject(string_view clauseName, shared_ptr<WithObject> attrRef, shared_ptr<ClauseArg> ref2) :
		ComparisonQueryObject{ clauseName }, attrRef{ attrRef }, ref{ ref } {};
};

// This class represents a comparison object between two synonym attribute references
class AttrRefAttrRefComparisonQueryObject : public ComparisonQueryObject {
private:
	shared_ptr<WithObject> attrRef1;
	shared_ptr<WithObject> attrRef2;

public:
	AttrRefAttrRefComparisonQueryObject(string_view clauseName, shared_ptr<WithObject> attrRef1, shared_ptr<WithObject> attrRef2) :
		ComparisonQueryObject{ clauseName }, attrRef1{ attrRef1 }, attrRef2{ attrRef2 } {};
};

#endif
