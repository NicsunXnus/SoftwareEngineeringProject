#include "../DesignExtractor/Extractor.h"
#include "ProcessedProgram.h"

bool ProcessedProgram::equalsTo(ProcessedProgram& rhs) {
  if (this->procedures.size() != rhs.procedures.size()) {
    return false;
  }
  for (int i = 0; i < this->procedures.size(); i++) {
    if ((*(this->procedures[i])).equalsTo(*(rhs.procedures[i]))) return false;
  }
  return true;
}

void ProcessedProgram::accept(std::shared_ptr<Extractor> extractor) {
  extractor->extract(shared_from_this());
}