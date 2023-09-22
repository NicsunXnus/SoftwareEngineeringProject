#ifndef ENTITY_H
#define ENTITY_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "../AST/ASTNode.h"

using namespace std;

typedef map<string, vector<string>> StringMap;

class ProcedureEntity;
class StatementEntity;
class VariableEntity;
class ConstantEntity;
class PatternStatementEntity;


/**
* This class represents the Entity component of the Design Extractor.
*/
class Entity {
public:
    Entity() = default;
    
    virtual ~Entity() = default;

    // Method to get the map
    shared_ptr<StringMap> getMap() {
      return make_shared<StringMap>(this->EntityStorageMap);
    }

    // Overloaded function for inserting to map with line number
    void insertToMap(string name, int lineNumber) {
        if (this->EntityStorageMap.find(name) == this->EntityStorageMap.end()) {
            this->EntityStorageMap[name] = vector<string>();
        }
        this->EntityStorageMap[name].push_back(to_string(lineNumber));
    }

    // Overloaded function for inserting to map without line number
    void insertToMap(string name) {
        if (this->EntityStorageMap.find(name) == this->EntityStorageMap.end()) {
            this->EntityStorageMap[name] = vector<string>();
        }
    }

    // Abstract method to be implemented by the individual entities
    virtual void extractEntity(shared_ptr<ASTNode> astNode) = 0;

private:
    map<string, vector<string>> EntityStorageMap;
};

/*
* The following classes are used to extract the different types of entities.
*/

class ProcedureEntity : public Entity {
public:
    void extractEntity(shared_ptr<ASTNode> astNode) override {
        // Check if astNode is a ProcedureNode
        shared_ptr<ProcedureNode> procedureNode = dynamic_pointer_cast<ProcedureNode>(astNode);

        if (procedureNode) {
            // If astNode is indeed a ProcedureNode, extract information
            string procedureName = procedureNode->getName();
            insertToMap(procedureName);
        }
        else {
            // Handle other cases or report an error
            cerr << "Unsupported ASTNode type." << endl;
        }
    }
};

class StatementEntity : public Entity {
public:
    void extractEntity(shared_ptr<ASTNode> astNode) override {
        // Check if astNode is a StatementNode
        shared_ptr<StatementNode> statementNode = dynamic_pointer_cast<StatementNode>(astNode);
        
        if (statementNode) {
            // If astNode is indeed a StatementNode, extract information
            string statementName = statementNode->getName();
            
            // Get the line number from astNode 
            int lineNumber = astNode->getStatementNumber();
            
            // Store the statement name as the key and add the line number to the vector
            insertToMap(statementName, lineNumber);
            insertToMap("stmt", lineNumber);
        }
        else {
            cerr << "Unsupported ASTNode type." << endl;
        }
    }
};

class VariableEntity : public Entity {
public:
    void extractEntity(shared_ptr<ASTNode> astNode) override {
        // Check if astNode is a VariableNode 
        shared_ptr<VariableNode> variableNode = dynamic_pointer_cast<VariableNode>(astNode);
        
        if (variableNode) {
            // If astNode is indeed a VariableNode, extract information
            string variableName = variableNode->getValue(); 
            int lineNumber = astNode->getStatementNumber();
            
            // Store the variable name as the key and add the line number to the vector
            insertToMap(variableName, lineNumber);
            
        }
        else {
            cerr << "Unsupported ASTNode type." << endl;
        }
    }
};

class ConstantEntity : public Entity {
public:
    void extractEntity(shared_ptr<ASTNode> astNode) override {
        // Check if astNode is a ConstantNode
        shared_ptr<ConstantNode> constantNode = dynamic_pointer_cast<ConstantNode>(astNode);

        if (constantNode) {
            // If astNode is indeed a ConstantNode, extract information
            string constantValue = constantNode->getValue();
            int lineNumber = astNode->getStatementNumber();
            
            // Store the constant value as the key and add the line number to the vector
            insertToMap(constantValue, lineNumber);
        }
        else {
            cerr << "Unsupported ASTNode type." << endl;
        }
    }
};

#endif