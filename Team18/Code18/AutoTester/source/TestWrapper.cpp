#include <fstream>

#include "TestWrapper.h"
#include "../../source/SimpleTokenizer/SIMPLETokenizer.h"

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
void TestWrapper::parse(std::string filename) {
  //std::cout << "parsing" << std::endl;
  this->appWrap.parse(filename);
  //std::cout << "parsed" << std::endl;
}

// method to evaluating a query
void TestWrapper::evaluate(std::string query, std::list<std::string>& results) {
  //std::cout << "start" << std::endl;
  this->appWrap.evaluate(query, results);
  //std::cout << "end" << std::endl << std::flush;
}
/*
int main(int argc, char* argv[]) {
	TestWrapper tw = TestWrapper();
	//tw.parse(argv[1]);

	//tw.evaluate(std::string query, std::list<std::string>&results);
	return 0;
}*/
