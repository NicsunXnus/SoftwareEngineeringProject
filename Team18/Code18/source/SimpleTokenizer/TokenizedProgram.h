#ifndef TOKENIZEDPROGRAM_H
#define TOKENIZEDPROGRAM_H

#include <memory>
#include <vector>

#include "TokenizedProcedure.h"

using namespace std::string_view_literals;

class TokenizedProgram;

/// <summary>
/// Abstraction for a vector of Procedures
/// </summary>
class TokenizedProgram {
 private:
  // no need shared_ptr of vector because copying over pointers is not memory
  // intensive.
  std::vector<std::shared_ptr<TokenizedProcedure>> procedures;

 public:
  TokenizedProgram(std::vector<std::shared_ptr<TokenizedProcedure>> procedures)
      : procedures{procedures} {};

  std::vector<std::shared_ptr<TokenizedProcedure>> getAllProcedures() {
    return this->procedures;
  }

  bool equalsTo(TokenizedProgram& rhs);
};

#endif
