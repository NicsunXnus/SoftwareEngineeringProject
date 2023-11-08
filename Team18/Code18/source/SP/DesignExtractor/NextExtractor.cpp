#include "NextExtractor.h"

void NextExtractor::extract(shared_ptr<ProcessedStmtList> processedStmtList) {
	this->traverse(processedStmtList->getStmts());
}

unordered_set<string> NextExtractor::extract(shared_ptr<ProcessedStmtList> processedStmtList, unordered_set<string>& prevStatementNumbers) {
	vector<shared_ptr<ProcessedStmt>> statements = processedStmtList->getStmts();
	for (auto& stmt : statements) {
		string statementNumber = stmt->getStatementNumberInString();
		if (!prevStatementNumbers.empty()) {
			for (const auto& prevStatementNumber : prevStatementNumbers) {
				this->insertToAbstractionMap(prevStatementNumber, statementNumber);
			}
			prevStatementNumbers.clear();
		}
		stmt->accept(shared_from_this(), prevStatementNumbers);

		if (stmt == statements.back()) {
			this->insertKeyToAbstractionMap(statementNumber);
		}
	}
	return prevStatementNumbers;
}

void NextExtractor::extract(shared_ptr<ProcessedStmt> processedStmt, unordered_set<string>& prevStatementNumbers) {
	prevStatementNumbers.insert(processedStmt->getStatementNumberInString());
}

void NextExtractor::extract(shared_ptr<ProcessedWhileStmt> processedWhile, unordered_set<string>& prevStatementNumbers) {
	string whileStatementNumber = processedWhile->getStatementNumberInString(); // Store the while statement number
	string firstStatementNumber = processedWhile->getWhileBlock()->getStmts().front()->getStatementNumberInString();
	this->insertToAbstractionMap(whileStatementNumber, firstStatementNumber);

	unordered_set<string> whileStatements;

	// Traverse the statements within the while loop
	unordered_set<string> lastInLoopSet = processedWhile->getWhileBlock()->accept(shared_from_this(), prevStatementNumbers);

	// Make the last statements of the loop reference back to the while loop
	for (const auto& lastInLoop : lastInLoopSet) {
		this->insertToAbstractionMap(lastInLoop, whileStatementNumber);
	}
	prevStatementNumbers.clear();
	prevStatementNumbers.insert(whileStatementNumber);
}

void NextExtractor::extract(shared_ptr<ProcessedIfStmt> processedIf, unordered_set<string>& prevStatementNumbers) {
	string statementNumber = processedIf->getStatementNumberInString();
    string firstStatementNumberThen = processedIf->getThenBlock()->getStmts().front()->getStatementNumberInString();// If Block CFG
    this->insertToAbstractionMap(statementNumber, firstStatementNumberThen);

    string firstStatementNumberElse = processedIf->getElseBlock()->getStmts().front()->getStatementNumberInString(); // Else Block CFG
    this->insertToAbstractionMap(statementNumber, firstStatementNumberElse);

    unordered_set<string> elsePrevStatementNumbers = prevStatementNumbers;
	unordered_set<string> lastInIfSet = processedIf->getThenBlock()->accept(shared_from_this(), prevStatementNumbers);
    unordered_set<string> lastInElseSet = processedIf->getElseBlock()->accept(shared_from_this(), elsePrevStatementNumbers);

    prevStatementNumbers.insert(lastInIfSet.begin(), lastInIfSet.end());
    prevStatementNumbers.insert(lastInElseSet.begin(), lastInElseSet.end());
}

void NextExtractor::insertKeyToAbstractionMap(string key) {
    if (getStorageMap()->find(key) == getStorageMap()->end()) {
		getStorageMap()->insert({ key, {}});
    }
}

void NextExtractor::traverse(vector<shared_ptr<ProcessedStmt>> processedStatements) {
	unordered_set<string> prevStatementNumbers;
	for (auto& stmt : processedStatements) {
		string statementNumber = stmt->getStatementNumberInString();
		if (!prevStatementNumbers.empty()) {
			for (const auto& prevStatementNumber : prevStatementNumbers) {
				insertToAbstractionMap(prevStatementNumber, statementNumber);
			}
			prevStatementNumbers.clear();
		}
		stmt->accept(shared_from_this(), prevStatementNumbers);

		if (stmt == processedStatements.back()) {
			insertKeyToAbstractionMap(statementNumber);
			prevStatementNumbers.insert(statementNumber);
		}
	}
}