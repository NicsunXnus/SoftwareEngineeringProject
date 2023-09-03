#include <string>

#include "AbstractionStorage.h"


class FollowsAbstractionStorage : public AbstractionStorage {
private:
    map<int, vector<int>>* database;
};