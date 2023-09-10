#pragma once
#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

#include "../AST/ASTNode.h"

/**
* This class represents the Abstractions component of the Design Extractor.
*/
class Abstractions {

};

class FollowsAbstraction : public Abstractions {

};

class ParentsAbstraction : public Abstractions {

};

class UsesAbstraction : public Abstractions {

};

class ModifiesAbstraction : public Abstractions {

};