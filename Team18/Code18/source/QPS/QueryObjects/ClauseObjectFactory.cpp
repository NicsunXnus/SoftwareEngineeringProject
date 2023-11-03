#include "ClauseObjectFactory.h"

bool validateStmtStmtArgs(shared_ptr<ClauseArg> arg1, shared_ptr<ClauseArg> arg2, const unordered_set<ENTITY>& validStmtEntities) {
	bool isArg1SynonymStmtRef{ arg1->isSynonym() && (validStmtEntities.find(arg1->getSynonym()->getEntityType()) != validStmtEntities.end()) };
	bool isArg2SynonymStmtRef{ arg2->isSynonym() && (validStmtEntities.find(arg2->getSynonym()->getEntityType()) != validStmtEntities.end()) };

	bool isArg1ValidStmtRef{ isArg1SynonymStmtRef || arg1->isInteger() || arg1->isWildcard() };
	bool isArg2ValidStmtRef{ isArg2SynonymStmtRef || arg2->isInteger() || arg2->isWildcard() };

	if (isArg1ValidStmtRef && isArg2ValidStmtRef) {
		return true;
	}
	else {
		if (arg1->isSynonym() && !isArg1ValidStmtRef) {
			throw SemanticErrorException("Arg 1 of stmt stmt clause is an invalid synonym");
		}
		else if (arg2->isSynonym() && !isArg2ValidStmtRef) {
			throw SemanticErrorException("Arg 2 of stmt stmt clause is an invalid synonym");
		}
		else {
			throw SyntaxErrorException("Stmt stmt clause has invalid stmtRef");
		}
	}
}

bool validateUsesModifiesArgs(shared_ptr<ClauseArg> arg1, shared_ptr<ClauseArg> arg2, const unordered_set<ENTITY>& validEnts1,
	const unordered_set<ENTITY>& validEnts2, bool isP) {
	bool isArg1SynonymValidRef{ arg1->isSynonym() && (validEnts1.find(arg1->getSynonym()->getEntityType()) != validEnts1.end()) };
	bool isArg2SynonymValidRef{ arg2->isSynonym() && (validEnts2.find(arg2->getSynonym()->getEntityType()) != validEnts2.end()) };

	bool isArg1ValidRef{};
	if (!isP) { // UsesS/ModifiesS entity checking
		isArg1ValidRef = isArg1SynonymValidRef || arg1->isInteger();
	}
	else { // UsesP/ModifiesP entity checking
		isArg1ValidRef = isArg1SynonymValidRef || arg1->isIdentifier();
	}
	bool isArg2ValidEntRef{ isArg2SynonymValidRef || arg2->isWildcard() || arg2->isIdentifier() };

	if (isArg1ValidRef && isArg2ValidEntRef) { // Uses has an entity ref as the first argument
		return true;
	}

	// check if first arg is wildcard or if arg1 is a declared entity that is not semantically accepted by Uses
	if (arg1->isWildcard()) {
		throw SemanticErrorException("Semantic error: Uses/Modifies cannot have a wildcard as the 1st arg");
	}
	else if ((arg1->isSynonym() && !isArg1SynonymValidRef) || arg1->isInteger() || arg1->isIdentifier()) {
		return false; // could be the other version of uses/modifies
	}
	else if (arg2->isSynonym() && !isArg2SynonymValidRef) {
		// check if arg2 is a declared entity thats not a variable
		throw SemanticErrorException("Semantic error: 2nd argument is not a valid entity for Uses/Modifies");
	}
	else { // should be a syntactic error in the query
		throw SyntaxErrorException("Syntax error: Error in validating the clause for Uses/Modifies");
	}
}

