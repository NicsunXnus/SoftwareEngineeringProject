#include "Token.h"

// Getter for the token name since the field is private.
string Token::getName() {
	return this->tokenName;
}
// Overriden
bool Token::operator==(Token& other) {
	shared_ptr<Token> otherPtr = make_shared<Token>(other);
	return checkEquality(make_shared<Token>(*this), otherPtr);
}

// Checks if the two tokens supplied are the same runtime type and same stored tokenName.
bool Token::checkEquality(shared_ptr<Token> left, shared_ptr<Token> right) {
	if (left == nullptr || right == nullptr) {
		return false;
	}
	string leftId = typeid(*left).name();
	string rightId = typeid(*right).name();
	bool sameType = leftId == rightId;
	if (!sameType) {
		return false;
	}
	string leftName = left->tokenName;
	string rightName = right->tokenName;
	return leftName == rightName;
}
