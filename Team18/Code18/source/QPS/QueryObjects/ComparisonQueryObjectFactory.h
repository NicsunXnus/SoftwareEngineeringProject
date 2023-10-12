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

	shared_ptr<QueryObject> create(string_view clauseName, vector<std::shared_ptr<ClauseArg>> arguments) override {
		if (static_cast<int>(arguments.size() != 2)) {
			throw SyntaxErrorException("invalid argument count for static static comparison object");
		}

		shared_ptr<ClauseArg> arg1{ arguments[0] };
		shared_ptr<ClauseArg> arg2{ arguments[1] };

		bool arg1IsValid{ arg1->isIdentifier || arg1->isInteger };
		bool arg2IsValid{ arg2->isIdentifier || arg2->isInteger };

		if (!arg1IsValid || !arg2IsValid) {
			throw SyntaxErrorException("invalid static static comparison arguments");
		}

		return make_shared<ProcNameObject>(clauseName, synonym);
	};
};

class StaticAttrRefComparisonFactory : public ComparisonQueryObjectFactory {
private:

public:
	StaticAttrRefComparisonFactory() : ComparisonQueryObjectFactory{} {}

	// arg[0] is synonym name, arg[1] is attrName, arg[2] is static value
	shared_ptr<QueryObject> create(string_view clauseName, vector<std::shared_ptr<ClauseArg>> arguments) override {
		if (static_cast<int>(arguments.size() != 3)) {
			throw SyntaxErrorException("invalid argument count for attrRef static comparison object");
		}

		shared_ptr<ClauseArg> synonymArg{ arguments[0] };
		shared_ptr<ClauseArg> attrName{ arguments[1] };
		shared_ptr<ClauseArg> staticValue{ arguments[2] };

		bool arg1IsValid{ arg1->isIdentifier || arg1->isInteger };
		bool arg2IsValid{ arg2->isIdentifier || arg2->isInteger };

		if (!arg1IsValid || !arg2IsValid) {
			throw SyntaxErrorException("invalid static static comparison arguments");
		}

		return make_shared<ProcNameObject>(clauseName, synonym);
	};
};

class AttrRefAttrRefComparisonFactory : public ComparisonQueryObjectFactory {
private:

public:
	AttrRefAttrRefComparisonFactory() : ComparisonQueryObjectFactory{} {}

	shared_ptr<QueryObject> create(string_view clauseName, vector<std::shared_ptr<ClauseArg>> arguments) override {
		if (static_cast<int>(arguments.size() != 4)) {
			throw SyntaxErrorException("invalid argument count for attrRef attrRef comparison object");
		}


	}
};

#endif
