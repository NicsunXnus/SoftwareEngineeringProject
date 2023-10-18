#ifndef SIMPLETOKENIZER_H
#define SIMPLETOKENIZER_H

#include <iostream>
#include <memory>
#include <stack>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "../ExceptionMessages.h"
#include "../HelperFunctions.h"
#include "../SimpleTokens/TokenFactory.h"
#include "TokenizedConditionalStmt.h"
#include "TokenizedProcedure.h"
#include "TokenizedProgram.h"
#include "TokenizedSemicolonStmt.h"
#include "TokenizedStmtList.h"

using namespace std::string_view_literals;

class SimpleTokenizer {
 private:
  // Keeps track of the statement number as the tokenizer results are being
  // generated

  inline static int statementNumber = 0;
  // Detects "{" and "}" in the outermost scope.
  // Throws an exception if there are any mismatching "{" and "}", including
  // within inner scopes. Returns a vector of pair<int, int> pointers. Each pair
  // represents the indexes of a matching "{" and "}" respectively.
  static std::vector<std::shared_ptr<std::pair<int, int>>>
  outermostScopeDetector(std::string input) {
    // stack keeps track of the indexes of the open curly
    std::stack<int> scopeTracker;
    std::vector<std::shared_ptr<std::pair<int, int>>> output;
    for (int i = 0; i < input.size(); i++) {
      char thisChar = input[i];
      if (thisChar == '{') {
        scopeTracker.push(i);
      }
      if (thisChar == '}') {
        if (scopeTracker.empty()) {
          std::cerr << ExceptionMessages::extraCloseCurly << std::endl;
          return std::vector<std::shared_ptr<std::pair<int, int>>>();  // return
                                                                       // empty
        }
        int lastSeen = scopeTracker.top();
        scopeTracker.pop();
        if (scopeTracker.empty()) {
          output.push_back(std::make_shared<std::pair<int, int>>(lastSeen, i));
        }
      }
    }

    if (!scopeTracker.empty()) {
      std::cerr << ExceptionMessages::extraOpenCurly << std::endl;
      return std::vector<std::shared_ptr<std::pair<int, int>>>();  // return
                                                                   // empty
    }
    return output;
  }

  // Tokenizes an expression, split by certain delimiters. Prioritises
  // identifiers for valid names
  static std::vector<std::shared_ptr<Token>> tokenizeExpression(
      std::string expression, std::string delimiter) {
    std::string trimmed = trimWhitespaces(expression);
    std::vector<std::shared_ptr<Token>> output;
    std::vector<std::string> split = splitString(trimmed, delimiter, true);
    for (std::string s : split) {
      // ignore whitespaces. equality check is basically saying: not cannot find
      // so == can find whitespaces are declared in HelperFunctions.h
      if (whitespaces.find(s) != std::string::npos) continue;
      output.push_back(TokenFactory::generateTokenForSimple(s, true));
    }
    return output;
  }

  // Simple enum to be used in the next few functions
  static enum ConditionalDeclaration { IF, WHILE };

  // just evaluates the conditonal expression and what type of conditional
  // statement it is.
  static std::pair<ConditionalDeclaration, std::vector<std::shared_ptr<Token>>>
  processConditionalExpression(std::string trimmed, bool isIf) {
    ConditionalDeclaration dec;
    int afterKeyword;
    int beforeLast;
    if (isIf) {
      dec = ConditionalDeclaration::IF;
      afterKeyword = trimmed.find("if") + 2;  // because if is 2 characters long
      beforeLast =
          trimmed.rfind("then") - 1;  // to get to the character before "then"
    } else {
      dec = ConditionalDeclaration::WHILE;
      afterKeyword =
          trimmed.find("while") + 5;    // because while is 5 characters long
      beforeLast = trimmed.size() - 1;  // last character
    }
    std::string inBetween =
        trimWhitespaces(substring(trimmed, afterKeyword, beforeLast));
    bool parenFormatting = inBetween[0] == '(' && inBetween.back() == ')';
    if (!parenFormatting) {
      std::cerr << ExceptionMessages::invalidConditionalDeclaration
                << std::endl;
      return std::pair<ConditionalDeclaration,
                       std::vector<std::shared_ptr<Token>>>();  // return empty
    }

    std::string delimiter = arithmeticOpsWithWhitespace + "|" + relationalOps;
    // 1 and -2 to remove the parentheses
    std::vector<std::shared_ptr<Token>> condExp = tokenizeExpression(
        substring(inBetween, 1, inBetween.size() - 2), delimiter);

    return std::pair(dec, condExp);
  }

