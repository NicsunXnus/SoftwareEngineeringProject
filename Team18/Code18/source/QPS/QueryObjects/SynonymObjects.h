#pragma once
#ifndef SYNONYMOBJECT_H
#define SYNONYMOBJECT_H

#include <string_view>
#include <ctype.h>

using namespace std;

/*
* This class represents a Query object, for synonyms: LETTER ( LETTER | DIGIT )*
*/
class SynonymObject {
public:
	SynonymObject(string_view tokenName) {

	}

	/*
	* This function checks if a given string is a valid synonym: LETTER ( LETTER | DIGIT )*
	*/
	static bool isValid(string_view str) {
		int len = str.size();
		if (len == 0) {
			return false;
		}
		if (!isalpha(str[0])) {
			return false;
		}
		for (int i = 1; i < len; i++) {
			if (!(isalpha(str[i]) || isdigit(str[i]))) {
				return false;
			}
		}

		return true;
	}
};

#endif