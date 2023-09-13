#pragma once
#ifndef DESIGNOBJECTFACTORY_H
#define DESIGNOBJECTFACTORY_H

#include <string_view>
#include "QueryObjectFactory.h"
#include "DesignObjects.h"

using namespace std;


/*
* This class represents a Query Object Factory, for design entities
*/
class DesignObjectFactory : public QueryObjectFactory {
public:
	DesignObjectFactory() : QueryObjectFactory{} {

	}

};

/*
* This class represents a Query Object Factory, for design entity stmt
*/
class StmtObjectFactory : public DesignObjectFactory {
public:
	StmtObjectFactory() {

	};

	shared_ptr<QueryObject> create(string_view str) override {
		return make_shared<StmtObject>(str);
	}
	
};

/*
* This class represents a Query Object Factory, for design entity read
*/
class ReadObjectFactory : public DesignObjectFactory {
public:
	ReadObjectFactory() {
	};

	shared_ptr<QueryObject> create(string_view str) override {
		return make_shared<ReadObject>(str);
	}
};

/*
* This class represents a Query Object Factory, for design entity print
*/
class PrintObjectFactory : public DesignObjectFactory {
public:
	PrintObjectFactory() {
	};

	shared_ptr<QueryObject> create(string_view str) override {
		return make_shared<PrintObject>(str);
	}
};

/*
* This class represents a Query Object Factory, for design entity call
*/
class CallObjectFactory : public DesignObjectFactory {
public:
	CallObjectFactory() {
	};

	shared_ptr<QueryObject> create(string_view str) override {
		return make_shared<CallObject>(str);
	}
};

/*
* This class represents a Query Object Factory, for design entity while
*/
class WhileObjectFactory : public DesignObjectFactory {
public:
	WhileObjectFactory() {
	};

	shared_ptr<QueryObject> create(string_view str) override {
		return make_shared<WhileObject>(str);
	}
};

/*
* This class represents a Query Object Factory, for design entity if
*/
class IfObjectFactory : public DesignObjectFactory {
public:
	IfObjectFactory() {
	};

	shared_ptr<QueryObject> create(string_view str) override {
		return make_shared<IfObject>(str);
	}
};

/*
* This class represents a Query Object Factory, for design entity assign
*/
class AssignObjectFactory : public DesignObjectFactory {
public:
	AssignObjectFactory() {
	};

	shared_ptr<QueryObject> create(string_view str) override {
		return make_shared<AssignObject>(str);
	}
};

/*
* This class represents a Query Object Factory, for design entity variable
*/
class VariableObjectFactory : public DesignObjectFactory {
public:
	VariableObjectFactory() {
	};

	shared_ptr<QueryObject> create(string_view str) override {
		return make_shared<VariableObject>(str);
	}
};

/*
* This class represents a Query Object Factory, for design entity constant
*/
class ConstantObjectFactory : public DesignObjectFactory {
public:
	ConstantObjectFactory() {
	};

	shared_ptr<QueryObject> create(string_view str) override {
		return make_shared<ConstantObject>(str);
	}
};

/*
* This class represents a Query Object Factory, for design entity procedure
*/
class ProcedureObjectFactory : public DesignObjectFactory {
public:
	ProcedureObjectFactory() {
	};

	shared_ptr<QueryObject> create(string_view str) override {
		return make_shared<ProcedureObject>(str);
	}
};


#endif
