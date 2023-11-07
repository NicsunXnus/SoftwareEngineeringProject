#pragma once

#include "Insertor.h"
#include "Responder.h"


/**
 * This class represents the PKB component.
 * Comprises 3 main sub-components: Insertor, StorageManager, Responder
 */
class PKB {
 public:
   static inline Insertor insertor = Insertor();
   static inline Responder responder = Responder();

 private:
   static inline StorageManager storage_manager = StorageManager();
};
