#include "PatternClauseObjectFactory.h"
#include "../../SP/SimpleProcessor/ExpressionProcessor.h"

bool PatternClauseObjectFactory::validateAssignPatternArgs(vector<shared_ptr<ClauseArg>>& arguments) {
	std::shared_ptr<ClauseArg> patternSynonym{ arguments[0] };
	std::shared_ptr<ClauseArg> arg1{ arguments[1] };
	std::shared_ptr<ClauseArg> arg2{ arguments[2] };

	if (patternSynonym->isSynonym() && patternSynonym->getSynonym()->getEntityType() == WHILE) {
		return false;
	}

	bool isArg0ValidSynonymAssign{ patternSynonym->isSynonym() && patternSynonym->getSynonym()->getEntityType() == ASSIGN };
	bool isArg0If{ patternSynonym->isSynonym() && patternSynonym->getSynonym()->getEntityType() == IF };

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
			shared_ptr<Node> patternTree = ep.nodifyArithmeticExpression(svToString(arg2->getArgValue()));
			isSyntacticallyCorrectExpr = true;
		}
		catch (invalid_argument e) {
			throw SyntaxErrorException("Syntax error: invalid syntax in assign pattern expression-spec");
		}
	}
	bool isArg2ValidExprSpec{ isArg2Wildcard || isSyntacticallyCorrectExpr };



	if (isArg0ValidSynonymAssign && isArg1ValidEntRef && isArg2ValidExprSpec) { // all 3 args are valid for a pattern clause
		return true;
	}

	if (!patternSynonym->isSynonym() || !(arg1->isSynonym() || isArg1Ident || isArg1Wildcard) || !isArg2ValidExprSpec) {
		// check if any of the arguments are invalid generically
		throw SyntaxErrorException("Invalid arguments in assign pattern clause");
	}
	else if (isArg0If) {
		throw SyntaxErrorException("Expected If pattern syntax. Got while/assign syntax instead")
	}
	else if (patternSynonym->isSynonym() && !isArg0ValidSynonymAssign) {
		throw SemanticErrorException("Semantic error: synonym for assign pattern clause is not valid");
	}
	else if (arg1->isSynonym() && !isArg1ValidSynonym) {
		throw SemanticErrorException("Semantic error: arg1 of pattern clause is not valid");
	}
	else {
		throw SyntaxErrorException("Syntax error: Error in validating pattern clause");
	}
}

bool PatternClauseObjectFactory::validateWhilePatternArgs(vector<shared_ptr<ClauseArg>>& arguments) {
	std::shared_ptr<ClauseArg> patternSynonym{ arguments[0] };
	std::shared_ptr<ClauseArg> arg1{ arguments[1] };
	std::shared_ptr<ClauseArg> arg2{ arguments[2] };

	if (patternSynonym->isSynonym() && patternSynonym->getSynonym()->getEntityType() == ASSIGN) {
		return false;
	}

	bool isArg0ValidSynonymWhile{ patternSynonym->isSynonym() && patternSynonym->getSynonym()->getEntityType() == WHILE };
	bool isArg0If{ patternSynonym->isSynonym() && patternSynonym->getSynonym()->getEntityType() == IF };

	bool isArg1Wildcard{ arg1->isWildcard() };
	bool isArg1ValidSynonym{ arg1->isSynonym()
		&& (validArg1SynonymEntities.find(arg1->getSynonym()->getEntityType()) != validArg1SynonymEntities.end()) };
	bool isArg1Ident{ arg1->isIdentifier() };
	bool isArg1ValidEntRef{ isArg1Wildcard || isArg1ValidSynonym || isArg1Ident };

	bool isArg2Wildcard{ arg2->isWildcard() };

	if (isArg0ValidSynonymWhile && isArg1ValidEntRef && isArg2Wildcard) {
		return true;
	}

	if (!patternSynonym->isSynonym() || !(arg1->isSynonym() || isArg1Ident || isArg1Wildcard) || !isArg2Wildcard) {
		// check if any of the arguments are invalid generically
		throw SyntaxErrorException("Invalid arguments in while pattern clause");
	}
	else if (isArg0If) {
		throw SyntaxErrorException("Expected If pattern syntax. Got while/assign syntax instead")
	}
	else if (patternSynonym->isSynonym() && !isArg0ValidSynonymWhile) {
		throw SemanticErrorException("Semantic error: synonym for while pattern clause is not valid");
	}
	else if (arg1->isSynonym() && !isArg1ValidSynonym) {
		throw SemanticErrorException("Semantic error: arg1 of pattern clause is not valid");
	}
	else {
		throw SyntaxErrorException("Syntax error: Error in validating pattern clause");
	}
}

