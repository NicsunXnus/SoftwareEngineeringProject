#ifndef EXCEPTIONMESSAGES_H
#define EXCEPTIONMESSAGES_H

#include <string>

class ExceptionMessages {
public:
  inline static const std::string extraOpenCurly = "{ found without matching }";
  inline static const std::string extraCloseCurly = "} found without matching {";

  inline static const std::string invalidProcedureDefinition = "Invalid Procedure defintion detected. Expected: 'procedure <name> {} ...";
  inline static const std::string invalidConditionalDeclaration = "Invalid conditional declaration detected. Expected: <if|while> (<conditionalExp>) ...";
  inline static const std::string invalidConditionalExp = "Invalid relational expression detected. Expected: <arithmetic> <relationalOp> <arithmetic>";
  inline static const std::string invalidIfDeclaration = "Invalid if statement declaration detected. Expected: if (<conditionalExp>) then ...";

  inline static const std::string emptyProgramGiven = "Program provided is empty";
  inline static const std::string emptyProcedureGiven = "Procedure provided is empty";
  inline static const std::string emptyStatementListGiven = "Statement list provided is empty";
  inline static const std::string emptyStatementGiven = "Statement provided is empty";

  inline static const std::string missingConditionalDeclaration = "No conditional declaration before curly braces {} detected";
  inline static const std::string missingElseBlock = "Else Block not found after an if-then block";
  inline static const std::string missingElseKeyword = "Else keyword not found between the blocks";
};
#endif
