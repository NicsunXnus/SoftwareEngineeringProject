#pragma once
#ifndef DESIGNOBJECTFACTORY_H
#define DESIGNOBJECTFACTORY_H

#include <string_view>
#include "QueryObject.h"

using namespace std;
/*
* This class represents a Query object, for design entities
*/
class DesignObjectFactory : public QueryObject {
public:
	DesignObjectFactory(string_view tokenName)
		: QueryObject{ tokenName } {
	}

	virtual shared_ptr<QueryObject> create(string_view str) {
		return make_shared<QueryObject>(str);
	}
};

/*
* This class represents a Query object, for design entity stmt
*/
class StmtObjectFactory : public DesignObjectFactory {
public:
	StmtObjectFactory(string_view tokenName)
		: DesignObjectFactory{ tokenName } {
	};

	shared_ptr<QueryObject> create(string_view str) override {
		return make_shared<StmtObject>(str);
	}
	
};

/*
* This class represents a Query object, for design entity read
*/
class ReadObjectFactory : public DesignObjectFactory {
public:
	ReadObjectFactory(string_view tokenName)
		: DesignObjectFactory{ tokenName } {
	};

	shared_ptr<QueryObject> create(string_view str) override {
		return make_shared<ReadObject>(str);
	}
};

/*
* This class represents a Query object, for design entity print
*/
class PrintObjectFactory : public DesignObjectFactory {
public:
	PrintObjectFactory(string_view tokenName)
		: DesignObjectFactory{ tokenName } {
	};

	shared_ptr<QueryObject> create(string_view str) override {
		return make_shared<PrintObject>(str);
	}
};

/*
* This class represents a Query object, for design entity call
*/
class CallObjectFactory : public DesignObjectFactory {
public:
	CallObjectFactory(string_view tokenName)
		: DesignObjectFactory{ tokenName } {
	};

	shared_ptr<QueryObject> create(string_view str) override {
		return make_shared<CallObject>(str);
	}
};

/*
* This class represents a Query object, for design entity while
*/
class WhileObjectFactory : public DesignObjectFactory {
public:
	WhileObjectFactory(string_view tokenName)
		: DesignObjectFactory{ tokenName } {
	};

	shared_ptr<QueryObject> create(string_view str) override {
		return make_shared<WhileObject>(str);
	}
};

/*
* This class represents a Query object, for design entity if
*/
class IfObjectFactory : public DesignObjectFactory {
public:
	IfObjectFactory(string_view tokenName)
		: DesignObjectFactory{ tokenName } {
	};

	shared_ptr<QueryObject> create(string_view str) override {
		return make_shared<IfObject>(str);
	}
};

/*
* This class represents a Query object, for design entity assign
*/
class AssignObjectFactory : public DesignObjectFactory {
public:
	AssignObjectFactory(string_view tokenName)
		: DesignObjectFactory{ tokenName } {
	};

	shared_ptr<QueryObject> create(string_view str) override {
		return make_shared<AssignObject>(str);
	}
};

/*
* This class represents a Query object, for design entity variable
*/
class VariableObjectFactory : public DesignObjectFactory {
public:
	VariableObjectFactory(string_view tokenName)
		: DesignObjectFactory{ tokenName } {
	};

	shared_ptr<QueryObject> create(string_view str) override {
		return make_shared<VariableObject>(str);
	}
};

/*
* This class represents a Query object, for design entity constant
*/
class ConstantObjectFactory : public DesignObjectFactory {
public:
	ConstantObjectFactory(string_view tokenName)
		: DesignObjectFactory{ tokenName } {
	};

	shared_ptr<QueryObject> create(string_view str) override {
		return make_shared<ConstantObject>(str);
	}
};

/*
* This class represents a Query object, for design entity procedure
*/
class ProcedureObjectFactory : public DesignObjectFactory {
public:
	ProcedureObjectFactory(string_view tokenName)
		: DesignObjectFactory{ tokenName } {
	};

	shared_ptr<QueryObject> create(string_view str) override {
		return make_shared<ProcedureObject>(str);
	}
};


/*
* This function takes in a string and returns the DesignObject related to the string. Throws exception otherwise
*/
shared_ptr<DesignObjectFactory> createDesignFactory(string_view type) {
	if (type == "stmt"sv) {
		return make_shared<StmtObjectFactory>(type);
	}
	else if (type == "read"sv) {
		return make_shared<ReadObjectFactory>(type);
	}
	else if (type == "print"sv) {
		return make_shared<PrintObjectFactory>(type);
	}
	else if (type == "call"sv) {
		return make_shared<CallObjectFactory>(type);
	}
	else if (type == "while"sv) {
		return make_shared<WhileObjectFactory>(type);
	}
	else if (type == "if"sv) {
		return make_shared<IfObjectFactory>(type);
	}
	else if (type == "assign"sv) {
		return make_shared<AssignObjectFactory>(type);
	}
	else if (type == "variable"sv) {
		return make_shared<VariableObjectFactory>(type);
	}
	else if (type == "constant"sv) {
		return make_shared<ConstantObjectFactory>(type);
	}
	else if (type == "procedure"sv) {
		return make_shared<ProcedureObjectFactory>(type);
	}
	throw runtime_error("Invalid string token for design object");
}

#endif
