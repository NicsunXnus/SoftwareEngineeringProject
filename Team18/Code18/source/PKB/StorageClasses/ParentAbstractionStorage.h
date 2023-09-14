#pragma once
#include <string>

#include "AbstractionStorage.h"

// This class represents the storage for the Parent/Parent* design abstractions.
// key: line number
// vector of values : all line numbers that are parent* (parent, grandparent etc.).The first element of the vector will be the one that is the direct parent.
class ParentAbstractionStorage : public AbstractionStorage {
};