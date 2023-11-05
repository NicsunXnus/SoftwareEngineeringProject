#ifndef EXCEPTIONMESSAGES_H
#define EXCEPTIONMESSAGES_H

#include <string>

using namespace std;

class ExceptionMessages {
public:
  inline static const string extraOpenSep = "{ or ( found without matching } or )";
  inline static const string extraCloseSep = "} or ) found without matching { or (";

  inline static const string invalidProgramDefinition = "Invalid program definition detected. Extra code found outside of procedures.";
  inline static const string invalidProcedureDefinition = "Invalid Procedure defintion detected. Expected: 'procedure <name> {} ...";
  inline static const string invalidConditionalDeclaration = "Invalid conditional declaration detected. Expected: <if|while> (<conditionalExp>) ...";
  inline static const string invalidCondExp = "Invalid conditional expression detected.";
  inline static const string invalidRelExp = "Invalid relational expression detected.";
  inline static const string invalidArithExp = "Invalid arithmetic expression detected.";
  inline static const string invalidIfDeclaration = "Invalid if statement declaration detected. Expected: if (<conditionalExp>) then ...";
  inline static const string invalidSemicolonStmt = "Invalid semicolon statement detected.";
  inline static const string invalidAssignmentStmt = "Invalid assignment statement detected.";
  inline static const string invalidIdentifier = "Invalid identifier detected.";
  inline static const string invalidTwoWordStmt = "Invalid two-word statement detected.";
  inline static const string invalidToken = "Invalid string detected while trying to tokenize.";

  inline static const string emptyProgramGiven =
      "Program provided is empty";
  inline static const string emptyProcedureGiven =
      "Procedure provided is empty";
  inline static const string emptyStatementListGiven =
      "Statement list provided is empty";
  inline static const string emptyStatementGiven =
      "Statement provided is empty";

  inline static const string missingConditionalDeclaration =
      "No conditional declaration before curly braces {} detected";
  inline static const string missingElseBlock =
      "Else Block not found after an if-then block";
  inline static const string missingElseKeyword =
      "Else keyword not found between the blocks";

  inline static const string endIndexLarger = "End index is larger than start index";
  inline static const string negativeStartIndex = "Start index cannot be negative";

  inline static const string wrongNumberChildren = "Supplied number of children for this node is invalid";
  inline static const string consecutiveVariablesOrConstants = "Consecutive variables or constants";
  inline static const string invalidOperation = "Invalid operation supplied";
  inline static const string missingOperatee = "Missing operatee";
  inline static const string missingOperator = "Missing operator";
  inline static const string missingBrackets = "Missing brackets";
  inline static const string emptyBrackets = "Empty brackets found";
  inline static const string extraBrackets = "Extra brackets found";
  inline static const string closeBeforeOpenParen = ") before (";
  inline static const string extraOpenParen = "( found without matching )";
  inline static const string extraCloseParen = ") found without matching (";
};

#endif