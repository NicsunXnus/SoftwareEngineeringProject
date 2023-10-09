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
#include "LineNumberToProcedureNameExtractor.h"

/**
 * This class is used to extract the Calls abstraction from the AST.
 */
class CallsAbstractionExtractor : public AbstractionExtractor {
public:
    // Constructor
    CallsAbstractionExtractor() {
        this->lineNumberToProcedureNameExtractor = make_shared<LineNumberToProcedureNameExtractor>();
    }
    
    // Override methods to avoid unnecessary processing
    void handleRead(std::shared_ptr<ReadNode> readNode) override {}
    void handlePrint(std::shared_ptr<PrintNode> printNode) override {}
    void handleAssign(std::shared_ptr<AssignNode> assignNode) override {}
    
    void handleCall(std::shared_ptr<CallNode> callNode) override {
        string statementNumber = to_string(callNode->getStatementNumber());
        string parentProcedureName = this->lineNumberToProcedureNameExtractor->getProcedureNameFromStatementNumber(statementNumber);
        string procedureCalledName = callNode->getProc()->getName();
        insertToAbstractionMap(parentProcedureName, procedureCalledName);
    }

    void handleProgram(std::shared_ptr<ProgramNode> programNode) {
        std::vector<std::shared_ptr<ProcedureNode>> procedures = programNode->getProcedures();
        for (const auto &procedure : procedures) {
            insertToAbstractionMap(procedure->getName());
            extractDesigns(procedure);
        }
    }

    void extractAbstractions(shared_ptr<ASTNode> astNode) override {
        // Create LineNumberToProcedureNameExtractor to extract the LineNumberToProcedureName abstraction
        this->lineNumberToProcedureNameExtractor->extractDesigns(astNode);
        extractDesigns(astNode);
        processProcedureNames();
    }

    shared_ptr<LineNumberToProcedureNameExtractor> getLineNumberToProcedureNameExtractor() {
        return this->lineNumberToProcedureNameExtractor;
    }


private:
    shared_ptr<LineNumberToProcedureNameExtractor> lineNumberToProcedureNameExtractor;

    // The method adds all procedures called indirectly by the parent procedure
    void processProcedureNames() {
        for (const auto& [parentProcedureName, procedureCalledNames] : *AbstractionStorageMap) {
            unordered_set<string> proceduresCalledIndirectly = unordered_set<string>();
            for (const auto& procedureCalledName : procedureCalledNames) {
                proceduresCalledIndirectly.insert(procedureCalledName);
                processProcedureNamesHelper(procedureCalledName, proceduresCalledIndirectly);
            }
            AbstractionStorageMap->at(parentProcedureName) = vector<string>(proceduresCalledIndirectly.begin(), proceduresCalledIndirectly.end());
        }
    }

    void processProcedureNamesHelper(string procedureCalledName, unordered_set<string>& proceduresCalledIndirectly) {
        if (AbstractionStorageMap->find(procedureCalledName) != AbstractionStorageMap->end()) {
            for (const auto& procedureName : AbstractionStorageMap->at(procedureCalledName)) {
                proceduresCalledIndirectly.insert(procedureName);
                processProcedureNamesHelper(procedureName, proceduresCalledIndirectly);
            }
        }
    }

};
