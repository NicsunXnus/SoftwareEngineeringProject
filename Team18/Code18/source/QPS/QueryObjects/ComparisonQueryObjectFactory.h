#ifndef COMPARISONQUERYOBJECTFACTORY_H
#define COMPARISONQUERYOBJECTFACTORY_H

#include <string_view>
#include "ComparisonQueryObject.h"
#include "QueryObjectFactory.h"
#include "WithClauseObjectFactory.h"


class ComparisonQueryObjectFactory : public QueryObjectFactory {
private:

public:
	ComparisonQueryObjectFactory() : QueryObjectFactory{} {}

};

class StaticStaticComparisonFactory : public ComparisonQueryObjectFactory {
private:

public:
	StaticStaticComparisonFactory() : ComparisonQueryObjectFactory{} {}

	shared_ptr<QueryObject> create(string_view clauseName, vector<std::shared_ptr<ClauseArg>> arguments) override {
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

		return make_shared<StaticStaticComparisonQueryObject>(clauseName, arg1, arg2);
	};
};

class StaticAttrRefComparisonFactory : public ComparisonQueryObjectFactory {
private:

public:
	StaticAttrRefComparisonFactory() : ComparisonQueryObjectFactory{} {}

	// arg[0] is synonym name, arg[1] is attrName, arg[2] is static value
	shared_ptr<QueryObject> create(string_view clauseName, vector<std::shared_ptr<ClauseArg>> arguments) override {
		if (static_cast<int>(arguments.size() != 3)) {
			throw SyntaxErrorException("invalid argument count for attrRef static comparison object");
		}

		shared_ptr<ClauseArg> synonymArg{ arguments[0] };
		shared_ptr<ClauseArg> attrName{ arguments[1] };
		shared_ptr<ClauseArg> staticValue{ arguments[2] };
		string_view attrNameSV{ attrName->getArg() };
		string_view staticValueSV{ staticValue->getArg()};

		bool isSynonymArgSynonym{ synonymArg->isSynonym() };
		bool isValidStaticVal{ staticValue->isIdentifier() || staticValue->isInteger() };
		bool areNames{ (attrNameSV == "procName"sv || attrNameSV == "varName"sv) && staticValue->isIdentifier() };
		bool areIntegers{ (attrNameSV == "value"sv || attrNameSV == "stmt#"sv) && staticValue->isInteger() };
		
		if (!isSynonymArgSynonym || !isValidStaticVal) {
			throw SyntaxErrorException("Invalid arguments in attrRef static comparison object");
		}

		// create attr ref object
		shared_ptr<QueryObjectFactory> attrRefFactory{ QueryObjectFactory::createFactory(attrName->getArg()) };
		std::vector<shared_ptr<ClauseArg>> synonymVec{ synonymArg };
		shared_ptr<QueryObject> attrRef{ attrRefFactory->create(attrName->getArg(), synonymVec) };

		// check both args are same type
		if (!areNames && !areIntegers) {
			throw SemanticErrorException("both arguments in Static=AttrRef need to be the same type");
		}

		return make_shared<StaticAttrRefComparisonQueryObject>(clauseName, attrRef, staticValue);
	};
};

class AttrRefAttrRefComparisonFactory : public ComparisonQueryObjectFactory {
private:

public:
	AttrRefAttrRefComparisonFactory() : ComparisonQueryObjectFactory{} {}

	shared_ptr<QueryObject> create(string_view clauseName, vector<std::shared_ptr<ClauseArg>> arguments) override {
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

		// create attr ref 1 object
		shared_ptr<QueryObjectFactory> attrRefFactory1{ QueryObjectFactory::createFactory(attrName1->getArg()) };
		std::vector<shared_ptr<ClauseArg>> synonymVec1{ synonymArg1 };
		shared_ptr<QueryObject> attrRef1{ attrRefFactory1->create(attrName1->getArg(), synonymVec1) };

		// create attr ref 2 object
		shared_ptr<QueryObjectFactory> attrRefFactory2{ QueryObjectFactory::createFactory(attrName2->getArg()) };
		std::vector<shared_ptr<ClauseArg>> synonymVec2{ synonymArg2 };
		shared_ptr<QueryObject> attrRef2{ attrRefFactory2->create(attrName2->getArg(), synonymVec2) };

		// check both args are same type
		if (!areNames && !areIntegers) {
			throw SemanticErrorException("both arguments in Static=AttrRef need to be the same type");
		}

		return make_shared<AttrRefAttrRefComparisonQueryObject>(clauseName, attrRef1, attrRef2);
	}
};

#endif
