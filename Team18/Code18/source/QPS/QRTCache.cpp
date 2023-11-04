#include "QRTCache.h"

using namespace std;


bool QRTCache::contains(string key) {
    return cache.find(key) != cache.end();
}

void QRTCache::insert(string key, shared_ptr<QueryResultsTable> value) {
    cache[key] = value;
}

shared_ptr<QueryResultsTable> QRTCache::get(string key) {
    return cache[key];
}

void QRTCache::clear() {
    cache.clear();
}

string QRTCache::generateCacheName(string objectType, vector<string> args) {
    string result = objectType;
    for (string arg : args) {
        result += " " + arg;
    }
    return result;
}
