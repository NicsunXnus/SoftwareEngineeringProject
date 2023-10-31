#include "NotQueryObject.h"

shared_ptr<QueryResultsTable> NotQueryObject::callAndProcess(
    shared_ptr<DataAccessLayer> dataAccessLayer) {
  // Get the original table
  auto table = originalQueryObject->callAndProcess(dataAccessLayer);
  int syn_count = getSynonymCount();

  if (syn_count == 0) {  // No synonyms: inverse the bool
    table->flipSignificant();
    return table;

  } else if (syn_count == 1) {  // 1 synonym
    // Get full table of synonym arg, then set diff with original clause
    // e.g: not p1.procName = "test" -> p1.procName - (p1.procName = "test")
    auto syn_table = getFirstSynonym()->callAndProcess(dataAccessLayer);
    return table->difference(syn_table);

  } else {  // 2 synonyms
    // Get full table of both syn args, then set diff with the
    // original clause e.g: not p1.procName = p2.procName -> (p1.procName x
    // p2.procName) - (p1.procName = p2.procName)
    auto syn1_table = getFirstSynonym()->callAndProcess(dataAccessLayer);
    auto syn2_table = getSecondSynonym()->callAndProcess(dataAccessLayer);
    return table->difference(syn1_table, syn2_table);
  }
}