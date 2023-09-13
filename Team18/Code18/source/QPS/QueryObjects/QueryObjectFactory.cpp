#include "QueryObjectFactory.h"
#include "DesignObjectsFactory.h"

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
		return make_shared<StmtObjectFactory>();
	}
	else if (type == "Modifies"sv) {
		return make_shared<ReadObjectFactory>();
	}
	else if (type == "Follows"sv) {
		return make_shared<ReadObjectFactory>();
	}
	else if (type == "Follows*"sv) {
		return make_shared<ReadObjectFactory>();
	}
	else if (type == "Parent"sv) {
		return make_shared<ReadObjectFactory>();
	}
	else if (type == "Parent*"sv) {
		return make_shared<ReadObjectFactory>();
	}
	throw runtime_error("Invalid string token for design object");
}
