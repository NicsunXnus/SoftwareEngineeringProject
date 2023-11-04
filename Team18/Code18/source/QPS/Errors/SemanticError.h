#ifndef SEMANTIC_ERROR_EXCEPTION_H
#define SEMANTIC_ERROR_EXCEPTION_H
#include <iostream>
#include "QPSError.h"

using namespace std;

class SemanticErrorException : public QPSError {
public:
    SemanticErrorException(const string& message) : QPSError{ message } {}


    const char* getType() const noexcept override {
        return type.c_str();
    }


private:
    string type = SEMANTIC_ERROR;
};


#endif