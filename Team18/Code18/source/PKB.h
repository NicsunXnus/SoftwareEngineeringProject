#pragma once

#include "PKB/Adapter.h"
#include "PKB/Insertor.h"
#include "PKB/Responder.h"
#include "PKB/StorageManager.h"

/**
 * This class represents the PKB component.
 * Comprises 3 main sub-components: Insertor, StorageManager, Responder
 */
class PKB {
 public:
  static inline Insertor insertor;
  static inline Responder responder;

 private:
  static inline StorageManager storage_manager;
};
