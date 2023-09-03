#include <string>

#include "AbstractionStorage.h"


class ParentAbstractionStorage : public AbstractionStorage {
private:
    map<int, vector<int>>* database;
};