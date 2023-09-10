#include <fstream>

#include "TestWrapper.h"
#include "TokenizerClasses/SIMPLETokenizer.h"

// implementation code of WrapperFactory - do NOT modify the next 5 lines
AbstractWrapper* WrapperFactory::wrapper = 0;
AbstractWrapper* WrapperFactory::createWrapper() {
  if (wrapper == 0) wrapper = new TestWrapper;
  return wrapper;
}
// Do not modify the following line
volatile bool TestWrapper::GlobalStop = false;

// a default constructor
TestWrapper::TestWrapper() {
  // create any objects here as instance variables of this class
  // as well as any initialization required for your spa program
}

void TestWrapper::directParse(std::string srcCode) {
  // shift everything from line 35 onwards to here, and make parse() call this code.
}

// method for parsing the SIMPLE source
void TestWrapper::parse(std::string filename) {
  // call your parser to do the parsing
  std::ifstream simpleStream(filename);

  std::string src = "";
  if (simpleStream.is_open()) {
    while (simpleStream) {
      src += simpleStream.get();
    }
  }
  std::vector<std::vector<std::shared_ptr<Token>>> tokens = SimpleTokenizer::tokenize(src);
  // ...rest of your code...

}

// method to evaluating a query
void TestWrapper::evaluate(std::string query, std::list<std::string>& results) {
  // call your evaluator to evaluate the query here
    // ...code to evaluate query...

    // store the answers to the query in the results list (it is initially empty)
    // each result must be a string.
    shared_ptr<QueryDriver> driver = make_shared<QueryDriver>(query);
    list<string>queryResults = driver->execute();
    // chat-gpt code
    std::copy(queryResults.begin(), queryResults.end(), std::back_inserter(results));

}
