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

	const unordered_set<ENTITY> validSynonyms{ PROCEDURE, CALL };

	shared_ptr<QueryObject> create(string_view clauseName, vector<std::shared_ptr<ClauseArg>> arguments) override {
		shared_ptr<ClauseArg> synonym{ arguments[0] };
		
		if (!synonym->isSynonym()) {
			throw SyntaxErrorException("procName synonym clause arg does not contain a synonym");
		}

		ENTITY synonymEntity{ synonym->getSynonym()->getEntityType() };

		// check if synonym type is valid
		if (validSynonyms.find(synonymEntity) == validSynonyms.end()) {
			throw SemanticErrorException("Invalid syn type for procName obj");
		}

		return make_shared<ProcNameObject>(clauseName, synonym);
	};
};

// This class represents the varName attribute reference clause object factory
class VarNameObjectFactory : public WithClauseObjectFactory {
public:
	VarNameObjectFactory() {};

	const unordered_set<ENTITY> validSynonyms{ VARIABLE, READ, PRINT };

	shared_ptr<QueryObject> create(string_view clauseName, vector<std::shared_ptr<ClauseArg>> arguments) override {
		shared_ptr<ClauseArg> synonym{ arguments[0] };

		if (!synonym->isSynonym()) {
			throw SyntaxErrorException("varName synonym clause arg does not contain a synonym");
		}

		ENTITY synonymEntity{ synonym->getSynonym()->getEntityType() };

		// check if synonym type is valid
		if (validSynonyms.find(synonymEntity) == validSynonyms.end()) {
			throw SemanticErrorException("Invalid syn type for varName obj");
		}

		return make_shared<VarNameObject>(clauseName, synonym);
	};
};


// This class represents the value attribute reference clause object factory
class ValueObjectFactory : public WithClauseObjectFactory {
public:
	ValueObjectFactory() {};

	const unordered_set<ENTITY> validSynonyms{ CONSTANT };

	shared_ptr<QueryObject> create(string_view clauseName, vector<std::shared_ptr<ClauseArg>> arguments) override {
		shared_ptr<ClauseArg> synonym{ arguments[0] };

		if (!synonym->isSynonym()) {
			throw SyntaxErrorException("value synonym clause arg does not contain a synonym");
		}

		ENTITY synonymEntity{ synonym->getSynonym()->getEntityType() };

		// check if synonym type is valid
		if (validSynonyms.find(synonymEntity) == validSynonyms.end()) {
			throw SemanticErrorException("Invalid syn type for value obj");
		}

		return make_shared<ValueObject>(clauseName, synonym);
	};
};

// This class represents the stmt# attribute reference clause object factory
class StmtNoObjectFactory : public WithClauseObjectFactory {
public:
	StmtNoObjectFactory() {};

	const unordered_set<ENTITY> validSynonyms{ STMT, READ, PRINT, CALL, WHILE, IF, ASSIGN };

	shared_ptr<QueryObject> create(string_view clauseName, vector<std::shared_ptr<ClauseArg>> arguments) override {
		shared_ptr<ClauseArg> synonym{ arguments[0] };

		if (!synonym->isSynonym()) {
			throw SyntaxErrorException("stmt# synonym clause arg does not contain a synonym");
		}

		ENTITY synonymEntity{ synonym->getSynonym()->getEntityType() };

		// check if synonym type is valid
		if (validSynonyms.find(synonymEntity) == validSynonyms.end()) {
			throw SemanticErrorException("Invalid syn type for stmt# obj");
		}

		return make_shared<StmtNoObject>(clauseName, synonym);
	};
};

#endif
