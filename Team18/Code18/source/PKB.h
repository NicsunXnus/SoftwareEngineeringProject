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
	inline static Insertor insertor;
	inline static Responder responder;

private:
	inline static StorageManager storage_manager;
};

