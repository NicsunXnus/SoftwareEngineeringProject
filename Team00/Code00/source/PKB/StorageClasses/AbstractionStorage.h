#ifndef ABSTRACTIONSTORAGE_H
#define ABSTRACTIONSTORAGE_H
#include <map>
#include <vector>
#include <variant>

typedef std::variant<int, std::string> KeyValue;

using namespace std;

/**
* This class is the base class for storage.
* Map has vector<int> as values, but keys can be any type.
*/
class AbstractionStorage {
public:
	map<KeyValue, vector<int>>* getDatabase();

    void print_rows() const;
    
    void insert_one(const KeyValue& key, const int& value);

private:
    map<KeyValue, vector<int>>* database;
};

#endif