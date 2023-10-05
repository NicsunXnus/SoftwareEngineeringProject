#pragma once
#ifndef CLAUSEOBJECT_H
#define CLAUSEOBJECT_H

#include <string_view>
#include "QueryObject.h"
#include "ClauseArg.h"

/*
* This class represents a Query object, for clause entities
*/
class ClauseObject : public QueryObject {
private:
	shared_ptr<ClauseArg> argument0;
	shared_ptr<ClauseArg> argument1;

public:
	ClauseObject(string_view clauseName, shared_ptr<ClauseArg> argument0, shared_ptr<ClauseArg> argument1)
		: QueryObject{ clauseName }, argument0{ argument0 }, argument1{ argument1 } {
	}

	// Constructor for creating an empty clause object, to be used in gracefully handling clauses with errors.
	ClauseObject(string_view name) : QueryObject{ name } {}


	shared_ptr<ClauseArg> getArg1() {
		return argument0;
	}

	shared_ptr<ClauseArg> getArg2() {
		return argument1;
	}

	
	
};
inline string col1 = "col1";
inline string col2 = "col2";



inline shared_ptr<QueryResultsTable> filterStmtRef(shared_ptr<ClauseArg> arg, string colName, shared_ptr<QueryResultsTable> table
	, shared_ptr<DataAccessLayer> dataAccessLayer, unordered_map<string_view, shared_ptr<QueryObject>> synonyms) {
	if (arg->isInteger()) {
		// filter out rows where row val from colName == integer, REMOVE THE ENTIRE COLUMN AFTERWARDS
		//  if table.cols.size < 1, set isSignificant to true or false depending on table.rows.size > 0 or not
		vector<string> targets;

		targets.push_back(svToString(arg->getArg()));
		shared_ptr<QueryResultsTable> filteredTable = table->filter(colName, targets);
		filteredTable->deleteColumn(colName);
		return filteredTable;

	}
	else if (arg->isSynonym()) {
		// get synoynm type. if stmt, then do nothing, else, cross join with select synonym
		ENTITY type = arg->getSynonym()->getEntityType();
		// RENAME colName to synonym name
		table->renameColumn(svToString(arg->getArg()), colName);
		if (type == STMT) {
			// do nothing, RENAME colName to stmt synonym name		
			return table;
		}
		else {
			// call select synonym and inner join (need similar col names though, )
			shared_ptr<QueryObject> synoynm = synonyms.at(arg->getArg());
			shared_ptr<QueryResultsTable> synonymQueryObject = synoynm->callAndProcess(dataAccessLayer, synonyms);
			shared_ptr<QueryResultsTable> innerJoined = table->innerJoin(synonymQueryObject);
			return innerJoined;
		}
	}
	else {
		// wildcard, drop column, if table.cols.size < 1, set isSignificant to true or false depending on table.rows.size > 0 or not
		table->deleteColumn(colName);
	}
	return table;
}

inline shared_ptr<QueryResultsTable> filterEntRef(shared_ptr<ClauseArg> arg, string colName, shared_ptr<QueryResultsTable> table
	, shared_ptr<DataAccessLayer> dataAccessLayer, unordered_map<string_view, shared_ptr<QueryObject>> synonyms) {
	if (arg->isIdentifier()) {
		// filter out rows where row val from colName == IDENTIFIER, REMOVE THE ENTIRE COLUMN AFTERWARDS
		// if table.cols.size < 1, set isSignificant to true or false depending on table.rows.size > 0 or not
		vector<string> targets;

		targets.push_back(svToString(arg->getIdentifier()));
		shared_ptr<QueryResultsTable> filteredTable = table->filter(colName, targets);
		filteredTable->deleteColumn(colName);

		return filteredTable;

	}
	else if (arg->isSynonym()) {
		// get synoynm type. if stmt, then do nothing, else, cross join with select synonym
		ENTITY type = arg->getSynonym()->getEntityType();
		// RENAME colName to synonym name
		table->renameColumn(svToString(arg->getArg()), colName);
		if (type == STMT) {
			// do nothing, RENAME colName to stmt synonym name
			return table;
		}
		else {
			// call select synonym and inner join (need similar col names though, )
			shared_ptr<QueryObject> synoynm = synonyms.at(arg->getArg());
			shared_ptr<QueryResultsTable> synonymQueryObject = synoynm->callAndProcess(dataAccessLayer, synonyms);
			shared_ptr<QueryResultsTable> innerJoined = table->innerJoin(synonymQueryObject);
			return innerJoined;
		}
	}
	else {
		// wildcard, drop column, if table.cols.size < 1, set isSignificant to true or false depending on table.rows.size > 0 or not
		table->deleteColumn(colName);
	}
	return table;
}

