#pragma once
#ifndef DESIGNOBJECT_H
#define DESIGNOBJECT_H

#include <string_view>
#include "QueryObject.h"
#include "../../Constants/QPSPKB.h"

using namespace std;
/*
* This class represents a Query object, for design entities
*/
class DesignObject : public QueryObject {
public:
	DesignObject(string_view tokenName)
		: QueryObject{ tokenName } {

	}
	
};

/*
* This class represents a Query object, for design entity stmt
*/
class StmtObject : public DesignObject {
public:
	StmtObject(string_view tokenName)
		: DesignObject{ tokenName } {
	};
};

/*
* This class represents a Query object, for design entity read
*/
class ReadObject : public DesignObject {
public:
	ReadObject(string_view tokenName)
		: DesignObject{ tokenName } {
	};
};

/*
* This class represents a Query object, for design entity print
*/
class PrintObject : public DesignObject {
public:
	PrintObject(string_view tokenName)
		: DesignObject{ tokenName } {
	};
};

/*
* This class represents a Query object, for design entity call
*/
class CallObject : public DesignObject {
public:
	CallObject(string_view tokenName)
		: DesignObject{ tokenName } {
	};
};

/*
* This class represents a Query object, for design entity while
*/
class WhileObject : public DesignObject {
public:
	WhileObject(string_view tokenName)
		: DesignObject{ tokenName } {
	};
};

/*
* This class represents a Query object, for design entity if
*/
class IfObject : public DesignObject {
public:
	IfObject(string_view tokenName)
		: DesignObject{ tokenName } {
	};
};

/*
* This class represents a Query object, for design entity assign
*/
class AssignObject : public DesignObject {
public:
	AssignObject(string_view tokenName)
		: DesignObject{ tokenName } {
	};
};

/*
* This class represents a Query object, for design entity variable
*/
class VariableObject : public DesignObject {
public:
	VariableObject(string_view tokenName)
		: DesignObject{ tokenName } {
	};
};

/*
* This class represents a Query object, for design entity constant
*/
class ConstantObject : public DesignObject {
public:
	ConstantObject(string_view tokenName)
		: DesignObject{ tokenName } {
	};
};

/*
* This class represents a Query object, for design entity procedure
*/
class ProcedureObject : public DesignObject {
public:
	ProcedureObject(string_view tokenName)
		: DesignObject{ tokenName } {
	};
};

#endif
