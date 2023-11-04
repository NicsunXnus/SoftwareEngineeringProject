#include "ProcessedProgram.h"

bool ProcessedProgram::checkEquality(shared_ptr<ProcessedProgram> left, shared_ptr<ProcessedProgram> right) {
  if (left->procedures.size() != right->procedures.size()) {
    return false;
  }
  for (int i = 0; i < left->procedures.size(); i++) {
    if (ProcessedProcedure::checkEquality(left->procedures[i], right->procedures[i])) {
      return false;
    }
  }
  return true;
}

void ProcessedProgram::accept(shared_ptr<Extractor> extractor) {
  extractor->extract(shared_from_this());
}