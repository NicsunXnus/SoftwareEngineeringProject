#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "../AST/ASTNode.h"
#include "../AST/ExprNode.h"
#include "ProcedureEntity.h" 


class Entity {
public:
    static std::shared_ptr<Entity> createEntity(const std::string& entityType) {
        if (entityType == "procedure") {
            return std::make_shared<ProcedureEntity>();
        // TODO: For pattern matching
        // } else if (entityType == "assign") {
        //     return std::make_shared<AssignStatementEntity>();
        } else if (entityType == "stmt") {
            return std::make_shared<StatementEntity>();
        } else if (entityType == "variable") {
            return std::make_shared<VariableEntity>();
        } else if (entityType == "constant") {
            return std::make_shared<ConstantEntity>();
        } else {
            // Handle unsupported entity types or return a default entity
            return std::make_shared<Entity>();
        }
    }

    std::map<std::string, std::vector<int>>getMap() {
        return this->EntityStorageMap;
    }

    std::string getEntityType() {
        return this->entityType;
    }

    void insert(std::string name, int lineNumber) {
        if (this->EntityStorageMap.find(name) == this->EntityStorageMap.end()) {
            this->EntityStorageMap[name] = std::vector<int>();
        }
        this->EntityStorageMap[name].push_back(lineNumber);
    }

    virtual ~Entity() = default;
    virtual void extractEntity(std::shared_ptr<ASTNode> astNode) = 0;

private:
    std::map<std::string, std::vector<int>> EntityStorageMap;
    std::string entityType;
};

class ProcedureEntity : public Entity {
public:
    void extractEntity(std::shared_ptr<ASTNode> astNode) override {
        // Check if astNode is a ProgramNode
        std::shared_ptr<ProgramNode> programNode = std::dynamic_pointer_cast<ProgramNode>(astNode);

        if (programNode) {
            // Extract the name of the procedure from the ProgramNode
            std::vector<std::shared_ptr<ProcedureNode>> procedures = programNode->getProcedures();
            
            // Iterate through the procedures and store their names in EntityStorageMap
            for (const auto& procedure : procedures) {
                std::string procedureName = procedure->getName();
                insert(procedureName, -1); 
            }
        }
    }
};

// Used for statements which do not have pattern matching
class StatementEntity : public Entity {
    void extractEntity(std::shared_ptr<ASTNode> astNode) override {
        // Check if astNode is a StatementNode
        std::shared_ptr<StatementNode> statementNode = std::dynamic_pointer_cast<StatementNode>(astNode);
        
        if (statementNode) {
            // If astNode is indeed a StatementNode, extract information
            std::string statementName = statementNode->getName();
            
            // Get the line number from astNode 
            int lineNumber = astNode->getStatementNumber();
            
            // Store the statement name as the key and add the line number to the vector
            insert(statementName, lineNumber);
        }
    }
};


// class AssignStatementEntity : public StatementEntity {
// public:
//     void extractEntity(ASTNode astNode) override {
//         // TODO: For pattern matching
//     }
// };

class VariableEntity : public Entity {
public:
    void extractEntity(std::shared_ptr<ASTNode> astNode) override {
        // Check if astNode is a VariableNode 
        std::shared_ptr<VariableNode> variableNode = std::dynamic_pointer_cast<VariableNode>(astNode);
        
        if (variableNode) {
            // If astNode is indeed a VariableNode, extract information
            std::string variableName = variableNode->getValue(); 
            int lineNumber = astNode->getStatementNumber();
            
            // Store the variable name as the key and add the line number to the vector
            insert(variableName, lineNumber);
        }
    }
};

class ConstantEntity : public Entity {
public:
    void extractEntity(std::shared_ptr<ASTNode> astNode) override {
        // Check if astNode is a ConstantNode
        std::shared_ptr<ProgramNode> constantNode = std::dynamic_pointer_cast<ConstantNode>(astNode);

        if (constantNode) {
            // If astNode is indeed a ConstantNode, extract information
            std::string constantValue = constantNode->getValue();
            int lineNumber = astNode->getStatementNumber();
            
            // Store the constant value as the key and add the line number to the vector
            insert(constantValue, lineNumber);
        }
    }
};


