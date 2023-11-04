#ifndef PATTERNCLAUSEOBJECTFACTORY_H
#define PATTERNCLAUSEOBJECTFACTORY_H

#include <string_view>
#include "PatternClauseObject.h"
#include "QueryObjectFactory.h"


class PatternClauseObjectFactory : public QueryObjectFactory {
private:
	const unordered_set<ENTITY> validArg1SynonymEntities{ VARIABLE };
	const int IF_PATTERN_ARGUMENT_COUNT{ 4 };

	bool validateIfPatternArgs(vector<shared_ptr<ClauseArg>>& arguments);
	bool validateAssignPatternArgs(vector<shared_ptr<ClauseArg>>& arguments);
	bool validateWhilePatternArgs(vector<shared_ptr<ClauseArg>>& arguments);

	shared_ptr<QueryObject> createIf(string_view clauseName, vector<shared_ptr<ClauseArg>> arguments);

public:
	PatternClauseObjectFactory() : QueryObjectFactory{} {}

	shared_ptr<QueryObject> create(string_view clauseName, vector<shared_ptr<ClauseArg>> arguments) override;
};
#endif