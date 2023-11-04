// ai-gen start (gpt3, 1)
// c++ create custom exception for syntax error
#ifndef SYNTAX_ERROR_EXCEPTION_H
#define SYNTAX_ERROR_EXCEPTION_H
#include <iostream>
#include "QPSError.h"

using namespace std;

class SyntaxErrorException : public QPSError {
public:
    SyntaxErrorException(const string& message) : QPSError{ message } {}

   
  
    const char* getType() const noexcept override {
        return type.c_str();
    }


private:
    string type = SYNTAX_ERROR;
};


// ai-gen end
#endif