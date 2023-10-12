#include "QueryObjectFactory.h"
#include "DesignObjectsFactory.h"
#include "ClauseObjectFactory.h"
#include "PatternClauseObjectFactory.h"
#include "WithClauseObjectFactory.h"
#include "ComparisonQueryObjectFactory.h"
#include "../Errors/SyntaxError.h"

shared_ptr<QueryObjectFactory> QueryObjectFactory::createFactory(string_view type) {
	/* Design entity types */
	if (type == "stmt"sv) {
		return make_shared<StmtObjectFactory>();
	}
	else if (type == "read"sv) {
		return make_shared<ReadObjectFactory>();
	}
	else if (type == "print"sv) {
		return make_shared<PrintObjectFactory>();
	}
	else if (type == "call"sv) {
		return make_shared<CallObjectFactory>();
	}
	else if (type == "while"sv) {
		return make_shared<WhileObjectFactory>();
	}
	else if (type == "if"sv) {
		return make_shared<IfObjectFactory>();
	}
	else if (type == "assign"sv) {
		return make_shared<AssignObjectFactory>();
	}
	else if (type == "variable"sv) {
		return make_shared<VariableObjectFactory>();
	}
	else if (type == "constant"sv) {
		return make_shared<ConstantObjectFactory>();
	}
	else if (type == "procedure"sv) {
		return make_shared<ProcedureObjectFactory>();
	}

	/* Clause types */
	else if (type == "Uses"sv) {
		return make_shared<UsesObjectFactory>();
	}
	else if (type == "Modifies"sv) {
		return make_shared<ModifiesObjectFactory>();
	}
	else if (type == "Follows"sv) {
		return make_shared<FollowsObjectFactory>();
	}
	else if (type == "Follows*"sv) {
		return make_shared<FollowsStarObjectFactory>();
	}
	else if (type == "Parent"sv) {
		return make_shared<ParentObjectFactory>();
	}
	else if (type == "Parent*"sv) {
		return make_shared<ParentStarObjectFactory>();
	}
	else if (type == "Calls"sv) {
		return make_shared<CallsObjectFactory>();
	}
	else if (type == "Calls*"sv) {
		return make_shared<CallsStarObjectFactory>();
	}
	else if (type == "Next"sv) {
		return make_shared<NextObjectFactory>();
	}
	else if (type == "Next*"sv) {
		return make_shared<NextStarObjectFactory>();
	}
	else if (type == "Affects"sv) {
		return make_shared<AffectsObjectFactory>();
	}

	/* Pattern type */
	else if (type == "pattern"sv) {
		return make_shared<PatternClauseObjectFactory>();
	}

	/* With type */
	else if (type == "procName"sv) {
		return make_shared<ProcNameObjectFactory>();
	}
	else if (type == "varName"sv) {
		return make_shared<VarNameObjectFactory>();
	}
	else if (type == "value"sv) {
		return make_shared<ValueObjectFactory>();
	}
	else if (type == "stmt#"sv) {
		return make_shared<StmtNoObjectFactory>();
	}

	/* Comparison type */
	else if (type == "Static=Static"sv) {
		return make_shared<StaticStaticComparisonFactory>();
	}
	else if (type == "Static=AttrRef"sv) {
		return make_shared<StaticAttrRefComparisonFactory>();
	}
	else if (type == "AttrRef=AttrRef"sv) {
		return make_shared<AttrRefAttrRefComparisonFactory>();
	}

	throw SyntaxErrorException("Invalid string token for design object");
}
