#pragma once

#include "PKB/Insertor.h"
#include "PKB/StorageManager.h"
#include "PKB/Responder.h"
#include "PKB/Adapter.h"

/**
* This class represents the PKB component.
* Comprises 3 main sub-components: Insertor, StorageManager, Responder
*/
class PKB {
  public:
	static inline Insertor insertor;
	static inline Responder responder;
  static inline Adapter adapter;

  private:
    static inline StorageManager storage_manager;
};
