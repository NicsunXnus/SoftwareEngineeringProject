#pragma once
#include "ApplicationWrapper.h"

// a default constructor
ApplicationWrapper::ApplicationWrapper() {
  // create any objects here as instance variables of this class
  // as well as any initialization required for your spa program
}

// destructor
ApplicationWrapper::~ApplicationWrapper() {

}

// method for parsing the SIMPLE source code
void ApplicationWrapper::directParse(std::string_view sourceCode) {
  std::shared_ptr<TokenizedProgram> tokenizedProgram = SimpleTokenizer::tokenizeProgram(sourceCode);
  //std::vector<std::shared_ptr<StatementNode>> statements = ASTBuilder::parseProgram(tokenizedProgram);
  //
  //DesignExtractor designExtractor = DesignExtractor();
  //for (const auto& statement : statements) {
  //  designExtractor.extractEntities(statement);
  //}
  //designExtractor.insertEntities();
  
}

// method to evaluating a query
void ApplicationWrapper::evaluate(std::string query, std::list<std::string>& results) {
  // call your evaluator to evaluate the query here
  // ...code to evaluate query...

  // store the answers to the query in the results list (it is initially empty)
  // each result must be a string.

  shared_ptr<QueryDriver> driver = make_shared<QueryDriver>(query);
  list<string>queryResults = driver->execute();
  // chat-gpt code
  std::copy(queryResults.begin(), queryResults.end(), std::back_inserter(results));
}