  // Processes and validates the conditional statement declaration of a
  // conditional statement
  static std::pair<ConditionalDeclaration, std::vector<std::shared_ptr<Token>>>
  processConditionalDeclaration(std::string declaration) {
    std::string trimmed = trimWhitespaces(declaration);
    if (trimmed.empty()) {
      std::cerr << ExceptionMessages::emptyStatementGiven << std::endl;
      return std::pair<ConditionalDeclaration,
                       std::vector<std::shared_ptr<Token>>>();  // return empty
    }

    // find first word
    // if "if", find last word, it should be "then"
    // if "while", do nothing
    // finally process condtional exp

    // Determine first word and type of conditional statement
    bool firstWordWhile =
        trimmed.find_first_not_of(whitespaces) == trimmed.find("while");
    bool firstWordIf =
        trimmed.find_first_not_of(whitespaces) == trimmed.find("if");
    bool isIf = false;
    if (firstWordIf) {
      // minus 3 because rfind will find the position of the "t" in "then", but
      // find_last_not_of will find position of "n". minus exactly 3 because
      // "then" is 4 letters, so 3 letter diff between "t" and "n"
      bool lastWordThen =
          (trimmed.find_last_not_of(whitespaces) - 3) == trimmed.rfind("then");
      if (!lastWordThen) {
        std::cerr << ExceptionMessages::invalidIfDeclaration << std::endl;
        return std::pair<ConditionalDeclaration,
                         std::vector<std::shared_ptr<Token>>>();  // return
                                                                  // empty
      }
      isIf = true;
    }
    if (!firstWordIf && !firstWordWhile) {
      std::cerr << ExceptionMessages::invalidConditionalDeclaration
                << std::endl;
      return std::pair<ConditionalDeclaration,
                       std::vector<std::shared_ptr<Token>>>();  // return empty
    }

    return processConditionalExpression(trimmed, isIf);
  }

  // Tokenizes a Statement List, which can be found in either IF/WHILE or
  // Procedures
  static std::shared_ptr<TokenizedStmtList> tokenizeStmtList(
      std::string stmtList) {
    std::string trimmed = trimWhitespaces(std::string(stmtList));
    if (trimmed.empty()) {
      std::cerr << ExceptionMessages::emptyStatementListGiven << std::endl;
      return nullptr;
    }
    std::vector<std::shared_ptr<std::pair<int, int>>> curlyPairs =
        outermostScopeDetector(
            trimmed);  // this should detect { and } for if-then-else and while
    std::vector<std::shared_ptr<TokenizedStmt>> out;

    int prevEnd = 0;
    int pairIndex = 0;
    while (pairIndex < curlyPairs.size()) {
      std::pair<int, int> currPair = *(curlyPairs[pairIndex]);

      /// Process the part that is before the conditional statement: <IF|WHILE>
      /// <conditional> { }
      std::string preCurly = substring(trimmed, prevEnd, currPair.first - 1);
      // this should contain no semicolons and no { }. Should also be at least
      // size 1 due to conditional declaration
      std::vector<std::string> preCurlySplit =
          splitString(preCurly, ";", false);
      if (preCurlySplit.empty()) {
        std::cerr << ExceptionMessages::missingConditionalDeclaration
                  << std::endl;
        return nullptr;
      }
      // tokenize each semicolon statement. Last element is not a semicolon
      // statement due to conditional declaration
      for (int stmtIndex = 0; stmtIndex < preCurlySplit.size() - 1;
           stmtIndex++) {
        out.push_back(tokenizeSemicolonStatement(preCurlySplit[stmtIndex]));
      }

      /// Process the part that is within the conditional statement <IF|WHILE>
      /// <conditional> { }
      std::string lastPreCurly = preCurlySplit.back();
      std::pair<ConditionalDeclaration, std::vector<std::shared_ptr<Token>>>
          processed = processConditionalDeclaration(lastPreCurly);
      ConditionalDeclaration dec = processed.first;

      // Increment and save it here first. Will be used once the IF/WHILE
      // statements' body stmtLists are completely defined.
      SimpleTokenizer::statementNumber += 1;
      const int conditionalStmtNumber = SimpleTokenizer::statementNumber;
      // Tokenized StmtList for either the THEN-BLOCK or WHILE-BLOCK
      std::shared_ptr<TokenizedStmtList> firstBody = tokenizeStmtList(
          substring(trimmed, currPair.first + 1, currPair.second - 1));
      if (dec == ConditionalDeclaration::WHILE) {
        // process statementList within while body
        out.push_back(std::make_shared<TokenizedWhileStmt>(
            conditionalStmtNumber, processed.second, firstBody));
        prevEnd = currPair.second + 1;
      }

      // Look ahead to the next pair to process the ELSE-BLOCK
      if (dec == ConditionalDeclaration::IF) {
        pairIndex += 1;
        if (pairIndex >= curlyPairs.size()) {
          std::cerr << ExceptionMessages::missingElseBlock << std::endl;
          return nullptr;
        }
        std::pair<int, int> nextPair = *(curlyPairs[pairIndex]);
        std::string inBetweenCurly =
            substring(trimmed, currPair.second + 1, nextPair.first - 1);
        bool isElseString = trimWhitespaces(inBetweenCurly) == "else";
        if (!isElseString) {
          std::cerr << ExceptionMessages::missingElseKeyword << std::endl;
          return nullptr;
        }
        std::shared_ptr<TokenizedStmtList> elseBody = tokenizeStmtList(
            substring(trimmed, nextPair.first + 1, nextPair.second - 1));
        out.push_back(std::make_shared<TokenizedIfStmt>(
            conditionalStmtNumber, processed.second, firstBody, elseBody));
        prevEnd = nextPair.second + 1;
      }
      pairIndex += 1;
    }
    if (prevEnd < trimmed.size()) {
      // process any leftover code after all curly braces
      std::string afterAllCurly =
          substring(trimmed, prevEnd, trimmed.size() - 1);
      // this should contain no semicolons and no { }
      std::vector<std::string> afterCurlySplit =
          splitString(afterAllCurly, ";", false);
      // tokenize each semicolon statement.
      for (std::string stmt : afterCurlySplit) {
        out.push_back(tokenizeSemicolonStatement(stmt));
      }
    }
    if (out.empty()) {
      std::cerr << ExceptionMessages::emptyStatementListGiven << std::endl;
      return nullptr;
    }

    return std::make_shared<TokenizedStmtList>(out);
  }

