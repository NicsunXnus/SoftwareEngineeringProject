#ifndef CALLTOKEN_H
#define CALLTOKEN_H

#include <string>
#include <string_view>
#include "KeywordToken.h"

using namespace std::string_view_literals;

/**
* This class represents the call token
*/
class CallToken : public KeywordToken {
public:
	CallToken()
		: KeywordToken{ "call"sv } {};
};

#endif
