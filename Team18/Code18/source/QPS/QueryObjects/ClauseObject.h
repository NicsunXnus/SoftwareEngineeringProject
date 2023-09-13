#pragma once
#ifndef CLAUSEOBJECT_H
#define CLAUSEOBJECT_H

#include <string_view>
#include "QueryObject.h"

/*
* This class represents a Query object, for clause entities
*/
class ClauseObject : public QueryObject {
private:
	map<string, vector<string>> res;
public:
	ClauseObject(vector<string_view> data)
		: QueryObject{ data } {

	}

	void setResult(variant<vector<string>, map<string, vector<string>>> result) override {
		res = get<map<string, vector<string>>>(result);
	}

	variant<vector<string>, map<string, vector<string>>> getResult() override {
		return res;
	}
};

/*
* This class represents a Query object, for clause Uses
*/
class UsesObject : public ClauseObject {
public:
	UsesObject(vector<string_view> data)
		: ClauseObject{ data } {
	};
	void call(shared_ptr<DataAccessLayer> dataAccessLayer) override {
		setResult(dataAccessLayer->getClause());
	}

};

/*
* This class represents a Query object, for clause Modifies
*/
class ModifiesObject : public ClauseObject {
public:
	ModifiesObject(vector<string_view> data)
		: ClauseObject{ data } {
	};
	void call(shared_ptr<DataAccessLayer> dataAccessLayer) override {
		setResult(dataAccessLayer->getClause());
	}

};

/*
* This class represents a Query object, for clause Follows
*/
class FollowsObject : public ClauseObject {
public:
	FollowsObject(vector<string_view> data)
		: ClauseObject{ data } {
	};
	void call(shared_ptr<DataAccessLayer> dataAccessLayer) override {
		setResult(dataAccessLayer->getClause());
	}

};

/*
* This class represents a Query object, for clause Follows*
*/
class FollowsStarObject : public ClauseObject {
public:
	FollowsStarObject(vector<string_view> data)
		: ClauseObject{ data } {
	};
	void call(shared_ptr<DataAccessLayer> dataAccessLayer) override {
		setResult(dataAccessLayer->getClause());
	}

};

/*
* This class represents a Query object, for clause Parent
*/
class ParentObject : public ClauseObject {
public:
	ParentObject(vector<string_view> data)
		: ClauseObject{ data } {
	};
	void call(shared_ptr<DataAccessLayer> dataAccessLayer) override {
		setResult(dataAccessLayer->getClause());
	}

};

/*
* This class represents a Query object, for clause Parent*
*/
class ParentStarObject : public ClauseObject {
public:
	ParentStarObject(vector<string_view> data)
		: ClauseObject{ data } {
	};
	void call(shared_ptr<DataAccessLayer> dataAccessLayer) override {
		setResult(dataAccessLayer->getClause());
	}

};

#endif
