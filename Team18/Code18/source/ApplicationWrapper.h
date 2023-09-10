#ifndef APPLICATIONWRAPPER_H
#define APPLICATIONWRAPPER_H

#include <fstream>
#include <string>
#include <iostream>
#include <list>

#include "TokenizerClasses/SIMPLETokenizer.h"
#include "AST/ASTBuilder.h"
#include "QPS/QueryDriver.h"
#include "DesignExtractor/DesignExtractor.h"

class ApplicationWrapper {
public:
  // a default constructor
  ApplicationWrapper();

  // destructor
  ~ApplicationWrapper();

  // directly parses source code assuming the input is the actual code
  void directParse(std::string srcCode);

  // method for parsing the SIMPLE source, assuming the input is the filename
  void parse(std::string filename) {
    // call your parser to do the parsing
    std::ifstream simpleStream(filename);

    std::string src = "";
    if (simpleStream.is_open()) {
      while (simpleStream) {
        char c = simpleStream.get();
        if (c == -1) { // special end of line character
          break;
        }
        src += c;
      }
    }
    this->directParse(src);
  }

  // method to evaluating a query
  void evaluate(std::string query, std::list<std::string>& results);
};
#endif