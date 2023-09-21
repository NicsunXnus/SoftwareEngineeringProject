#ifndef PATTERNCLAUSEOBJECTFACTORY_H
#define PATTERNCLAUSEOBJECTFACTORY_H

#include <string_view>
#include "PatternClauseObject.h"
#include "QueryObjectFactory.h"


class PatternClauseObjectFactory : public QueryObjectFactory {
private:
	const unordered_set<ENTITY> validArg0SynonymEntities{ ASSIGN };
	const unordered_set<ENTITY> validArg1SynonymEntities{ VARIABLE };

public:
	PatternClauseObjectFactory() : QueryObjectFactory{} {}

	shared_ptr<QueryObject> create(string_view clauseName, vector<std::shared_ptr<ClauseArg>> arguments) override {
		std::shared_ptr<ClauseArg> patternSynonym{ arguments[0] };
		std::shared_ptr<ClauseArg> arg1{ arguments[1] };
		std::shared_ptr<ClauseArg> arg2{ arguments[2] };


		bool isArg0ValidSynonym{ patternSynonym->isSynonym() 
			&& (validArg0SynonymEntities.find(patternSynonym->getSynonym()->getEntityType()) != validArg0SynonymEntities.end()) };

		bool isArg1Wildcard{ arg1->isWildcard() };
		bool isArg1ValidSynonym{ arg1->isSynonym() 
			&& (validArg1SynonymEntities.find(arg1->getSynonym()->getEntityType()) != validArg1SynonymEntities.end()) };
		bool isArg1Ident{ arg1->isIdentifier() };
		bool isArg1ValidEntRef{ isArg1Wildcard || isArg1ValidSynonym || isArg1Ident };

		bool isArg2Wildcard{ arg2->isWildcard() };
		bool isArg2PartialMatching{ arg2->isPartialMatchingExprSpec()};
		bool isArg2ValidExprSpec{ isArg2Wildcard || isArg2PartialMatching };

		
		if (isArg0ValidSynonym && isArg1ValidEntRef && isArg2ValidExprSpec) { // all 3 args are valid for a pattern clause
			return make_shared<PatternObject>("pattern"sv, arguments);
		}

		if (patternSynonym->isSynonym() && !isArg0ValidSynonym) {
			throw SemanticErrorException("Semantic error: synonym for pattern clause is not valid");
		} else if (arg1->isSynonym() && !isArg1ValidSynonym) {
			throw SemanticErrorException("Semantic error: arg1 of pattern clause is not valid");
		} else {
			throw SyntaxErrorException("Syntax error: Error in validating pattern clause");
		}
	
	}
};
#endif