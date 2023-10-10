#ifndef WITHCLAUSEOBJECTFACTORY_H
#define WITHCLAUSEOBJECTFACTORY_H

#include <string_view>
#include "WithClauseObject.h"
#include "QueryObjectFactory.h"


class WithClauseObjectFactory : public QueryObjectFactory {
private:

public:
	WithClauseObjectFactory() : QueryObjectFactory{} {}
};

// This class represents the procName attribute reference clause object factory
class ProcNameObjectFactory : public WithClauseObjectFactory {
public:
	ProcNameObjectFactory() {};

	shared_ptr<QueryObject> create(string_view clauseName, vector<std::shared_ptr<ClauseArg>> arguments) override {
		return make_shared<StmtObject>("");
	};
};

// This class represents the varName attribute reference clause object factory
class VarNameObjectFactory : public WithClauseObjectFactory {
public:
	VarNameObjectFactory() {};

	shared_ptr<QueryObject> create(string_view clauseName, vector<std::shared_ptr<ClauseArg>> arguments) override {
		return make_shared<StmtObject>("");
	};
};


// This class represents the value attribute reference clause object factory
class ValueObjectFactory : public WithClauseObjectFactory {
public:
	ValueObjectFactory() {};

	shared_ptr<QueryObject> create(string_view clauseName, vector<std::shared_ptr<ClauseArg>> arguments) override {
		return make_shared<StmtObject>("");
	};
};

// This class represents the stmt# attribute reference clause object factory
class StmtNoObjectFactory : public WithClauseObjectFactory {
public:
	StmtNoObjectFactory() {};

	shared_ptr<QueryObject> create(string_view clauseName, vector<std::shared_ptr<ClauseArg>> arguments) override {
		return make_shared<StmtObject>("");
	};
};

#endif
