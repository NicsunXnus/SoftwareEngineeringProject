#pragma once
#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <unordered_set>

using namespace std;

#include "../AST/ASTNode.h"
#include "AbstractionExtractor.h"

/**
 */
class NextAbstractionExtractor : public AbstractionExtractor {
public:    
    // Constructor
    // NextAbstractionExtractor() {
    //     this->ifStorageMap = std::make_shared<map<string, vector<string>>>();
    // }

    void handleAssign(std::shared_ptr<AssignNode> assignNode) override {}
    void handleCall(std::shared_ptr<CallNode> callNode) override {}
    void handleRead(std::shared_ptr<ReadNode> readNode) override {}
    void handlePrint(std::shared_ptr<PrintNode> printNode) override {}
    void handleExpr(std::shared_ptr<ExprNode> exprNode) override {}
    void handleCondExpr(std::shared_ptr<CondExprNode> condExprNode) override {}

    void handleProcedure(std::shared_ptr<ProcedureNode> procedureNode) override {
        std::vector<std::shared_ptr<StatementNode>> statements = procedureNode->getStatements();
        traverse(statements);
    }

    void handleWhile(std::shared_ptr<WhileNode> whileNode) override {
        // std::vector<std::shared_ptr<StatementNode>> statements = whileNode->getStatements();
        // string whileStatementNumber = to_string(whileNode->getStatementNumber());
        
        // // Connect the first statement number to the while statement number
        // string firstStatementNumber = to_string(statements.front()->getStatementNumber());
        // insertToAbstractionMap(whileStatementNumber, firstStatementNumber);
        
        // string lastStatment = traverse(statements);
        // insertToAbstractionMap(lastStatment, whileStatementNumber);
    }

    void handleIf(std::shared_ptr<IfNode> ifNode) override {
        // std::vector<std::shared_ptr<StatementNode>> ifStatements = ifNode->getStatements();
        // std::vector<std::shared_ptr<StatementNode>> elseStatements = ifNode->getElseStatements();

        // string ifStatementNumber = to_string(ifNode->getStatementNumber());
        // // Get first value of ifStatements
        // string firstIfStatementNumber = to_string(ifStatements.front()->getStatementNumber());
        // // Get first value of elseStatements
        // string firstElseStatementNumber = to_string(elseStatements.front()->getStatementNumber());

        // insertToAbstractionMap(ifStatementNumber, firstIfStatementNumber);
        // insertToAbstractionMap(ifStatementNumber, firstElseStatementNumber);

        // string endIfStatementNumber = traverse(ifStatements);
        // string endElseStatementNumber = traverse(elseStatements);

        // insertToIfStorageMap(ifStatementNumber, endIfStatementNumber, endElseStatementNumber);
    }

    void extractAbstractions(shared_ptr<ASTNode> astNode) override {
        extractDesigns(astNode);
        // processIfLines();
    }

private:
    // std::shared_ptr<map<string, vector<string>>> ifStorageMap;

    // string traverse(std::vector<std::shared_ptr<StatementNode>> statements) {
    //     unordered_set<string> prevStatementNumbers = {};
    //     for (const auto &statement : statements){
    //         string statementNumber = to_string(statement->getStatementNumber());
    //         if (prevStatementNumbers.size() > 0) {
    //             // Add all the previous statement numbers to the current statement number
    //             for (const auto &prevStatementNumber : prevStatementNumbers) {
    //                 insertToAbstractionMap(prevStatementNumber, statementNumber);
    //             }
    //             // Clear the previous statement numbers
    //             prevStatementNumbers.clear();
    //         }

    //         // Insert the statement number into the previous statement numbers
    //         if (statement->getName() != "if") {
    //             prevStatementNumbers.insert(statementNumber);
    //         }

    //         if (statement->getName() == "while") {
    //             // Get the first statement number of the while statement
    //             string firstStatementNumber = to_string(statement->getStatements().front()->getStatementNumber());

    //             // Connect the first statement number to the while statement number
    //             insertToAbstractionMap(statementNumber, firstStatementNumber);

    //             // Get the last statement number of the while statement
    //             string lastStatementNumber = traverse(statement->getStatements());
    //             insertToAbstractionMap(lastStatementNumber, statementNumber);
    //             return lastStatementNumber;

    //         } else if (statement->getName() == "if") {
    //             // Get the first statement number of the if/else statement
    //             // Connect the first statement number to the if/else statement number
    //             string firstStatementNumber = to_string(statement->getStatements().front()->getStatementNumber());
    //             insertToAbstractionMap(statementNumber, firstStatementNumber);
    //             firstStatementNumber = to_string(statement->getElseStatements().front()->getStatementNumber());
    //             insertToAbstractionMap(statementNumber, firstStatementNumber);
                
