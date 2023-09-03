#include <string>

#include "AbstractionStorage.h"


class ModifiesAbstractionStorage : public AbstractionStorage {
private:
    map<string, vector<int>>* database;
};