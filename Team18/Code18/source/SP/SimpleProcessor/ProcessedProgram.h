#ifndef TOKENIZEDPROGRAM_H
#define TOKENIZEDPROGRAM_H

#include <memory>
#include <vector>
#include "ProcessedProcedure.h"

using namespace std::string_view_literals;

class Extractor;
class ProcessedProgram;

/// <summary>
/// Abstraction for a vector of Procedures
/// </summary>
class ProcessedProgram : public std::enable_shared_from_this<ProcessedProgram> {
private:
  // no need shared_ptr of vector because copying over pointers is not memory intensive.
  std::vector<std::shared_ptr<ProcessedProcedure>> procedures;
public:
  ProcessedProgram(std::vector<std::shared_ptr<ProcessedProcedure>>  procedures) : procedures{ procedures } {};

  std::vector<std::shared_ptr<ProcessedProcedure>>  getAllProcedures() {
    return this->procedures;
  }

  bool equalsTo(ProcessedProgram& rhs);

  void accept(std::shared_ptr<Extractor> extractor);
};

#endif
