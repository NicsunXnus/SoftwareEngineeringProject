#ifndef CACHE_H
#define CACHE_H


#include <unordered_map>
#include <iostream>


using namespace std;

// Abstract base class representing a cache
template <typename KeyType, typename ValueType>
class Cache {
public:
    virtual bool contains(KeyType key) = 0;
    virtual void insert(KeyType key, ValueType value) = 0;
    virtual ValueType get(KeyType key) = 0;
    virtual void clear() = 0;
};

#endif