#pragma once
#ifndef CLAUSEOBJECTFACTORY_H
#define CLAUSEOBJECTFACTORY_H

#include <string_view>
#include "ClauseObject.h"
#include "QueryObjectFactory.h"


class ClauseObjectFactory : public QueryObjectFactory {
public:
	ClauseObjectFactory() : QueryObjectFactory{} {

	}
};

/*
* This class represents a Query Object Factory, for clause Uses
*/
class UsesObjectFactory : public ClauseObjectFactory {
public:
	UsesObjectFactory() {

	};

	shared_ptr<QueryObject> create(vector<string_view> data) override {
		return make_shared<UsesObject>(data);
	}

};

/*
* This class represents a Query Object Factory, for clause Modifies
*/
class ModifiesObjectFactory : public ClauseObjectFactory {
public:
	ModifiesObjectFactory() {

	}; 

	shared_ptr<QueryObject> create(vector<string_view> data) override {
		return make_shared<ModifiesObject>(data);
	}

};

/*
* This class represents a Query Object Factory, for clause Follows
*/
class FollowsObjectFactory : public ClauseObjectFactory {
public:
	FollowsObjectFactory() {

	};

	shared_ptr<QueryObject> create(vector<string_view> data) override {
		return make_shared<FollowsObject>(data);
	}

};

/*
* This class represents a Query Object Factory, for clause Follows*
*/
class FollowsStarObjectFactory : public ClauseObjectFactory {
public:
	FollowsStarObjectFactory() {

	};

	shared_ptr<QueryObject> create(vector<string_view> data) override {
		return make_shared<FollowsStarObject>(data);
	}

};

/*
* This class represents a Query Object Factory, for clause Parent
*/
class ParentObjectFactory : public ClauseObjectFactory {
public:
	ParentObjectFactory() {

	};

	shared_ptr<QueryObject> create(vector<string_view> data) override {
		return make_shared<ParentObject>(data);
	}

};

/*
* This class represents a Query Object Factory, for clause Parent*
*/
class ParentStarObjectFactory : public ClauseObjectFactory {
public:
	ParentStarObjectFactory() {

	};

	shared_ptr<QueryObject> create(vector<string_view> data) override {
		return make_shared<ParentStarObject>(data);
	}

};

#endif