#include "ProcessedStmt.h"
#include "../DesignExtractor/Extractor.h"
#include "../DesignExtractor/ProcedureLineNumberExtractor.h"
#include "../DesignExtractor/NextExtractor.h"


bool ProcessedStmt::equalsTo(ProcessedStmt& rhs) {
  return this->statementNumber == rhs.statementNumber;
}