inline shared_ptr<QueryResultsTable> handleFollowsParents(shared_ptr<ClauseArg> arg1, shared_ptr<ClauseArg> arg2, shared_ptr<QueryResultsTable> table
	, shared_ptr<DataAccessLayer> dataAccessLayer, unordered_map<string_view, shared_ptr<QueryObject>> synonyms) {
	// not valid, as something like Follows(a, a) should return an empty table
	if (arg1->isSynonym() && arg2->isSynonym() && arg1->getArg() == arg2->getArg()) {
		table->deleteColumn(col1);
		table->deleteColumn(col2);
		table->setSignificant(false);
		return table;
	}
	shared_ptr<QueryResultsTable> filterFirstArg = filterStmtRef(arg1, col1, table, dataAccessLayer, synonyms);
	shared_ptr<QueryResultsTable> filterSecondArg = filterStmtRef(arg2, col2, filterFirstArg, dataAccessLayer, synonyms);
	return filterSecondArg;
}


inline shared_ptr<QueryResultsTable> handleModifiesUses(shared_ptr<ClauseArg> arg1, shared_ptr<ClauseArg> arg2, shared_ptr<QueryResultsTable> table
	, shared_ptr<DataAccessLayer> dataAccessLayer, unordered_map<string_view, shared_ptr<QueryObject>> synonyms) {

	shared_ptr<QueryResultsTable> filterFirstArg = filterStmtRef(arg1, col1, table, dataAccessLayer, synonyms);
	shared_ptr<QueryResultsTable> filterSecondArg = filterEntRef(arg2, col2, filterFirstArg, dataAccessLayer, synonyms);
	return filterSecondArg;
}





/*
* This class represents a Query object, for clause Uses with a statement ref as the first argument
*/
class UsesObject : public ClauseObject {
public:
	UsesObject(string_view clauseName, shared_ptr<ClauseArg> argument0, shared_ptr<ClauseArg> argument1)
		: ClauseObject{ clauseName, argument0, argument1 } {
	};

	shared_ptr<QueryResultsTable> callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer, unordered_map<string_view, shared_ptr<QueryObject>> synonyms) override {
		map<string, vector<string>> PKBdata = dataAccessLayer->getClause(USES);
		vector<string> headers;
		headers.push_back(col2);
		headers.push_back(col1);
		// create table with temporary name table headers: col1, col2
		shared_ptr<QueryResultsTable> table = QueryResultsTable::createTable(headers, PKBdata);
		return handleModifiesUses(getArg1(), getArg2(), table, dataAccessLayer, synonyms);
	}

	
};

/*
* This class represents a Query object, for clause Uses with an entity ref as the first argument
*/
class UsesEntityObject : public ClauseObject {
public:
	UsesEntityObject(string_view clauseName, shared_ptr<ClauseArg> argument0, shared_ptr<ClauseArg> argument1)
		: ClauseObject{ clauseName, argument0, argument1 } {
	};

	shared_ptr<QueryResultsTable> callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer, unordered_map<string_view, shared_ptr<QueryObject>> synonyms) override {
		map<string, vector<string>> PKBdata = dataAccessLayer->getClause(USES);
		vector<string> headers;
		headers.push_back(col2);
		headers.push_back(col1);
		// create table with temporary name table headers: col1, col2
		shared_ptr<QueryResultsTable> table = QueryResultsTable::createTable(headers, PKBdata);
		shared_ptr<QueryResultsTable> filterFirstArg = filterEntRef(getArg1(), col2, table, dataAccessLayer, synonyms);
		shared_ptr<QueryResultsTable> filterSecondArg = filterEntRef(getArg2(), col1, filterFirstArg, dataAccessLayer, synonyms);
		return filterSecondArg;
	}

};

