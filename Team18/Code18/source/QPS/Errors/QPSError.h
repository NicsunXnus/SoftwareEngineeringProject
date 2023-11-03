#ifndef QPS_ERROR_H
#define QPS_ERROR_H
#include <iostream>
#include <exception>
#include "../../Constants/QPSConstants.h"

using namespace std;

class QPSError : public exception {
private:
    string errorMessage;
public:
    QPSError(const string& message) : errorMessage(message) {}
    const char* what() const noexcept override {
        return (errorMessage).c_str();
    }
    virtual const char* getType() const noexcept {
        return QPS_ERROR.c_str();
    }


};
#endif