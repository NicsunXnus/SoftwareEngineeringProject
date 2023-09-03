#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>

#include "Insertor.h"
#include "StorageManager.h"
#include "Responder.h"

using namespace std;


/**
* This class represents the PKB component.
* Comprises 3 main sub-components: Insertor, StorageManager, Responder
*/
class PKB {
public:
	// methods that call Insertor
	static int setAST(TNode* r);


	// methods that call Responder

private:
	static Insertor insertor; 
	static StorageManager storage_manager;
	static Responder responder;
};