/*
* This class represents a Query object, for clause Modifies with a statement ref as the first argument
*/
class ModifiesObject : public ClauseObject {
public:
	ModifiesObject(string_view clauseName, shared_ptr<ClauseArg> argument0, shared_ptr<ClauseArg> argument1)
		: ClauseObject{ clauseName, argument0, argument1 } {
	};
	shared_ptr<QueryResultsTable> callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer, unordered_map<string_view, shared_ptr<QueryObject>> synonyms) override {
		map<string, vector<string>> PKBdata = dataAccessLayer->getClause(MODIFIES);
		vector<string> headers;
		headers.push_back(col2);
		headers.push_back(col1);
		// create table with temporary name table headers: col1, col2
		shared_ptr<QueryResultsTable> table = QueryResultsTable::createTable(headers, PKBdata);
		return handleModifiesUses(getArg1(), getArg2(), table, dataAccessLayer, synonyms);
	}



};

/*
* This class represents a Query object, for clause Modifies with an entity ref as the first argument
*/
class ModifiesEntityObject : public ClauseObject {
public:
	ModifiesEntityObject(string_view clauseName, shared_ptr<ClauseArg> argument0, shared_ptr<ClauseArg> argument1)
		: ClauseObject{ clauseName, argument0, argument1 } {
	};
	shared_ptr<QueryResultsTable> callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer, unordered_map<string_view, shared_ptr<QueryObject>> synonyms) override {
		map<string, vector<string>> PKBdata = dataAccessLayer->getClause(MODIFIES);
		vector<string> headers;
		headers.push_back(col2);
		headers.push_back(col1);
		// create table with temporary name table headers: col1, col2
		shared_ptr<QueryResultsTable> table = QueryResultsTable::createTable(headers, PKBdata);
		shared_ptr<QueryResultsTable> filterFirstArg = filterEntRef(getArg1(), col2, table, dataAccessLayer, synonyms);
		shared_ptr<QueryResultsTable> filterSecondArg = filterEntRef(getArg2(), col1, filterFirstArg, dataAccessLayer, synonyms);
		return filterSecondArg;
	}



};

/*
* This class represents a Query object, for clause Follows
*/
class FollowsObject : public ClauseObject {
public:
	FollowsObject(string_view clauseName, shared_ptr<ClauseArg> argument0, shared_ptr<ClauseArg> argument1)
		: ClauseObject{ clauseName, argument0, argument1 } {
	};
	shared_ptr<QueryResultsTable> callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer, unordered_map<string_view, shared_ptr<QueryObject>> synonyms) override {
		map<string, vector<string>> PKBdata = dataAccessLayer->getClause(FOLLOWS);
		vector<string> headers;
		headers.push_back(col1);
		headers.push_back(col2);
		// create table with temporary name table headers: col1, col2
		shared_ptr<QueryResultsTable> table = QueryResultsTable::createTable(headers, PKBdata);
		return handleFollowsParents(getArg1(), getArg2(), table, dataAccessLayer, synonyms);
	}


};

/*
* This class represents a Query object, for clause Follows*
*/
class FollowsStarObject : public ClauseObject {
public:
	FollowsStarObject(string_view clauseName, shared_ptr<ClauseArg> argument0, shared_ptr<ClauseArg> argument1)
		: ClauseObject{ clauseName, argument0, argument1 } {
	};
	shared_ptr<QueryResultsTable> callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer, unordered_map<string_view, shared_ptr<QueryObject>> synonyms) override {
		map<string, vector<string>> PKBdata = dataAccessLayer->getClause(FOLLOWSSTAR);
		vector<string> headers;
		headers.push_back(col1);
		headers.push_back(col2);
		// create table with temporary name table headers: col1, col2
		shared_ptr<QueryResultsTable> table = QueryResultsTable::createTable(headers, PKBdata);
		return handleFollowsParents(getArg1(), getArg2(), table, dataAccessLayer, synonyms);
	}

};

