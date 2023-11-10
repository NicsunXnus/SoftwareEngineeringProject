#include "ComparisonQueryObjectFactory.h"

shared_ptr<QueryObject> makeAttrRef(vector<shared_ptr<ClauseArg>> arguments) {
	shared_ptr<ClauseArg> synonymArg{ arguments[0] };
	shared_ptr<ClauseArg> attrName{ arguments[1] };

	shared_ptr<QueryObjectFactory> attrRefFactory{ QueryObjectFactory::createFactory(attrName->getArg()) };
	vector<shared_ptr<ClauseArg>> synonymVec{ synonymArg };
	return attrRefFactory->create(attrName->getArg(), synonymVec);
}

bool StaticStaticComparisonFactory::validateArgs(vector<shared_ptr<ClauseArg>>& arguments) {
	if (static_cast<int>(arguments.size() != 2)) {
		throw SyntaxErrorException("invalid argument count for static static comparison object");
	}

	shared_ptr<ClauseArg> arg1{ arguments[0] };
	shared_ptr<ClauseArg> arg2{ arguments[1] };

	bool arg1IsValid{ arg1->isIdentifier() || arg1->isInteger() };
	bool arg2IsValid{ arg2->isIdentifier() || arg2->isInteger() };
	bool argsAreIdentifiers{ arg1->isIdentifier() && arg2->isIdentifier() };
	bool argsAreIntegers{ arg1->isInteger() && arg2->isInteger() };


	if (!arg1IsValid || !arg2IsValid) {
		throw SyntaxErrorException("invalid static static comparison arguments");
	}
	else if (!argsAreIdentifiers && !argsAreIntegers) {
		throw SemanticErrorException("both arguments in Static=Static need to be the same type");
	}

	return true;
}

shared_ptr<QueryObject> StaticStaticComparisonFactory::create(string_view clauseName, vector<shared_ptr<ClauseArg>> arguments) {
	if (validateArgs(arguments)) {
		return make_shared<StaticStaticComparisonQueryObject>(clauseName, arguments[0], arguments[1]);
	}
	else {
		throw SyntaxErrorException("Error in Static=Static clause argument validation");
	}
}

shared_ptr<QueryObject> StaticAttrRefComparisonFactory::createAttrRef(vector<shared_ptr<ClauseArg>> arguments) {
	return makeAttrRef(arguments);
}

bool StaticAttrRefComparisonFactory::validateArgs(vector<shared_ptr<ClauseArg>>& arguments) {
	if (static_cast<int>(arguments.size() != 3)) {
		throw SyntaxErrorException("invalid argument count for attrRef static comparison object");
	}

	shared_ptr<ClauseArg> synonymArg{ arguments[0] };
	shared_ptr<ClauseArg> attrName{ arguments[1] };
	shared_ptr<ClauseArg> staticValue{ arguments[2] };
	string_view attrNameSV{ attrName->getArg() };
	string_view staticValueSV{ staticValue->getArg() };

	bool isSynonymArgSynonym{ synonymArg->isSynonym() };
	bool isValidStaticVal{ staticValue->isIdentifier() || staticValue->isInteger() };
	bool areNames{ (attrNameSV == "procName"sv || attrNameSV == "varName"sv) && staticValue->isIdentifier() };
	bool areIntegers{ (attrNameSV == "value"sv || attrNameSV == "stmt#"sv) && staticValue->isInteger() };

	if (!isSynonymArgSynonym || !isValidStaticVal) {
		throw SyntaxErrorException("Invalid arguments in attrRef static comparison object");
	}
	if (!areNames && !areIntegers) { // check both arguments are the same type
		throw SemanticErrorException("both arguments in Static=AttrRef need to be the same type");
	}

	return true;
}

shared_ptr<QueryObject> StaticAttrRefComparisonFactory::create(string_view clauseName, vector<shared_ptr<ClauseArg>> arguments) {
	shared_ptr<QueryObject> attrRef;
	try {
		attrRef = createAttrRef({arguments[0], arguments[1]});
	}
	catch (SemanticErrorException& ex) {
		// continue validating args in case of syntax error
		validateArgs(arguments);
		throw ex;
	}
	if (validateArgs(arguments)) {
		return make_shared<StaticAttrRefComparisonQueryObject>(clauseName, attrRef, arguments[2]);
	}
}

shared_ptr<QueryObject> AttrRefAttrRefComparisonFactory::createAttrRef(vector<shared_ptr<ClauseArg>> arguments) {
	return makeAttrRef(arguments);
}

bool AttrRefAttrRefComparisonFactory::validateArgs(vector<shared_ptr<ClauseArg>>& arguments) {
	if (static_cast<int>(arguments.size() != 4)) {
		throw SyntaxErrorException("invalid argument count for attrRef attrRef comparison object");
	}

	shared_ptr<ClauseArg> synonymArg1{ arguments[0] };
	shared_ptr<ClauseArg> attrName1{ arguments[1] };
	shared_ptr<ClauseArg> synonymArg2{ arguments[2] };
	shared_ptr<ClauseArg> attrName2{ arguments[3] };
	string_view attrName1SV{ attrName1->getArg() };
	string_view attrName2SV{ attrName2->getArg() };

	bool isSynonymArg1Synonym{ synonymArg1->isSynonym() };
	bool isSynonymArg2Synonym{ synonymArg2->isSynonym() };
	bool areNames{ (attrName1SV == "procName"sv || attrName1SV == "varName"sv)
		&& (attrName2SV == "procName"sv || attrName2SV == "varName"sv) };
	bool areIntegers{ (attrName1SV == "value"sv || attrName1SV == "stmt#"sv) &&
		(attrName2SV == "value"sv || attrName2SV == "stmt#"sv) };

	if (!isSynonymArg1Synonym || !isSynonymArg2Synonym) {
		throw SyntaxErrorException("Invalid synonym args in attrRef attrRef comparison object");
	}
	// check both args are same type
	if (!areNames && !areIntegers) {
		throw SemanticErrorException("both arguments in AttrRef=AttrRef need to be the same type");
	}

	return true;
}

shared_ptr<QueryObject> AttrRefAttrRefComparisonFactory::create(string_view clauseName, vector<shared_ptr<ClauseArg>> arguments) {
	shared_ptr<QueryObject> attrRef1;
	shared_ptr<QueryObject> attrRef2;

	try {
		attrRef1 = createAttrRef({ arguments[0], arguments[1] });
		attrRef2 = createAttrRef({ arguments[2], arguments[3] });
	}
	catch (SemanticErrorException& ex) {
		validateArgs(arguments);
		throw ex;
	}
	if (validateArgs(arguments)) {
		return make_shared<AttrRefAttrRefComparisonQueryObject>(clauseName, attrRef1, attrRef2);
	}
}