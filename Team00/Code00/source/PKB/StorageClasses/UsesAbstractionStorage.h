#include <string>

#include "AbstractionStorage.h"


class UsesAbstractionStorage : public AbstractionStorage {
private:
    map<string, vector<int>>* database;
};