/*
* This class represents a Query object, for clause Parent
*/
class ParentObject : public ClauseObject {
public:
	ParentObject(string_view clauseName, shared_ptr<ClauseArg> argument0, shared_ptr<ClauseArg> argument1)
		: ClauseObject{ clauseName, argument0, argument1 } {
	};
	shared_ptr<QueryResultsTable> callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer, unordered_map<string_view, shared_ptr<QueryObject>> synonyms) override {
		map<string, vector<string>> PKBdata = dataAccessLayer->getClause(PARENT);
		vector<string> headers;
		headers.push_back(col2);
		headers.push_back(col1); // swapped due to PKB storage method
		// create table with temporary name table headers: col1, col2
		shared_ptr<QueryResultsTable> table = QueryResultsTable::createTable(headers, PKBdata);
		return handleFollowsParents(getArg1(), getArg2(), table, dataAccessLayer, synonyms);
	}

};

/*
* This class represents a Query object, for clause Parent*
*/
class ParentStarObject : public ClauseObject {
public:
	ParentStarObject(string_view clauseName, shared_ptr<ClauseArg> argument0, shared_ptr<ClauseArg> argument1)
		: ClauseObject{ clauseName, argument0, argument1 } {
	};
	shared_ptr<QueryResultsTable> callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer, unordered_map<string_view, shared_ptr<QueryObject>> synonyms) override {
		map<string, vector<string>> PKBdata = dataAccessLayer->getClause(PARENTSTAR);
		vector<string> headers;
		headers.push_back(col2);
		headers.push_back(col1); // swapped due to PKB storage method
		// create table with temporary name table headers: col1, col2
		shared_ptr<QueryResultsTable> table = QueryResultsTable::createTable(headers, PKBdata);
		return handleFollowsParents(getArg1(), getArg2(), table, dataAccessLayer, synonyms);
	}

};

/*
* This class represents a Query object, for clause Calls
*/
class CallsObject : public ClauseObject {
public:
	CallsObject(string_view clauseName, shared_ptr<ClauseArg> argument0, shared_ptr<ClauseArg> argument1)
		: ClauseObject{ clauseName, argument0, argument1 } {
	};
	shared_ptr<QueryResultsTable> callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer, unordered_map<string_view, shared_ptr<QueryObject>> synonyms) override {
		// TODO
		return make_shared<QueryResultsTable>();
	}

};

/*
* This class represents a Query object, for clause CallsStar
*/
class CallsStarObject : public ClauseObject {
public:
	CallsStarObject(string_view clauseName, shared_ptr<ClauseArg> argument0, shared_ptr<ClauseArg> argument1)
		: ClauseObject{ clauseName, argument0, argument1 } {
	};
	shared_ptr<QueryResultsTable> callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer, unordered_map<string_view, shared_ptr<QueryObject>> synonyms) override {
		// TODO
		return make_shared<QueryResultsTable>();
	}

};

/*
* This class represents a Query object, for clause Next
*/
class NextObject : public ClauseObject {
public:
	NextObject(string_view clauseName, shared_ptr<ClauseArg> argument0, shared_ptr<ClauseArg> argument1)
		: ClauseObject{ clauseName, argument0, argument1 } {
	};
	shared_ptr<QueryResultsTable> callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer, unordered_map<string_view, shared_ptr<QueryObject>> synonyms) override {
		// TODO
		return make_shared<QueryResultsTable>();
	}

};

/*
* This class represents a Query object, for clause Next*
*/
class NextStarObject : public ClauseObject {
public:
	NextStarObject(string_view clauseName, shared_ptr<ClauseArg> argument0, shared_ptr<ClauseArg> argument1)
		: ClauseObject{ clauseName, argument0, argument1 } {
	};
	shared_ptr<QueryResultsTable> callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer, unordered_map<string_view, shared_ptr<QueryObject>> synonyms) override {
		// TODO
		return make_shared<QueryResultsTable>();
	}

};

/*
* This class represents a Query object, for clause Affects
*/
class AffectsObject : public ClauseObject {
public:
	AffectsObject(string_view clauseName, shared_ptr<ClauseArg> argument0, shared_ptr<ClauseArg> argument1)
		: ClauseObject{ clauseName, argument0, argument1 } {
	};
	shared_ptr<QueryResultsTable> callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer, unordered_map<string_view, shared_ptr<QueryObject>> synonyms) override {
		// TODO
		return make_shared<QueryResultsTable>();
	}

};
#endif
