#include "TestWrapper.h"

#include <fstream>
#include "TestWrapper.h"

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
  this->appWrap = ApplicationWrapper();
}

// method for parsing the SIMPLE source
void TestWrapper::parse(string filename) {
  // cout << "parsing" << endl;
  this->appWrap.parse(filename);
  // cout << "parsed" << endl;
}

// method to evaluating a query
void TestWrapper::evaluate(string query, list<string>& results) {
  // cout << "start" << endl;
  this->appWrap.evaluate(query, results);
  // cout << "end" << endl << flush;
}
