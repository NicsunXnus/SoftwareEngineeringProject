#ifndef WITHCLAUSEOBJECT_H
#define WITHCLAUSEOBJECT_H

#include <string_view>
#include "QueryObject.h"
#include "ClauseArg.h"

/*
* This class represents a With clause object
* For invalid attributes, e.g. proc.stmt#, return empty table
*/
class WithObject : public QueryObject {
private:
	shared_ptr<ClauseArg> synonym;

public:
	WithObject(string_view clauseName, shared_ptr<ClauseArg> synonym)
		: QueryObject{ clauseName }, synonym{ synonym } {};
	shared_ptr<ClauseArg> getObjectSynonym() {
		return synonym;
	}
protected:
	shared_ptr<QueryResultsTable> returnWithTable(string_view argName, string attrName, StringMap attrMap);
	shared_ptr<QueryResultsTable> processTrivialTable(shared_ptr<DataAccessLayer> dataAccessLayer, string attrName, ENTITY entity);
};

// This class represents the procName attribute reference clause
class ProcNameObject : public WithObject {
private:
	string attrName = ".procName";
public:
	ProcNameObject(string_view clauseName, shared_ptr<ClauseArg> synonym)
		: WithObject{ clauseName, synonym } {};

	shared_ptr<QueryResultsTable> callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer) override;
};

// This class represents the varName attribute reference clause
class VarNameObject : public WithObject {
private:
	string attrName = ".varName";
public:
	VarNameObject(string_view clauseName, shared_ptr<ClauseArg> synonym)
		: WithObject{ clauseName, synonym } {
	};

	shared_ptr<QueryResultsTable> callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer) override;
};

// This class represents the value attribute reference clause
class ValueObject : public WithObject {
private:
	string attrName = ".value";
public:
	ValueObject(string_view clauseName, shared_ptr<ClauseArg> synonym)
		: WithObject{ clauseName, synonym } {
	};

	shared_ptr<QueryResultsTable> callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer) override;
};

// This class represents the statement number attribute reference clause
class StmtNoObject : public WithObject {
private:
	string attrName = ".stmt#";
public:
	StmtNoObject(string_view clauseName, shared_ptr<ClauseArg> synonym)
		: WithObject{ clauseName, synonym } {
	};

	shared_ptr<QueryResultsTable> callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer) override;
};

#endif
