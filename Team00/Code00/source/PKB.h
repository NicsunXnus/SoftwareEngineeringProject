#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>

#include "PKB/Insertor.h"
#include "PKB/StorageManager.h"
#include "PKB/Responder.h"

using namespace std;


/**
* This class represents the PKB component.
* Comprises 3 main sub-components: Insertor, StorageManager, Responder
*/
class PKB {
public:
	static Insertor insertor;
	static Responder responder;

private:
	static StorageManager storage_manager;
};
