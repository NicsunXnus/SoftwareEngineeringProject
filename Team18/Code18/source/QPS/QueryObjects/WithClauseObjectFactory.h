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
		shared_ptr<ClauseArg> synonym{ arguments[0] };
		
		if (!synonym->isSynonym) {
			throw SyntaxErrorException("procName synonym clause arg does not contain a synonym");
		}

		return make_shared<ProcNameObject>(clauseName, synonym);
	};
};

// This class represents the varName attribute reference clause object factory
class VarNameObjectFactory : public WithClauseObjectFactory {
public:
	VarNameObjectFactory() {};

	shared_ptr<QueryObject> create(string_view clauseName, vector<std::shared_ptr<ClauseArg>> arguments) override {
		shared_ptr<ClauseArg> synonym{ arguments[0] };

		if (!synonym->isSynonym) {
			throw SyntaxErrorException("varName synonym clause arg does not contain a synonym");
		}

		return make_shared<VarNameObject>(clauseName, synonym);
	};
};


// This class represents the value attribute reference clause object factory
class ValueObjectFactory : public WithClauseObjectFactory {
public:
	ValueObjectFactory() {};

	shared_ptr<QueryObject> create(string_view clauseName, vector<std::shared_ptr<ClauseArg>> arguments) override {
		shared_ptr<ClauseArg> synonym{ arguments[0] };

		if (!synonym->isSynonym) {
			throw SyntaxErrorException("value synonym clause arg does not contain a synonym");
		}

		return make_shared<ValueObject>(clauseName, synonym);
	};
};

// This class represents the stmt# attribute reference clause object factory
class StmtNoObjectFactory : public WithClauseObjectFactory {
public:
	StmtNoObjectFactory() {};

	shared_ptr<QueryObject> create(string_view clauseName, vector<std::shared_ptr<ClauseArg>> arguments) override {
		shared_ptr<ClauseArg> synonym{ arguments[0] };

		if (!synonym->isSynonym) {
			throw SyntaxErrorException("stmt# synonym clause arg does not contain a synonym");
		}

		return make_shared<StmtNoObject>(clauseName, synonym);
	};
};

#endif
