#ifndef TESTWRAPPER_H
#define TESTWRAPPER_H

#include <iostream>
#include <list>
#include <string>

// include your other headers here
#include "../../source/ApplicationWrapper.h"
#include "AbstractWrapper.h"

class TestWrapper : public AbstractWrapper {
 private:
  ApplicationWrapper appWrap;

 public:
  // default constructor
  TestWrapper();

  // destructor
  ~TestWrapper();

  // method for parsing the SIMPLE source
  virtual void parse(string filename);

  // method for evaluating a query
  virtual void evaluate(string query, list<string>& results);
};

#endif
