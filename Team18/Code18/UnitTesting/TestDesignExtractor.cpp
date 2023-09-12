#include "stdafx.h"
#include "CppUnitTest.h"
#include <string>
#include "../source/DesignExtractor/Entity.h"
#include "../source/DesignExtractor/DesignExtractor.h"
#include "../source/AST/ASTNode.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(TestDesignExtractor)
	{
	public:
		TEST_METHOD(TestSimpleASTExtraction)
		{
            //CREATE STATEMENT NODES TO TEST
            //Create ReadNode with VariableNode
            std::shared_ptr<VariableNode> testVariableNode = std::make_shared<VariableNode>("x", 1);
            std::shared_ptr<ReadNode> testReadNode = std::make_shared<ReadNode>(1, testVariableNode);

            //Create PrintNode with VariableNode
            std::shared_ptr<VariableNode> testVariableNode2 = std::make_shared<VariableNode>("y", 2);
            std::shared_ptr<PrintNode> testPrintNode = std::make_shared<PrintNode>(2, testVariableNode2);

            // //Create AssignNode
            std::shared_ptr<VariableNode> testVariableNode3 = std::make_shared<VariableNode>("z", 3);
            std::shared_ptr<ConstantNode> testConstantNode1 = std::make_shared<ConstantNode>(10);
            std::shared_ptr<ConstantNode> testConstantNode2 = std::make_shared<ConstantNode>(20);
            std::shared_ptr<PlusNode> testPlusNode = std::make_shared<PlusNode>(testConstantNode1, testConstantNode2);
            std::shared_ptr<AssignNode> testAssignNode = std::make_shared<AssignNode>(3, testVariableNode3, testPlusNode);

            //Create PrintNode with VariableNode
            std::shared_ptr<VariableNode> testVariableNode4 = std::make_shared<VariableNode>("x", 4);
            std::shared_ptr<PrintNode> testPrintNode2 = std::make_shared<PrintNode>(4, testVariableNode4);

            //Create ReadNode with VariableNode
            std::shared_ptr<VariableNode> testVariableNode5 = std::make_shared<VariableNode>("x", 5);
            std::shared_ptr<ReadNode> testReadNode2 = std::make_shared<ReadNode>(5, testVariableNode5);            

            //Combine the above statement nodes
            std::vector<std::shared_ptr<StatementNode>> testStatementNodes = {testReadNode, testPrintNode, testPrintNode2, testReadNode2, testAssignNode};

            //CREATE PROCEDURE NODE TO TEST
            std::shared_ptr<ProcedureNode> testProcedureNode1 = std::make_shared<ProcedureNode>("procedure1", testStatementNodes);
            std::vector<std::shared_ptr<ProcedureNode>>testProcedureNodes = {testProcedureNode1};

            //Create program node
            std::shared_ptr<ProgramNode> testProgramNode = std::make_shared<ProgramNode>(testProcedureNodes);
            
            // Create Design Extractor
            DesignExtractor testDesignExtractor = DesignExtractor();

            // Extract the entities from the root node of the AST
            testDesignExtractor.extractEntities(testProgramNode);

            //Get entities from design extractor
            std::shared_ptr<ProcedureEntity> testProcedureEntity = testDesignExtractor.getProcedureEntity();
            std::shared_ptr<StatementEntity> testStatementEntity = testDesignExtractor.getStatementEntity();
            std::shared_ptr<VariableEntity> testVariableEntity = testDesignExtractor.getVariableEntity();
            std::shared_ptr<ConstantEntity> testConstantEntity = testDesignExtractor.getConstantEntity();
            
            //Get maps from entities
            std::map<std::string, std::vector<std::string>> procedureMap = *(testProcedureEntity->getMap());
            std::map<std::string, std::vector<std::string>> statementMap = *(testStatementEntity->getMap());
            std::map<std::string, std::vector<std::string>> variableMap = *(testVariableEntity->getMap());
            std::map<std::string, std::vector<std::string>> constantMap = *(testConstantEntity->getMap());

            //Check if the procedure entity is extracted correctly
            Assert::AreEqual(procedureMap["procedure1"].empty(), true);

            //Check if the statement entity is extracted correctly
            
            Assert::AreEqual(statementMap["read"][0], std::to_string(1));
            Assert::AreEqual(statementMap["read"][1], std::to_string(5));
            Assert::AreEqual(statementMap["print"][0], std::to_string(2));
            Assert::AreEqual(statementMap["print"][1], std::to_string(4));    
            Assert::AreEqual(statementMap["assign"][0], std::to_string(3));
            Assert::AreEqual(statementMap["stmt"][0], std::to_string(1));
            Assert::AreEqual(statementMap["stmt"][1], std::to_string(2));
            Assert::AreEqual(statementMap["stmt"][2], std::to_string(3));
            Assert::AreEqual(statementMap["stmt"][3], std::to_string(4));
            Assert::AreEqual(statementMap["stmt"][4], std::to_string(5));
            
            //Check if the variable entity is extracted correctly
            Assert::AreEqual(variableMap["x"][0], std::to_string(1));
            Assert::AreEqual(variableMap["y"][0], std::to_string(2));
            Assert::AreEqual(variableMap["x"][1], std::to_string(4));
            Assert::AreEqual(variableMap["x"][2], std::to_string(5));
            Assert::AreEqual(variableMap["z"][0], std::to_string(3));
            //Check if the constant entity is extracted correctly
            Assert::AreEqual(constantMap["10"][0], std::to_string(3));
            Assert::AreEqual(constantMap["20"][0], std::to_string(3));

		}
    };
}