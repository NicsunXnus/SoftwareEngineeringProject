#ifndef CFGCACHE_H
#define CFGCACHE_H

#include "../../Cache.h"
#include "CFG.h"
#include "../../Constants/QPSConstants.h"

using namespace std;

class ExtendedCFG;

/*
* This class represents the cache for CFG, for Next and Affects
*/
class CFGCache : public Cache<string, shared_ptr<ExtendedCFG>> {
private:
    unordered_map<string, shared_ptr<ExtendedCFG>> cache;
public:
    bool contains(string key) override;

    void insert(string key, shared_ptr<ExtendedCFG> value) override;

    shared_ptr<ExtendedCFG> get(string key) override;

    void clear() override;

    static string generateCacheName(string objectType, vector<string> args);
};

#endif