#ifndef ABSTRACTWRAPPER_H
#define ABSTRACTWRAPPER_H

#include <iostream>
#include <list>
#include <string>

class AbstractWrapper {
 public:
  static volatile bool GlobalStop;
  // method for parsing the SIMPLE source
  virtual void parse(string filename) = 0;
  // method for evaluating a query
  virtual void evaluate(string query, list<string>& results) = 0;
};

class WrapperFactory {
 public:
  // static method for creating a wrapper
  static AbstractWrapper* createWrapper();

 private:
  // the following three will prevent the creation of WrapperFactory objects
  WrapperFactory();
  WrapperFactory(const WrapperFactory&);
  WrapperFactory& operator=(const WrapperFactory&);

  // the Wrapper object
  static AbstractWrapper* wrapper;
};

#endif
