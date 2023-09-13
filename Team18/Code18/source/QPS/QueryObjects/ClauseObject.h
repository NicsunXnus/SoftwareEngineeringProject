#ifndef CLAUSEOBJECT_H
#define CLAUSEOBJECT_H

#include <string_view>
#include "QueryObject.h"
#include "../DataAccessLayer.h"
#include "../../Constants/QPSPKB.h"

/*
* This class represents a Query object, for clause entities
*/
class ClauseObject : public QueryObject {
private:
	map<string, vector<string>> res;
public:
	ClauseObject(string_view tokenName)
		: QueryObject{ tokenName } {

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
	UsesObject(string_view tokenName)
		: ClauseObject{ tokenName } {
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
	ModifiesObject(string_view tokenName)
		: ClauseObject{ tokenName } {
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
	FollowsObject(string_view tokenName)
		: ClauseObject{ tokenName } {
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
	FollowsStarObject(string_view tokenName)
		: ClauseObject{ tokenName } {
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
	ParentObject(string_view tokenName)
		: ClauseObject{ tokenName } {
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
	ParentStarObject(string_view tokenName)
		: ClauseObject{ tokenName } {
	};
	void call(shared_ptr<DataAccessLayer> dataAccessLayer) override {
		setResult(dataAccessLayer->getClause());
	}

};

#endif
