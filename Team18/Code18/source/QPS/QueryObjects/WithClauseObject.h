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
};

inline shared_ptr<QueryResultsTable> returnWithTable(string_view argName, string attrName, StringMap attrMap) {
	vector<string> headers({ svToString(argName), svToString(argName) + attrName }); // p, p.procName
	shared_ptr<QueryResultsTable> table = QueryResultsTable::createTable(headers, attrMap);
	table->setPrimaryKey(svToString(argName) + attrName);
	return table;
}

// This class represent the procName attribute reference clause
class ProcNameObject : public WithObject {
private:
	string attrName = ".procName";
public:
	ProcNameObject(string_view clauseName, shared_ptr<ClauseArg> synonym)
		: WithObject{ clauseName, synonym } {};

	shared_ptr<QueryResultsTable> callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer) override {
		ENTITY type = getObjectSynonym()->getSynonym()->getEntityType();
		
		if (type == PROCEDURE) {
			unordered_set<string> PKBdata = dataAccessLayer->getAllProcedures();
			StringMap attrMap;
			for (string procName : PKBdata) {
				attrMap[procName] = { procName };
			}

			string_view argName = getObjectSynonym()->getArgValue();

			return returnWithTable(argName, attrName, attrMap);
		}
		else if (type == CALL) {
			StringMap PKBdata = dataAccessLayer->getCallProcNames();
			string_view argName = getObjectSynonym()->getArgValue();
			return returnWithTable(argName, attrName, PKBdata);
		}

		return QueryResultsTable::createEmptyTable();
	};
};

// This class represents the varName attribute reference clause
class VarNameObject : public WithObject {
private:
	string attrName = ".varName";
public:
	VarNameObject(string_view clauseName, shared_ptr<ClauseArg> synonym)
		: WithObject{ clauseName, synonym } {
	};

	shared_ptr<QueryResultsTable> callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer) override {
		ENTITY type = getObjectSynonym()->getSynonym()->getEntityType();

		if (type == VARIABLE) {
			unordered_set<string> PKBdata = dataAccessLayer->getEntity(VARIABLE);
			StringMap attrMap;
			for (string procName : PKBdata) {
				attrMap[procName] = { procName };
			}

			string_view argName = getObjectSynonym()->getArgValue();

			return returnWithTable(argName, attrName, attrMap);
		}
		else if (type == READ) {
			StringMap PKBdata = dataAccessLayer->getReadVarNames();
			string_view argName = getObjectSynonym()->getArgValue();
			return returnWithTable(argName, attrName, PKBdata);
		}
		else if (type == PRINT) {
			StringMap PKBdata = dataAccessLayer->getPrintVarNames();
			string_view argName = getObjectSynonym()->getArgValue();
			return returnWithTable(argName, attrName, PKBdata);
		}

		return QueryResultsTable::createEmptyTable();
	};
};

// This class represents the value attribute reference clause
class ValueObject : public WithObject {
private:
	string attrName = ".value";
public:
	ValueObject(string_view clauseName, shared_ptr<ClauseArg> synonym)
		: WithObject{ clauseName, synonym } {
	};

	shared_ptr<QueryResultsTable> callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer) override {
		ENTITY type = getObjectSynonym()->getSynonym()->getEntityType();

		if (type == CONSTANT) {
			unordered_set<string> PKBdata = dataAccessLayer->getEntity(CONSTANT);
			StringMap attrMap;
			for (string procName : PKBdata) {
				attrMap[procName] = { procName };
			}

			string_view argName = getObjectSynonym()->getArgValue();

			return returnWithTable(argName, attrName, attrMap);
		}


		return QueryResultsTable::createEmptyTable();
	};
};

// This class represents the statement number attribute reference clause
class StmtNoObject : public WithObject {
private:
	string attrName = ".stmt#";
public:
	StmtNoObject(string_view clauseName, shared_ptr<ClauseArg> synonym)
		: WithObject{ clauseName, synonym } {
	};

	shared_ptr<QueryResultsTable> callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer) override {
		ENTITY type = getObjectSynonym()->getSynonym()->getEntityType();
		if (type == STMT || type == READ || type == PRINT || type == CALL || type == WHILE || type == IF || type == ASSIGN) {
			unordered_set<string> PKBdata = dataAccessLayer->getEntity(type);
			StringMap attrMap;
			for (string procName : PKBdata) {
				attrMap[procName] = { procName };
			}

			string_view argName = getObjectSynonym()->getArgValue();

			return returnWithTable(argName, attrName, attrMap);
		}
		return QueryResultsTable::createEmptyTable();
	};
};

#endif
