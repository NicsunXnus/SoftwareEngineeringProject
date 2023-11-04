#ifndef QRTCACHE_H
#define QRTCACHE_H

#include "../Cache.h"
#include "QueryResultsTable.h"

using namespace std;

/*
* This class represents the cache for QueryResultsTable
*/
class QRTCache : public Cache<string, shared_ptr<QueryResultsTable>> {
private:
    unordered_map<string, shared_ptr<QueryResultsTable>> cache;
public:
    bool contains(string key) override;

    void insert(string key, shared_ptr<QueryResultsTable> value) override;

    shared_ptr<QueryResultsTable> get(string key) override;

    void clear() override;

    static string generateCacheName(string objectType, vector<string> args);
};

#endif