bool validateEntEntArgs(shared_ptr<ClauseArg> arg1, shared_ptr<ClauseArg> arg2, const unordered_set<ENTITY>& validEnts1, 
	const unordered_set<ENTITY>& validEnts2) {
	bool isArg1SynonymEntRef{ arg1->isSynonym() && (validEnts1.find(arg1->getSynonym()->getEntityType()) != validEnts1.end()) };
	bool isArg2SynonymEntRef{ arg2->isSynonym() && (validEnts2.find(arg2->getSynonym()->getEntityType()) != validEnts2.end()) };

	bool isArg1ValidEntRef{ isArg1SynonymEntRef || arg1->isIdentifier() || arg1->isWildcard() };
	bool isArg2ValidEntRef{ isArg2SynonymEntRef || arg2->isIdentifier() || arg2->isWildcard() };


	if (isArg1ValidEntRef && isArg2ValidEntRef) {
		return true;
	}
	else {
		if (arg1->isSynonym() && !isArg1SynonymEntRef) {
			throw SemanticErrorException("Arg 1 of ent ent clause is an invalid synonym");
		}
		else if (arg2->isSynonym() && !isArg2SynonymEntRef) {
			throw SemanticErrorException("Arg 2 of ent ent clause is an invalid synonym");
		}
		else {
			throw SyntaxErrorException("Ent ent clause has invalid args");
		}
	}
}

bool UsesObjectFactory::validateUsesStmtArgs(vector<shared_ptr<ClauseArg>> arguments) {
	return validateUsesModifiesArgs(arguments[0], arguments[1], validStmtEntities, validEntRefsArg2, false);
}

bool UsesObjectFactory::validateUsesPArgs(vector<shared_ptr<ClauseArg>> arguments) {
	return validateUsesModifiesArgs(arguments[0], arguments[1], validEntRefsArg1, validEntRefsArg2, true);
}

bool ModifiesObjectFactory::validateModifiesStmtArgs(vector<shared_ptr<ClauseArg>> arguments) {
	return validateUsesModifiesArgs(arguments[0], arguments[1], validStmtEntities, validEntRefsArg2, false);
}

bool ModifiesObjectFactory::validateModifiesPArgs(vector<shared_ptr<ClauseArg>> arguments) {
	return validateUsesModifiesArgs(arguments[0], arguments[1], validEntRefsArg1, validEntRefsArg2, true);
}

bool FollowsObjectFactory::validateFollowsArgs(vector<shared_ptr<ClauseArg>> arguments) {
	return validateStmtStmtArgs(arguments[0], arguments[1], validStmtEntities);
}

bool FollowsStarObjectFactory::validateFollowsStarArgs(vector<shared_ptr<ClauseArg>> arguments) {
	return validateStmtStmtArgs(arguments[0], arguments[1], validStmtEntities);
}

bool ParentObjectFactory::validateParentArgs(vector<shared_ptr<ClauseArg>> arguments) {
	return validateStmtStmtArgs(arguments[0], arguments[1], validStmtEntities);
}

bool ParentStarObjectFactory::validateParentStarArgs(vector<shared_ptr<ClauseArg>> arguments) {
	return validateStmtStmtArgs(arguments[0], arguments[1], validStmtEntities);
}

bool CallsObjectFactory::validateCallsArgs(vector<shared_ptr<ClauseArg>> arguments) {
	return validateEntEntArgs(arguments[0], arguments[1], validEntRefsArgs, validEntRefsArgs);
}

bool CallsStarObjectFactory::validateCallsStarArgs(vector<shared_ptr<ClauseArg>> arguments) {
	return validateEntEntArgs(arguments[0], arguments[1], validEntRefsArgs, validEntRefsArgs);
}

bool NextObjectFactory::validateNextArgs(vector<shared_ptr<ClauseArg>> arguments) {
	return validateStmtStmtArgs(arguments[0], arguments[1], validStmtEntities);
}

bool NextStarObjectFactory::validateNextStarArgs(vector<shared_ptr<ClauseArg>> arguments) {
	return validateStmtStmtArgs(arguments[0], arguments[1], validStmtEntities);
}

bool AffectsObjectFactory::validateAffectsArgs(vector<shared_ptr<ClauseArg>> arguments) {
	return validateStmtStmtArgs(arguments[0], arguments[1], validStmtEntities);
}