#ifndef WITHCLAUSEOBJECT_H
#define WITHCLAUSEOBJECT_H

#include <string_view>
#include "QueryObject.h"
#include "ClauseArg.h"

/*
* This class represents a With clause object
*/
class WithObject : public QueryObject {
private:
	shared_ptr<ClauseArg> synonym;

public:
	WithObject(string_view clauseName, shared_ptr<ClauseArg> synonym)
		: QueryObject{ clauseName }, synonym{ synonym } {};
};

// This class represent the procName attribute reference clause
class ProcNameObject : public WithObject {
public:
	ProcNameObject(string_view clauseName, shared_ptr<ClauseArg> synonym)
		: WithObject{ clauseName, synonym } {};

	shared_ptr<QueryResultsTable> callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer) override {};
};

// This class represents the varName attribute reference clause
class VarNameObject : public WithObject {
public:
	VarNameObject(string_view clauseName, shared_ptr<ClauseArg> synonym)
		: WithObject{ clauseName, synonym } {
	};

	shared_ptr<QueryResultsTable> callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer) override {};
};

// This class represents the value attribute reference clause
class ValueObject : public WithObject {
public:
	ValueObject(string_view clauseName, shared_ptr<ClauseArg> synonym)
		: WithObject{ clauseName, synonym } {
	};

	shared_ptr<QueryResultsTable> callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer) override {};
};

// This class represents the statement number attribute reference clause
class StmtNoObject : public WithObject {
public:
	StmtNoObject(string_view clauseName, shared_ptr<ClauseArg> synonym)
		: WithObject{ clauseName, synonym } {
	};

	shared_ptr<QueryResultsTable> callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer) override {};
};

#endif