  // Tokenizes statements that end with a semicolon.
  static std::shared_ptr<TokenizedSemicolonStmt> tokenizeSemicolonStatement(
      std::string stmt) {
    std::string trimmed = trimWhitespaces(stmt);
    if (trimmed.empty()) {
      std::cerr << ExceptionMessages::emptyStatementGiven << std::endl;
      return nullptr;
    }

    std::vector<std::shared_ptr<Token>> output;
    bool validStatement = false;

    if (trimmed.find("=") != std::string::npos) {  // Assignment Statement
      output = tokenizeAssignment(trimmed);

      SimpleTokenizer::statementNumber += 1;
      return std::make_shared<TokenizedSemicolonStmt>(
          SimpleTokenizer::statementNumber, output);
    }
    if (splitString(trimmed).size() != 2) {
      std::cerr << ExceptionMessages::invalidSemicolonStmt << std::endl;
      return nullptr;
    }
    // if the substring "read " exists and it is the first word occurence in the
    // statement
    else if (trimmed.find_first_not_of(whitespaces) == trimmed.find("read ")) {
      output = tokenizeTwoWordStmt(trimmed, "read");
      validStatement = true;
    }
    // if the substring "print " exists and it is the first word occurence in
    // the statement
    else if (trimmed.find_first_not_of(whitespaces) == trimmed.find("print ")) {
      output = tokenizeTwoWordStmt(trimmed, "print");
      validStatement = true;
    }

    // if the substring "call " exists and it is the first word occurence in the
    // statement
    else if (trimmed.find_first_not_of(whitespaces) == trimmed.find("call ")) {
      output = tokenizeTwoWordStmt(trimmed, "call");
      validStatement = true;
    }

    if (!validStatement) {
      std::cerr << ExceptionMessages::invalidSemicolonStmt << std::endl;
      return nullptr;
    }
    SimpleTokenizer::statementNumber += 1;
    return std::make_shared<TokenizedSemicolonStmt>(
        SimpleTokenizer::statementNumber, output);
  }

  // Tokenizes an assignment statement
  static std::vector<std::shared_ptr<Token>> tokenizeAssignment(
      std::string assStmt) {
    std::vector<std::shared_ptr<Token>> output;
    std::vector<std::string> splitByEquals = splitString(assStmt, "=");
    if (splitByEquals.size() != 2) {
      // Invalid assignment statement
      std::cerr << ExceptionMessages::invalidAssignmentStmt << std::endl;
      return std::vector<std::shared_ptr<Token>>();  // returns empty
    }
    std::string trimmedLeft = trimWhitespaces(splitByEquals[0]);
    if (!isValidName(trimmedLeft)) {
      std::cerr << ExceptionMessages::invalidIdentifier << std::endl;
      return std::vector<std::shared_ptr<Token>>();  // returns empty
    }
    std::shared_ptr<Token> left =
        TokenFactory::generateTokenForSimple(trimmedLeft, true);
    // arithmeticOpsWithWhitespace is defined in HelperFunctions.h
    std::vector<std::shared_ptr<Token>> right =
        tokenizeExpression(splitByEquals[1], arithmeticOpsWithWhitespace);
    output.push_back(left);
    output.push_back(TokenFactory::generateTokenForSimple("="sv));
    output.insert(output.end(), right.begin(), right.end());

    return output;
  }

