#pragma once
#include<stdio.h>
#include <iostream>
#include <string>

#include "../SimpleProcessor/ProcessedProgram.h"
#include "../SimpleProcessor/ProcessedProcedure.h"
#include "../SimpleProcessor/ProcessedStmtList.h"
#include "../SimpleProcessor/ProcessedStmt.h"
#include "../SimpleProcessor/ProcessedWhileStmt.h"
#include "../SimpleProcessor/ProcessedIfStmt.h"
#include "../SimpleProcessor/ProcessedAssignStmt.h"
#include "../SimpleProcessor/ProcessedCallStmt.h"
#include "../SimpleProcessor/ProcessedReadStmt.h"
#include "../SimpleProcessor/ProcessedPrintStmt.h"
#include "../AST/Node.h"
#include "../AST/TerminalNode.h"

using namespace std;


/**
 * This class is used as the base class to all the extractors. It is used to extract all the designs from the AST.
 * Methods are virtual so that they can be overriden by the individual extractors.
 */
class Extractor: public std::enable_shared_from_this<Extractor> {
public:
    // Constructor
    virtual ~Extractor() = default;

    virtual void extract(shared_ptr<ProcessedProgram> processedProgram) {};
    virtual void extract(shared_ptr<ProcessedProcedure> processedProcedure) {};
    virtual void extract(shared_ptr<ProcessedStmtList> processedStmtList) {};
    virtual void extract(shared_ptr<ProcessedStmtList> processedStmtList, std::string procedureName) {};
    virtual std::unordered_set<std::string> extract(shared_ptr<ProcessedStmtList> processedStmtList, std::unordered_set<std::string> &prevStatementNumbers) { return {}; };
    virtual void extract(shared_ptr<ProcessedStmt> processedStmt) {};
    virtual void extract(shared_ptr<ProcessedStmt> processedStmt, std::string procedureName) {};
    virtual void extract(shared_ptr<ProcessedStmt> processedStmt, std::unordered_set<std::string>& prevStatementNumbers) {};
    virtual void extract(shared_ptr<ProcessedWhileStmt> processedWhile) {};
    virtual void extract(shared_ptr<ProcessedWhileStmt> processedWhile, string procedureName) {};
    virtual void extract(shared_ptr<ProcessedWhileStmt> processedWhile, std::unordered_set<std::string>& prevStatementNumbers) {} ;
    virtual void extract(shared_ptr<ProcessedIfStmt> processedIf) {};
    virtual void extract(shared_ptr<ProcessedIfStmt> processedIf, string procedureName) {};
    virtual void extract(shared_ptr<ProcessedIfStmt> processedIf, std::unordered_set<std::string>& prevStatementNumbers) {} ;
    virtual void extract(shared_ptr<ProcessedAssignStmt> processedAssign) {};
    virtual void extract(shared_ptr<ProcessedCallStmt> processedCall) {};
    virtual void extract(shared_ptr<ProcessedReadStmt> processedRead) {};
    virtual void extract(shared_ptr<ProcessedPrintStmt> processedPrint) {};
    virtual void extract(shared_ptr<OpNode> node) {};
    virtual void extract(shared_ptr<VariableNode> node) {};
    virtual void extract(shared_ptr<ConstantNode> node) {};

};
