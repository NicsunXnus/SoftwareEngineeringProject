#include <map>
#include <string>
#include <iostream>

#include "AbstractionStorage.h"


using namespace std;

void AbstractionStorage::insert_one(const KeyValue& key, const int& value)
{
	// to be implemented
}

map<KeyValue, vector<int>>* AbstractionStorage::getDatabase()
{
	return database;
}