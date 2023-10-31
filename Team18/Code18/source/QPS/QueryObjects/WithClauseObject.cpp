#include "WithClauseObject.h"

shared_ptr<QueryResultsTable> WithObject::returnWithTable(string_view argName, string attrName, StringMap attrMap) {
	vector<string> headers({ svToString(argName), svToString(argName) + attrName }); // p, p.procName
	shared_ptr<QueryResultsTable> table = QueryResultsTable::createTable(headers, attrMap);
	table->setPrimaryKey(svToString(argName) + attrName);
	return table;
}

shared_ptr<ClauseArg> WithObject::getSynonymObject() {
	return synonym;
}

shared_ptr<QueryResultsTable> WithObject::processTrivialTable(shared_ptr<DataAccessLayer> dataAccessLayer, string attrName, ENTITY entity) {
	unordered_set<string> PKBdata = dataAccessLayer->getEntity(entity);
	StringMap attrMap;
	for (string procName : PKBdata) {
		attrMap[procName] = { procName };
	}

	string_view argName = getSynonymObject()->getArgValue();

	return returnWithTable(argName, attrName, attrMap);
}

shared_ptr<QueryResultsTable> ProcNameObject::callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer) {
	ENTITY type = getSynonymObject()->getSynonym()->getEntityType();

	if (type == PROCEDURE) {
		return processTrivialTable(dataAccessLayer, attrName, PROCEDURE);
	}
	else if (type == CALL) {
		StringMap PKBdata = dataAccessLayer->getCallProcNames();
		string_view argName = getSynonymObject()->getArgValue();
		return returnWithTable(argName, attrName, PKBdata);
	}

	return QueryResultsTable::createEmptyTable();
};

shared_ptr<QueryResultsTable> VarNameObject::callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer) {
	ENTITY type = getSynonymObject()->getSynonym()->getEntityType();

	if (type == VARIABLE) {
		return processTrivialTable(dataAccessLayer, attrName, VARIABLE);
	}
	else if (type == READ) {
		StringMap PKBdata = dataAccessLayer->getReadVarNames();
		string_view argName = getSynonymObject()->getArgValue();
		return returnWithTable(argName, attrName, PKBdata);
	}
	else if (type == PRINT) {
		StringMap PKBdata = dataAccessLayer->getPrintVarNames();
		string_view argName = getSynonymObject()->getArgValue();
		return returnWithTable(argName, attrName, PKBdata);
	}

	return QueryResultsTable::createEmptyTable();
};

shared_ptr<QueryResultsTable> ValueObject::callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer) {
	ENTITY type = getSynonymObject()->getSynonym()->getEntityType();

	if (type == CONSTANT) {
		return processTrivialTable(dataAccessLayer, attrName, CONSTANT);
	}

	return QueryResultsTable::createEmptyTable();
};

shared_ptr<QueryResultsTable> StmtNoObject::callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer) {
	ENTITY type = getSynonymObject()->getSynonym()->getEntityType();
	unordered_set<ENTITY> validStmtEntities{ STMT, READ, PRINT, ASSIGN, CALL, WHILE, IF };
	if (validStmtEntities.find(type) != validStmtEntities.end()) {
		return processTrivialTable(dataAccessLayer, attrName, type);
	}
	return QueryResultsTable::createEmptyTable();
};