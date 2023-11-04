#ifndef TOKENIZEDPROGRAM_H
#define TOKENIZEDPROGRAM_H

#include <memory>
#include <vector>
#include "ProcessedProcedure.h"

using namespace std;

class ProcessedProgram;
class Extractor;

/// <summary>
/// Abstraction for a vector of Procedures
/// </summary>
class ProcessedProgram : public enable_shared_from_this<ProcessedProgram> {
private:
  // no need shared_ptr of vector because copying over pointers is not memory intensive.
  vector<shared_ptr<ProcessedProcedure>> procedures;

public:
  static bool checkEquality(shared_ptr<ProcessedProgram> left, shared_ptr<ProcessedProgram> right);

  ProcessedProgram(vector<shared_ptr<ProcessedProcedure>>  procedures) : procedures{ procedures } {};

  vector<shared_ptr<ProcessedProcedure>> getAllProcedures();

  void accept(shared_ptr<Extractor> extractor);
};

#endif
