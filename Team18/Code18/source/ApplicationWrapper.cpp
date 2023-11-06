#pragma once
#include "ApplicationWrapper.h"
#include "SP/DesignExtractor/DesignExtractor.h"

// a default constructor
ApplicationWrapper::ApplicationWrapper() {
  // create any objects here as instance variables of this class
  // as well as any initialization required for your spa program
}

// destructor
ApplicationWrapper::~ApplicationWrapper() {

}

// method for parsing the SIMPLE source code
void ApplicationWrapper::directParse(string_view sourceCode, bool useMultithread) {
  shared_ptr<ProcessedProgram> processedProgram = SimpleProcessor::processProgram(sourceCode);
  DesignExtractor designExtractor = DesignExtractor();
  designExtractor.extractAndInsertAll(processedProgram, useMultithread);
}

// method to evaluating a query
void ApplicationWrapper::evaluate(string query, list<string>& results) {
  // call your evaluator to evaluate the query here
  // ...code to evaluate query...

    // store the answers to the query in the results list (it is initially empty)
    // each result must be a string.
    cout << "In AppWrapper, starting query evaluation\n";
    shared_ptr<QueryDriver> driver = make_shared<QueryDriver>(query);
    //cout << "AppWrap::Eval" << endl;
    list<string>queryResults = driver->execute();
    //cout << queryResults.size() << endl;
    // chat-gpt code
    copy(queryResults.begin(), queryResults.end(), back_inserter(results));
}