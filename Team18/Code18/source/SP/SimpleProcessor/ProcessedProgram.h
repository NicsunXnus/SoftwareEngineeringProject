#ifndef TOKENIZEDPROGRAM_H
#define TOKENIZEDPROGRAM_H

#include <memory>
#include <vector>
#include "ProcessedProcedure.h"

using namespace std;

class Extractor;
class ProcessedProgram;

/// <summary>
/// Abstraction for a vector of Procedures
/// </summary>
class ProcessedProgram : public enable_shared_from_this<ProcessedProgram> {
private:
  // no need shared_ptr of vector because copying over pointers is not memory intensive.
  vector<shared_ptr<ProcessedProcedure>> procedures;
public:
  ProcessedProgram(vector<shared_ptr<ProcessedProcedure>>  procedures) : procedures{ procedures } {};

  std::vector<shared_ptr<ProcessedProcedure>>  getAllProcedures() {
    return this->procedures;
  }

  static bool checkEquality(shared_ptr<ProcessedProgram> left, shared_ptr<ProcessedProgram> right);

  void accept(shared_ptr<Extractor> extractor);
};

#endif
