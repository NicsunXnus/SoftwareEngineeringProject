#ifndef APPLICATIONWRAPPER_H
#define APPLICATIONWRAPPER_H

#include <fstream>
#include <string>
#include <iostream>
#include <list>

#include "TokenizerClasses/SIMPLETokenizer.h"

class ApplicationWrapper {
public:
  // default constructor
  ApplicationWrapper();

  // destructor
  ~ApplicationWrapper();

  // a default constructor
  ApplicationWrapper() {
    // create any objects here as instance variables of this class
    // as well as any initialization required for your spa program
  }

  void directParse(std::string srcCode) {
    // shift everything from line 35 onwards to here, and make parse() call this code.
  }

  // method for parsing the SIMPLE source
  void parse(std::string filename) {
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
  void evaluate(std::string query, std::list<std::string>& results) {
    // call your evaluator to evaluate the query here
      // ...code to evaluate query...

      // store the answers to the query in the results list (it is initially empty)
      // each result must be a string.
  }
}
#endif