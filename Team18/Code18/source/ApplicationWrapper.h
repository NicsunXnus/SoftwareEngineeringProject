#ifndef APPLICATIONWRAPPER_H
#define APPLICATIONWRAPPER_H

#include <fstream>
#include <string>
#include <iostream>
#include <list>

#include "TokenizerClasses/SIMPLETokenizer.h"
#include "AST/ASTBuilder.h"
#include "QPS/QueryDriver.h"

class ApplicationWrapper {
public:
  // a default constructor
  ApplicationWrapper();

  // destructor
  ~ApplicationWrapper();

  // directly parses source code without taking into account that it is a string
  void directParse(std::string srcCode);

  // method for parsing the SIMPLE source. Do
  void parse(std::string filename) {
    // call your parser to do the parsing
    std::ifstream simpleStream(filename);

    std::string src = "";
    if (simpleStream.is_open()) {
      while (simpleStream) {
        src += simpleStream.get();
      }
    }
    this->directParse(src);
  }

  // method to evaluating a query
  void evaluate(std::string query, std::list<std::string>& results);
};
#endif