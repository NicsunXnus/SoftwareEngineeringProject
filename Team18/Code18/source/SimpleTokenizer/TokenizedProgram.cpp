#include "TokenizedProgram.h"

bool TokenizedProgram::equalsTo(TokenizedProgram& rhs) {
  if (this->procedures.size() != rhs.procedures.size()) {
    return false;
  }
  for (int i = 0; i < this->procedures.size(); i++) {
    if ((*(this->procedures[i])).equalsTo(*(rhs.procedures[i]))) return false;
  }
  return true;
}