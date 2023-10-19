#include "PatternClauseObjectFactory.h"
#include "../../SP/SimpleProcessor/ExpressionProcessor.h"

shared_ptr<QueryObject> PatternClauseObjectFactory::create(string_view clauseName, vector<std::shared_ptr<ClauseArg>> arguments) {
	if (static_cast<int>(arguments.size()) == IF_PATTERN_ARGUMENT_COUNT) {
		return createIf(clauseName, arguments);
	}

	std::shared_ptr<ClauseArg> patternSynonym{ arguments[0] };
	std::shared_ptr<ClauseArg> arg1{ arguments[1] };
	std::shared_ptr<ClauseArg> arg2{ arguments[2] };


	bool isArg0ValidSynonymAssign{ patternSynonym->isSynonym() && patternSynonym->getSynonym()->getEntityType() == ASSIGN };

	bool isArg0ValidSynonymWhile{ patternSynonym->isSynonym() && patternSynonym->getSynonym()->getEntityType() == WHILE };

	bool isArg1Wildcard{ arg1->isWildcard() };
	bool isArg1ValidSynonym{ arg1->isSynonym()
		&& (validArg1SynonymEntities.find(arg1->getSynonym()->getEntityType()) != validArg1SynonymEntities.end()) };
	bool isArg1Ident{ arg1->isIdentifier() };
	bool isArg1ValidEntRef{ isArg1Wildcard || isArg1ValidSynonym || isArg1Ident };

	bool isArg2Wildcard{ arg2->isWildcard() };
	bool isArg2PartialMatching{ arg2->isPartialMatchingExprSpec() };
	bool isArg2ExactExpr{ arg2->isExpr() };
	bool isSyntacticallyCorrectExpr{ false };
	if (isArg2PartialMatching || isArg2ExactExpr) {
		try {
			ExpressionProcessor ep = ExpressionProcessor();
			std::shared_ptr<Node> patternTree = ep.nodifyArithmeticExpression(svToString(arg2->getIdentifier()));
			isSyntacticallyCorrectExpr = true;
		}
		catch (invalid_argument e) {
			throw SyntaxErrorException("Syntax error: invalid syntax in assign pattern expression-spec");
		}
	}
	bool isArg2ValidExprSpec{ isArg2Wildcard || isSyntacticallyCorrectExpr };


	if (isArg0ValidSynonymWhile && isArg1ValidEntRef && isArg2Wildcard) {
		return make_shared<WhilePatternObject>("patternWhile"sv, arguments);
	}
	else if (isArg0ValidSynonymAssign && isArg1ValidEntRef && isArg2ValidExprSpec) { // all 3 args are valid for a pattern clause
		return make_shared<AssignPatternObject>("pattern"sv, arguments);
	}

	if (patternSynonym->isSynonym() && !isArg0ValidSynonymAssign && !isArg0ValidSynonymWhile) {
		throw SemanticErrorException("Semantic error: synonym for pattern clause is not valid");
	}
	else if (arg1->isSynonym() && !isArg1ValidSynonym) {
		throw SemanticErrorException("Semantic error: arg1 of pattern clause is not valid");
	}
	else {
		throw SyntaxErrorException("Syntax error: Error in validating pattern clause");
	}
}

shared_ptr<QueryObject> PatternClauseObjectFactory::createIf(string_view clauseName, vector<std::shared_ptr<ClauseArg>> arguments) {
	std::shared_ptr<ClauseArg> ifSynonym{ arguments[0] };
	std::shared_ptr<ClauseArg> arg1{ arguments[1] };
	std::shared_ptr<ClauseArg> arg2{ arguments[2] };
	std::shared_ptr<ClauseArg> arg3{ arguments[3] };

	bool isArg0ValidSynonym{ ifSynonym->isSynonym() && ifSynonym->getSynonym()->getEntityType() == IF };

	bool isArg1Wildcard{ arg1->isWildcard() };
	bool isArg1ValidSynonym{ arg1->isSynonym()
		&& (validArg1SynonymEntities.find(arg1->getSynonym()->getEntityType()) != validArg1SynonymEntities.end()) };
	bool isArg1Ident{ arg1->isIdentifier() };
	bool isArg1ValidEntRef{ isArg1Wildcard || isArg1ValidSynonym || isArg1Ident };

	bool isArg2Wildcard{ arg2->isWildcard() };
	bool isArg3Wildcard{ arg3->isWildcard() };

	if (isArg0ValidSynonym && isArg1ValidEntRef && isArg2Wildcard && isArg3Wildcard) {
		return make_shared<IfPatternObject>("patternIf"sv, arguments);
	}

	if (ifSynonym->isSynonym() && !isArg0ValidSynonym) {
		throw SemanticErrorException("Semantic error: synonym for if pattern clause not valid");
	}
	else if (arg1->isSynonym() && !isArg1ValidSynonym) {
		throw SemanticErrorException("Semantic error: arg1 for if pattern clause is not valid");
	}
	else {
		throw SyntaxErrorException("Syntax error: Error in validating if pattern clause");
	}
}