  // Tokenizes statements that are only are supposed to have two words.
  // Examples: read x; print y; call z; expectedWord1 defines which type of
  // statement it should be. INPUT THE EXACT WORD WITHOUT WHITESPACES.
  static std::vector<std::shared_ptr<Token>> tokenizeTwoWordStmt(
      std::string stmt, std::string expectedWord1) {
    std::vector<std::shared_ptr<Token>> output;
    std::vector<std::string> split = splitString(stmt);
    if (split.size() != 2) {
      // Invalid two-word statement
      std::cerr << ExceptionMessages::invalidTwoWordStmt << std::endl;
      return std::vector<std::shared_ptr<Token>>();  // returns empty
    }
    output.push_back(TokenFactory::generateTokenForSimple(expectedWord1));
    std::string right = trimWhitespaces(split[1]);
    if (!isValidName(right)) {
      std::cerr << ExceptionMessages::invalidIdentifier << std::endl;
      return std::vector<std::shared_ptr<Token>>();  // returns empty
    }
    output.push_back(TokenFactory::generateTokenForSimple(right, true));
    return output;
  }

  // Validates the details that are found before the {} of a procedure.
  // Throws exceptions if the procedure declaration is invalid.
  // Returns the procedure name
  static std::string processPreProcedureDetails(std::string input) {
    std::string trimmed = trimWhitespaces(input);
    std::vector<std::string> beforeOpenSplit = splitString(trimmed);
    if (beforeOpenSplit.size() != 2) {
      std::cerr << ExceptionMessages::invalidProcedureDefinition << std::endl;
      return "";
    }
    std::string procedureKeyword = trimWhitespaces(beforeOpenSplit[0]);
    if (procedureKeyword != "procedure") {
      std::cerr << ExceptionMessages::invalidProcedureDefinition << std::endl;
      return "";
    }
    std::string procName = trimWhitespaces(beforeOpenSplit[1]);
    if (!isValidName(procName)) {
      std::cerr << ExceptionMessages::invalidProcedureDefinition << std::endl;
      return "";
    }
    return procName;
  }

 public:
  /// <summary>
  /// Tokenizes a SIMPLE source code into a TokenizedProgram.
  ///
  /// See TokenizerResults for the details of the class.
  /// </summary>
  /// <param name="srcCode">The source code in string view form</param>
  /// <returns>A shared pointer to the TokenizedProgram</returns>
  static std::shared_ptr<TokenizedProgram> tokenizeProgram(
      std::string_view srcCode) {
    SimpleTokenizer::statementNumber = 0;
    std::string trimmed = trimWhitespaces(std::string(srcCode));
    if (trimmed.empty()) {
      std::cerr << ExceptionMessages::emptyProgramGiven << std::endl;
      return nullptr;
    }
    std::vector<std::shared_ptr<TokenizedProcedure>> out;

    std::vector<std::shared_ptr<std::pair<int, int>>> curlyPairs =
        outermostScopeDetector(trimmed);
    int previousEnd = 0;
    for (std::shared_ptr<std::pair<int, int>> shared_ptr_p : curlyPairs) {
      std::pair<int, int> p = *shared_ptr_p;
      if (p.second == p.first + 1) {
        std::cerr << ExceptionMessages::emptyStatementListGiven << std::endl;
        return nullptr;
      }

      // Process Procedure Name using string indexes
      std::string beforeOpen = substring(
          trimmed, previousEnd,
          p.first -
              1);  // -1 because we do not want the open curly to be included
      std::string procName = processPreProcedureDetails(beforeOpen);
      if (procName == "") {
        return nullptr;
      }

      // Process Procedure Body using string indexes and function
      std::string body = substring(
          trimmed, p.first + 1,
          p.second -
              1);  // +1 -1 because we do not want the curly to be included
      std::shared_ptr<TokenizedStmtList> stmtList = tokenizeStmtList(body);
      out.push_back(std::make_shared<TokenizedProcedure>(procName, stmtList));

      previousEnd =
          p.second +
          1;  // + 1 to not include the close curly in the next iteration
    }
    // There are leftover chracters. Which should not be the case since the
    // program was trimmed of whitespaces already.
    if (previousEnd != trimmed.size()) {
      std::cerr << ExceptionMessages::invalidProgramDefinition << std::endl;
      return nullptr;
    }
    return std::make_shared<TokenizedProgram>(out);
  }
};
#endif
