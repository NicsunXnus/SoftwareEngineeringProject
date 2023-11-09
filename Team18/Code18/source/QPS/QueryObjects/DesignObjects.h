#pragma once
#ifndef DESIGNOBJECT_H
#define DESIGNOBJECT_H

#include <string_view>
#include <string>
#include "QueryObject.h"



using namespace std;
/*
* This class represents a Query object, for design entities
*/
class DesignObject : public QueryObject {
private:
	string synonym;
public:
	DesignObject(string_view data)
		: QueryObject{ data } {
	}

	shared_ptr<unordered_set<string>> getSynonyms() {
		unordered_set<string> synonyms;
		synonyms.insert(synonym);
		return make_shared<unordered_set<string>>(synonyms);
	}
};

/*
* This class represents a Query object, for design entity stmt
*/
class StmtObject : public DesignObject {
public:
	StmtObject(string_view data)
		: DesignObject{ data } {
	};
	shared_ptr<QueryResultsTable> callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer) override {
		unordered_set<string> PKBdata = dataAccessLayer->getEntity(STMT);
		shared_ptr<QueryResultsTable> table = QueryResultsTable::createTable(svToString(getQueryObjectName()), PKBdata);
		table->setPrimaryKey(svToString(getQueryObjectName()));
		return table;
	}
};

/*
* This class represents a Query object, for design entity read
*/
class ReadObject : public DesignObject {
public:
	ReadObject(string_view data)
		: DesignObject{ data } {
	};
	shared_ptr<QueryResultsTable> callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer) override {
		unordered_set<string> PKBdata = dataAccessLayer->getEntity(READ);
		shared_ptr<QueryResultsTable> table = QueryResultsTable::createTable(svToString(getQueryObjectName()), PKBdata);
		table->setPrimaryKey(svToString(getQueryObjectName()));
		return table;
	}
};

/*
* This class represents a Query object, for design entity print
*/
class PrintObject : public DesignObject {
public:
	PrintObject(string_view data)
		: DesignObject{ data } {
	};
	shared_ptr<QueryResultsTable> callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer) override {
		unordered_set<string> PKBdata = dataAccessLayer->getEntity(PRINT);
		shared_ptr<QueryResultsTable> table = QueryResultsTable::createTable(svToString(getQueryObjectName()), PKBdata);
		table->setPrimaryKey(svToString(getQueryObjectName()));
		return table;
	}
};

/*
* This class represents a Query object, for design entity call
*/
class CallObject : public DesignObject {
public:
	CallObject(string_view data)
		: DesignObject{ data } {
	};
	shared_ptr<QueryResultsTable> callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer) override {
		unordered_set<string> PKBdata = dataAccessLayer->getEntity(CALL);
		shared_ptr<QueryResultsTable> table = QueryResultsTable::createTable(svToString(getQueryObjectName()), PKBdata);
		table->setPrimaryKey(svToString(getQueryObjectName()));
		return table;
	}
};

/*
* This class represents a Query object, for design entity while
*/
class WhileObject : public DesignObject {
public:
	WhileObject(string_view data)
		: DesignObject{ data } {
	};
	shared_ptr<QueryResultsTable> callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer) override {
		unordered_set<string> PKBdata = dataAccessLayer->getEntity(WHILE);
		shared_ptr<QueryResultsTable> table = QueryResultsTable::createTable(svToString(getQueryObjectName()), PKBdata);
		table->setPrimaryKey(svToString(getQueryObjectName()));
		return table;
	}
};

/*
* This class represents a Query object, for design entity if
*/
class IfObject : public DesignObject {
public:
	IfObject(string_view data)
		: DesignObject{ data } {
	};
	shared_ptr<QueryResultsTable> callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer) override {
		unordered_set<string> PKBdata = dataAccessLayer->getEntity(IF);
		shared_ptr<QueryResultsTable> table = QueryResultsTable::createTable(svToString(getQueryObjectName()), PKBdata);
		table->setPrimaryKey(svToString(getQueryObjectName()));
		return table;
	}
};

/*
* This class represents a Query object, for design entity assign
*/
class AssignObject : public DesignObject {
public:
	AssignObject(string_view data)
		: DesignObject{ data } {
	};
	shared_ptr<QueryResultsTable> callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer) override {
		unordered_set<string> PKBdata = dataAccessLayer->getEntity(ASSIGN);
		shared_ptr<QueryResultsTable> table = QueryResultsTable::createTable(svToString(getQueryObjectName()), PKBdata);
		table->setPrimaryKey(svToString(getQueryObjectName()));
		return table;
	}
};

/*
* This class represents a Query object, for design entity variable
*/
class VariableObject : public DesignObject {
public:
	VariableObject(string_view data)
		: DesignObject{ data } {
	};
	shared_ptr<QueryResultsTable> callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer) override {
		unordered_set<string> PKBdata = dataAccessLayer->getAllVariables();
		shared_ptr<QueryResultsTable> table = QueryResultsTable::createTable(svToString(getQueryObjectName()), PKBdata);
		table->setPrimaryKey(svToString(getQueryObjectName()));
		return table;
	}
};

/*
* This class represents a Query object, for design entity constant
*/
class ConstantObject : public DesignObject {
public:
	ConstantObject(string_view data)
		: DesignObject{ data } {
	};
	shared_ptr<QueryResultsTable> callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer) override {
		unordered_set<string> PKBdata = dataAccessLayer->getAllConstants();
		shared_ptr<QueryResultsTable> table = QueryResultsTable::createTable(svToString(getQueryObjectName()), PKBdata);
		table->setPrimaryKey(svToString(getQueryObjectName()));
		return table;
	}
};

/*
* This class represents a Query object, for design entity procedure
*/
class ProcedureObject : public DesignObject {
public:
	ProcedureObject(string_view data)
		: DesignObject{ data } {
	};
	shared_ptr<QueryResultsTable> callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer) override {
		unordered_set<string> PKBdata = dataAccessLayer->getAllProcedures();
		shared_ptr<QueryResultsTable> table = QueryResultsTable::createTable(svToString(getQueryObjectName()), PKBdata);
		table->setPrimaryKey(svToString(getQueryObjectName()));
		return table;
	}
};

#endif
