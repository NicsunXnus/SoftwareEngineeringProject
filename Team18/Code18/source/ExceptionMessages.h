#ifndef EXCEPTIONMESSAGES_H
#define EXCEPTIONMESSAGES_H

#include <string>

class ExceptionMessages {
public:
  inline static const std::string extraOpenSep = "{ or ( found without matching } or )";
  inline static const std::string extraCloseSep = "} or ) found without matching { or (";

  inline static const std::string invalidProgramDefinition = "Invalid program definition detected. Extra code found outside of procedures.";
  inline static const std::string invalidProcedureDefinition = "Invalid Procedure defintion detected. Expected: 'procedure <name> {} ...";
  inline static const std::string invalidConditionalDeclaration = "Invalid conditional declaration detected. Expected: <if|while> (<conditionalExp>) ...";
  inline static const std::string invalidCondExp = "Invalid conditional expression detected.";
  inline static const std::string invalidRelExp = "Invalid relational expression detected.";
  inline static const std::string invalidArithExp = "Invalid arithmetic expression detected.";
  inline static const std::string invalidIfDeclaration = "Invalid if statement declaration detected. Expected: if (<conditionalExp>) then ...";
  inline static const std::string invalidSemicolonStmt = "Invalid semicolon statement detected.";
  inline static const std::string invalidAssignmentStmt = "Invalid assignment statement detected.";
  inline static const std::string invalidIdentifier = "Invalid identifier detected.";
  inline static const std::string invalidTwoWordStmt = "Invalid two-word statement detected.";
  inline static const std::string invalidToken = "Invalid string detected while trying to tokenize.";

  inline static const std::string emptyProgramGiven =
      "Program provided is empty";
  inline static const std::string emptyProcedureGiven =
      "Procedure provided is empty";
  inline static const std::string emptyStatementListGiven =
      "Statement list provided is empty";
  inline static const std::string emptyStatementGiven =
      "Statement provided is empty";

  inline static const std::string missingConditionalDeclaration =
      "No conditional declaration before curly braces {} detected";
  inline static const std::string missingElseBlock =
      "Else Block not found after an if-then block";
  inline static const std::string missingElseKeyword =
      "Else keyword not found between the blocks";

  inline static const std::string endIndexLarger = "End index is larger than start index";
  inline static const std::string negativeStartIndex = "Start index cannot be negative";

  inline static const std::string wrongNumberChildren = "Supplied number of children for this node is invalid";
  inline static const std::string consecutiveVariablesOrConstants = "Consecutive variables or constants";
  inline static const std::string invalidOperation = "Invalid operation supplied";
  inline static const std::string missingOperatee = "Missing operatee";
  inline static const std::string missingOperator = "Missing operator";
  inline static const std::string missingBrackets = "Missing brackets";
  inline static const std::string emptyBrackets = "Empty brackets found";
  inline static const std::string extraBrackets = "Extra brackets found";
  inline static const std::string closeBeforeOpenParen = ") before (";
  inline static const std::string extraOpenParen = "( found without matching )";
  inline static const std::string extraCloseParen = ") found without matching (";
};
#endif