    //             // Get the last statement number of the if/else statement
    //             string lastStatementNumber = traverse(statement->getStatements());
    //             prevStatementNumbers.insert(lastStatementNumber);
    //             lastStatementNumber = traverse(statement->getElseStatements());
    //             prevStatementNumbers.insert(lastStatementNumber);
    //             return lastStatementNumber;
    //         } else if (statement == statements.back()) {
    //             insertKeyToAbstractionMap(statementNumber);
    //             return statementNumber;
    //         }
    //     }
    //     return "SOMETHIGN IS NHOT WORKIGN";
    // }

    unordered_set<string> traverse(std::vector<std::shared_ptr<StatementNode>> statements) {
        unordered_set<string> lastStatements;

        for (size_t i = 0; i < statements.size(); ++i) {
            const auto &statement = statements[i];
            string statementNumber = to_string(statement->getStatementNumber());

            if (lastStatements.size() > 0) {
                for (const auto &prevStatementNumber : lastStatements) {
                    insertToAbstractionMap(prevStatementNumber, statementNumber);
                }
                lastStatements.clear();
            }

            if (statement->getName() != "if" && statement->getName() != "while") {
                lastStatements.insert(statementNumber);
            }

            if (statement->getName() == "while") {
                string whileStatementNumber = statementNumber; // Store the while statement number
                string firstStatementNumber = to_string(statement->getStatements().front()->getStatementNumber());
                insertToAbstractionMap(whileStatementNumber, firstStatementNumber);

                unordered_set<string> lastInLoopSet = traverse(statement->getStatements());

                // Make the last statements of the loop reference back to the while loop
                for (const auto &lastInLoop : lastInLoopSet) {
                    insertToAbstractionMap(lastInLoop, whileStatementNumber);
                }

                lastStatements.insert(lastInLoopSet.begin(), lastInLoopSet.end());

            } else if (statement->getName() == "if") {
                string firstStatementNumber = to_string(statement->getStatements().front()->getStatementNumber());
                insertToAbstractionMap(statementNumber, firstStatementNumber);

                string firstStatementNumberElse = to_string(statement->getElseStatements().front()->getStatementNumber());
                insertToAbstractionMap(statementNumber, firstStatementNumberElse);

                if (i == statements.size() - 1) {
                    // If the "if" statement is the last statement, insert its number as the last statement
                    insertKeyToAbstractionMap(statementNumber);
                    lastStatements.insert(statementNumber);
                } else {
                    // Otherwise, skip inserting the last values of "if" statements
                    unordered_set<string> lastInIfSet = traverse(statement->getStatements());
                    unordered_set<string> lastInElseSet = traverse(statement->getElseStatements());

                    lastStatements.insert(lastInIfSet.begin(), lastInIfSet.end());
                    lastStatements.insert(lastInElseSet.begin(), lastInElseSet.end());
                }
            }
        }

        return lastStatements;
    }

    // Replace the values of the keys for IF statemetns in the abstraction map with the values in the ifStorageMap
    // void processIfLines() {
    //     for (const auto& [key, value] : *ifStorageMap) {
    //         if (AbstractionStorageMap->find(key) != AbstractionStorageMap->end()) {
    //             AbstractionStorageMap->at(key) = value;
    //         }
    //     }
    // }

    void insertKeyToAbstractionMap(string key) {
        if (this->AbstractionStorageMap->find(key) == this->AbstractionStorageMap->end()) {
            this->AbstractionStorageMap->insert({ key, vector<string>() });
        }
    }

    // void insertToIfStorageMap(string key, string lastIfLineNumber, string lastElseLineNumber) {
    //     if (this->ifStorageMap->find(key) == this->ifStorageMap->end()) {
    //         this->ifStorageMap->insert({ key, vector<string>() });
    //     }
    //     if (std::find(this->ifStorageMap->at(key).begin(), this->ifStorageMap->at(key).end(), lastIfLineNumber) == this->ifStorageMap->at(key).end()) {
    //         this->ifStorageMap->at(key).push_back(lastIfLineNumber);
    //     }
    //     if (std::find(this->ifStorageMap->at(key).begin(), this->ifStorageMap->at(key).end(), lastElseLineNumber) == this->ifStorageMap->at(key).end()) {
    //         this->ifStorageMap->at(key).push_back(lastElseLineNumber);
    //     }
    // }



};