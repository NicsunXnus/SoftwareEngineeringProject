#ifndef READTOKEN_H
#define READTOKEN_H

#include <string>
#include <string_view>
#include "KeywordToken.h"

using namespace std::string_view_literals;

/**
* This class represents the read token
*/
class ReadToken : public KeywordToken {
public:
	ReadToken()
		: KeywordToken{ "read"sv } {};
};

#endif
