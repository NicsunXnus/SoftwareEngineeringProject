#pragma once
#ifndef APPLICATIONWRAPPER_H
#define APPLICATIONWRAPPER_H

#include <fstream>
#include <iostream>
#include <list>
#include <string>

#include "SP/SimpleProcessor/SimpleProcessor.h"
#include "QPS/QueryDriver.h"

using namespace std;

class ApplicationWrapper {
 public:
  // a default constructor
  ApplicationWrapper();

  // destructor
  ~ApplicationWrapper();

  // directly parses source code assuming the input is the actual code
  void directParse(string_view srcCode, bool useMultithread = false);

  // method for parsing the SIMPLE source, assuming the input is the filename
  void parse(string filename, bool useMultithread = false) {
    // call your parser to do the parsing
    ifstream simpleStream(filename);

    string src = "";
    if (simpleStream.is_open()) {
      while (simpleStream) {
        char c = simpleStream.get();
        if (c == -1) {  // special end of line character
          break;
        }
        src += c;
      }
    }
    this->directParse(string_view(src), useMultithread);
  }

  // method to evaluating a query
  void evaluate(string query, list<string>& results);
};
#endif