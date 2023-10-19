#pragma once
#ifndef APPLICATIONWRAPPER_H
#define APPLICATIONWRAPPER_H

#include <fstream>
#include <iostream>
#include <list>
#include <string>

#include "SP/SimpleProcessor/SimpleProcessor.h"
#include "QPS/QueryDriver.h"

using namespace std::string_view_literals;

class ApplicationWrapper {
 public:
  // a default constructor
  ApplicationWrapper();

  // destructor
  ~ApplicationWrapper();

  // directly parses source code assuming the input is the actual code
  void directParse(std::string_view srcCode);

  // method for parsing the SIMPLE source, assuming the input is the filename
  void parse(std::string filename) {
    // call your parser to do the parsing
    std::ifstream simpleStream(filename);

    std::string src = "";
    if (simpleStream.is_open()) {
      while (simpleStream) {
        char c = simpleStream.get();
        if (c == -1) {  // special end of line character
          break;
        }
        src += c;
      }
    }
    this->directParse(std::string_view(src));
  }

  // method to evaluating a query
  void evaluate(std::string query, std::list<std::string>& results);
};
#endif