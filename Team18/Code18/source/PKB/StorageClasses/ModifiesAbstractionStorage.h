#pragma once
#include <string>

#include "AbstractionStorage.h"

// This class represents the storage for the Modifies design abstractions.
// key: variable v
// vector of values : line numbers(for non - procedure) or procedure names
// essentially, in a typical Modifies(x, v), v will be the key and x will be the possible values in the vector.
class ModifiesAbstractionStorage : public AbstractionStorage {
};