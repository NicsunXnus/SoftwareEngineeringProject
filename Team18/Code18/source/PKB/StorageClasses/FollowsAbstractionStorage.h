#pragma once
#include <string>

#include "AbstractionStorage.h"

// This class represents the storage for the Follows/Follows* design abstractions.
// key: line number
// vector of values : all line numbers that follows* .The first element of the vector will be the one that follows.
class FollowsAbstractionStorage : public AbstractionStorage {

};