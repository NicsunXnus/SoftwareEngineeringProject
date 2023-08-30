#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
typedef short PROC;

class ASTNode;

class VarTable;  // no need to #include "VarTable.h" as all I need is pointer

class PKB {
public:
	static VarTable* varTable; 
	static int setProcToAST(PROC p, ASTNode* r);
	static ASTNode* getRootAST (PROC p);

};