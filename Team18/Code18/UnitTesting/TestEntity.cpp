#include "stdafx.h"
#include "CppUnitTest.h"
#include "../source/DesignExtractor/Entity.h"
#include "../source/AST/ASTNode.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(TestEntity)
	{
	public:
		TEST_METHOD(TestProcedureEntity)
		{
            
            std::vector<std::shared_ptr<StatementNode>> emptyStatements;

            //Create procedure nodes
            std::shared_ptr<ProcedureNode> testProcdeureNode1 = std::make_shared<ProcedureNode>("procedure1", emptyStatements);
            std::shared_ptr<ProcedureNode> testProcdeureNode2 = std::make_shared<ProcedureNode>("procedure2", emptyStatements);
            std::shared_ptr<ProcedureNode> testProcdeureNode3 = std::make_shared<ProcedureNode>("procedure3", emptyStatements);
            std::shared_ptr<ProcedureNode> testProcdeureNode4 = std::make_shared<ProcedureNode>("procedure4", emptyStatements);
            std::shared_ptr<ProcedureNode> testProcdeureNode5 = std::make_shared<ProcedureNode>("procedure5", emptyStatements);

            // Combine procedure nodes into a vector
            std::vector<std::shared_ptr<ProcedureNode>>
                testProcedureNodes = {testProcdeureNode1, testProcdeureNode2, testProcdeureNode3, testProcdeureNode4, testProcdeureNode5};

            // Create procedure entity
            std::shared_ptr<ProcedureEntity> testProcedureEntity = std::make_shared<ProcedureEntity>();

            // Extract procedure entity from each procedure node
            for (const auto &procedure : testProcedureNodes) {
                testProcedureEntity->extractEntity(procedure);
            }

            //Get procedure map from procedure entity
            std::map<std::string, std::vector<int>> procedureMap = testProcedureEntity->getMap();

            //Check if procedure entity is extracted correctly
            Assert::AreEqual(procedureMap["procedure1"][0], -1);
            Assert::AreEqual(procedureMap["procedure2"][0], -1);
            Assert::AreEqual(procedureMap["procedure3"][0], -1);
            Assert::AreEqual(procedureMap["procedure4"][0], -1);
            Assert::AreEqual(procedureMap["procedure5"][0], -1);
            
            //Create a program node
            // std::shared_ptr<ProgramNode> testProgramNode = std::make_shared<ProgramNode>("test", testProcedureNodes)

		}
		TEST_METHOD(TestStatementEntity)
		{
            //Create statement nodes
            std::shared_ptr<StatementNode> testStatementNode1 = std::make_shared<StatementNode>("statement1", 1);
            std::shared_ptr<StatementNode> testStatementNode2 = std::make_shared<StatementNode>("statement1", 2);
            std::shared_ptr<StatementNode> testStatementNode3 = std::make_shared<StatementNode>("statement2", 3);
            std::shared_ptr<StatementNode> testStatementNode4 = std::make_shared<StatementNode>("statement3", 4);
            std::shared_ptr<StatementNode> testStatementNode5 = std::make_shared<StatementNode>("statement3", 5);

            // Combine statement nodes into a vector
            std::vector<std::shared_ptr<StatementNode>>
                testStatementNodes = {testStatementNode1, testStatementNode2, testStatementNode3, testStatementNode4, testStatementNode5};

            // Create statement entity
            std::shared_ptr<StatementEntity> testStatementEntity = std::make_shared<StatementEntity>();

            // Extract statment entity from each statement node
            for (const auto &statement : testStatementNodes) {
                testStatementEntity->extractEntity(statement);
            }

            //Get statement map from statement entity
            std::map<std::string, std::vector<int>> statementMap = testStatementEntity->getMap();

            //Check if the statement entity is extracted correctly
            Assert::AreEqual(statementMap["statement1"][0], 1);
            Assert::AreEqual(statementMap["statement1"][1], 2);
            Assert::AreEqual(statementMap["statement2"][0], 3);
            Assert::AreEqual(statementMap["statement3"][0], 4);
            Assert::AreEqual(statementMap["statement3"][1], 5);
		}

        TEST_METHOD(TestVariableEntity)
		{
            //Create variable nodes
            std::shared_ptr<VariableNode> testVariableNode1 = std::make_shared<VariableNode>("variable1", 1);
            std::shared_ptr<VariableNode> testVariableNode2 = std::make_shared<VariableNode>("variable1", 2);
            std::shared_ptr<VariableNode> testVariableNode3 = std::make_shared<VariableNode>("variable2", 3);
            std::shared_ptr<VariableNode> testVariableNode4 = std::make_shared<VariableNode>("variable2", 4);
            std::shared_ptr<VariableNode> testVariableNode5 = std::make_shared<VariableNode>("variable3", 5);

            // Combine variable nodes into a vector
            std::vector<std::shared_ptr<VariableNode>>
                testVariableNodes = {testVariableNode1, testVariableNode2, testVariableNode3, testVariableNode4, testVariableNode5};

            // Create variable entity
            std::shared_ptr<VariableEntity> testVariableEntity = std::make_shared<VariableEntity>();

            // Extract variable entity from each variable node
            for (const auto &variable : testVariableNodes) {
                testVariableEntity->extractEntity(variable);
            }

            //Get variable map from variable entity
            std::map<std::string, std::vector<int>> variableMap = testVariableEntity->getMap();

            //Check if the statement entity is extracted correctly
            Assert::AreEqual(variableMap["variable1"][0], 1);
            Assert::AreEqual(variableMap["variable1"][1], 2);
            Assert::AreEqual(variableMap["variable2"][0], 3);
            Assert::AreEqual(variableMap["variable2"][1], 4);
            Assert::AreEqual(variableMap["variable3"][0], 5);
		}

        TEST_METHOD(TestConstantEntity)
		{
            //Create constant nodes
            std::shared_ptr<ConstantNode> testConstantNode1 = std::make_shared<ConstantNode>(1, 1);
            std::shared_ptr<ConstantNode> testConstantNode2 = std::make_shared<ConstantNode>(1, 2);
            std::shared_ptr<ConstantNode> testConstantNode3 = std::make_shared<ConstantNode>(2, 3);
            std::shared_ptr<ConstantNode> testConstantNode4 = std::make_shared<ConstantNode>(2, 4);
            std::shared_ptr<ConstantNode> testConstantNode5 = std::make_shared<ConstantNode>(3, 5);

            // Combine constant nodes into a vector
            std::vector<std::shared_ptr<ConstantNode>>
                testConstantNodes = {testConstantNode1, testConstantNode2, testConstantNode3, testConstantNode4, testConstantNode5};
            
            // Create constant entity
            std::shared_ptr<ConstantEntity> testConstantEntity = std::make_shared<ConstantEntity>();

            // Extract constant entity from each constant node
            for (const auto &constant : testConstantNodes) {
                testConstantEntity->extractEntity(constant);
            }

            //Get constant map from constant entity
            std::map<std::string, std::vector<int>> constantMap = testConstantEntity->getMap();

            //Check if the statement entity is extracted correctly
            Assert::AreEqual(constantMap["1"][0], 1);
            Assert::AreEqual(constantMap["1"][1], 2);
            Assert::AreEqual(constantMap["2"][0], 3);
            Assert::AreEqual(constantMap["2"][1], 4);
            Assert::AreEqual(constantMap["3"][0], 5);
        }

	};
}