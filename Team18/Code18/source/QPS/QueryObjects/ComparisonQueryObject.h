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

	virtual shared_ptr<unordered_set<string>> getSynonyms() = 0;
};

// This class represents a comparison object between two static data types
class StaticStaticComparisonQueryObject : public ComparisonQueryObject {
private:
	shared_ptr<ClauseArg> ref1;
	shared_ptr<ClauseArg> ref2;

public:
	StaticStaticComparisonQueryObject(string_view clauseName, shared_ptr<ClauseArg> ref1, shared_ptr<ClauseArg> ref2) :
		ComparisonQueryObject{ clauseName }, ref1{ ref1 }, ref2{ ref2 } {};

	shared_ptr<unordered_set<string>> getSynonyms() {
		unordered_set<string> synonyms;
		return make_shared<unordered_set<string>>(synonyms);
	}

	shared_ptr<QueryResultsTable> callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer) override;
};

// This class represents a comparison object between a synonym attribute reference and a static data type
class StaticAttrRefComparisonQueryObject : public ComparisonQueryObject {
private:
	shared_ptr<ClauseArg> ref;
	shared_ptr<QueryObject> attrRef;

public:
	StaticAttrRefComparisonQueryObject(string_view clauseName, shared_ptr<QueryObject> attrRef, shared_ptr<ClauseArg> ref2) :
		ComparisonQueryObject{ clauseName }, attrRef{ attrRef }, ref{ ref2 } {};

	shared_ptr<unordered_set<string>> getSynonyms() {
		return attrRef->getSynonyms();
	}

	shared_ptr<QueryResultsTable> callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer) override;
};

// This class represents a comparison object between two synonym attribute references
class AttrRefAttrRefComparisonQueryObject : public ComparisonQueryObject {
private:
	shared_ptr<QueryObject> attrRef1;
	shared_ptr<QueryObject> attrRef2;

public:
	AttrRefAttrRefComparisonQueryObject(string_view clauseName, shared_ptr<QueryObject> attrRef1, shared_ptr<QueryObject> attrRef2) :
		ComparisonQueryObject{ clauseName }, attrRef1{ attrRef1 }, attrRef2{ attrRef2 } {};
	
	shared_ptr<unordered_set<string>> getSynonyms() {
		unordered_set<string> synonyms = *attrRef1->getSynonyms();
		unordered_set<string> attrRef2Syn = *attrRef2->getSynonyms();
		synonyms.insert(attrRef2Syn.begin(), attrRef2Syn.end());
		return make_shared<unordered_set<string>>(synonyms);
	}

	shared_ptr<QueryResultsTable> callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer) override;
};

#endif
