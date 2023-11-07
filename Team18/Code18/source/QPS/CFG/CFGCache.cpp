#include "CFGCache.h"

bool CFGCache::contains(string key) {
    return cache.find(key) != cache.end();
}

void CFGCache::insert(string key, shared_ptr<ExtendedCFG> value) {
    cache[key] = value;
}

shared_ptr<ExtendedCFG> CFGCache::get(string key) {
    return cache[key];
}

void CFGCache::clear() {
    cache.clear();
}

string CFGCache::generateCacheName(string objectType, vector<string> args) {
    return CFG_CACHE_STRING;
}