bool PatternClauseObjectFactory::validateIfPatternArgs(vector<shared_ptr<ClauseArg>>& arguments) {
	if (static_cast<int>(arguments.size()) < 4) {
		throw SyntaxErrorException("Insufficient argument count in if pattern clause");
	}

	std::shared_ptr<ClauseArg> ifSynonym{ arguments[0] };
	std::shared_ptr<ClauseArg> arg1{ arguments[1] };
	std::shared_ptr<ClauseArg> arg2{ arguments[2] };
	std::shared_ptr<ClauseArg> arg3{ arguments[3] };

	bool isArg0ValidSynonym{ ifSynonym->isSynonym() && ifSynonym->getSynonym()->getEntityType() == IF };
	bool isArg0WhileAssign{ ifSynonym->isSynonym() &&
		ifSynonym->getSynonym()->getEntityType() == ASSIGN || ifSynonym->getSynonym()->getEntityType() == WHILE };

	bool isArg1Wildcard{ arg1->isWildcard() };
	bool isArg1ValidSynonym{ arg1->isSynonym()
		&& (validArg1SynonymEntities.find(arg1->getSynonym()->getEntityType()) != validArg1SynonymEntities.end()) };
	bool isArg1Ident{ arg1->isIdentifier() };
	bool isArg1ValidEntRef{ isArg1Wildcard || isArg1ValidSynonym || isArg1Ident };

	bool isArg2Wildcard{ arg2->isWildcard() };
	bool isArg3Wildcard{ arg3->isWildcard() };

	if (isArg0ValidSynonym && isArg1ValidEntRef && isArg2Wildcard && isArg3Wildcard) {
		return true;
	}

	if (!ifSynonym->isSynonym() || !(arg1->isSynonym() || isArg1Ident || isArg1Wildcard) || !isArg2Wildcard || !isArg3Wildcard) {
		throw SyntaxErrorException("Invalid arguments in if pattern clause");
	}
	else if (isArg0WhileAssign) {
		// pattern syntax should be 3 args, but we got 4. Invalid syntax for while/assign pattern
		throw SyntaxErrorException("Expected syntax for while/assign pattern. Got if syntax instead");
	}
	else if (ifSynonym->isSynonym() && !isArg0ValidSynonym) {
		throw SemanticErrorException("Semantic error: synonym for if pattern clause is not valid");
	}
	else if (arg1->isSynonym() && !isArg1ValidSynonym) {
		throw SemanticErrorException("Semantic error: arg1 for if pattern clause is not valid");
	}
	else {
		throw SyntaxErrorException("Syntax error: Error in validating if pattern clause");
	}
}


shared_ptr<QueryObject> PatternClauseObjectFactory::create(string_view clauseName, vector<std::shared_ptr<ClauseArg>> arguments) {
	if (static_cast<int>(arguments.size()) == IF_PATTERN_ARGUMENT_COUNT) {
		return createIf(clauseName, arguments);
	}

	if (validateAssignPatternArgs(arguments)) {
		return make_shared<AssignPatternObject>("pattern"sv, arguments);
	}
	else if (validateWhilePatternArgs(arguments)) {
		return make_shared<WhilePatternObject>("patternWhile"sv, arguments);
	}
	else {
		throw SyntaxErrorException("Error in while clause validation");
	}
}

shared_ptr<QueryObject> PatternClauseObjectFactory::createIf(string_view clauseName, vector<std::shared_ptr<ClauseArg>> arguments) {
	if (validateIfPatternArgs(arguments)) {
		return make_shared<IfPatternObject>("patternIf"sv, arguments);
	}
	else {
		// shouldnt reach this point as we'll throw an error in validate
		throw SyntaxErrorException("Error in if pattern validation");
	}
}
