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
      private:
            static std::map<std::string, std::vector<std::string>> procedureMap;
            static std::map<std::string, std::vector<std::string>> statementMap;
            static std::map<std::string, std::vector<std::string>> variableMap;
            static std::map<std::string, std::vector<std::string>> constantMap;
      public :
            TEST_CLASS_INITIALIZE(ClassInitialize)
            {
                  // Set up common resources or build the necessary structure here
                  /* The code in mind
                        procedure procedure1 {
                              read x;
                              print y;
                              z = 10 + 20;
                              print x;
                              read x;
                              if (z == 10) then {
                                    z = 20 + 20;
                                    read y;
                              } else {
                                    z = 30 + 40;
                                    print x;
                              }
                              a = 10;
                              while (a == 10) {
                                    print a;
                                    call procedure2;
                              }
                        }
                  */
                  // Create ReadNode with VariableNode
                  std::shared_ptr<VariableNode> testVariableNode = std::make_shared<VariableNode>("x", 1);
                  std::shared_ptr<ReadNode> testReadNode = std::make_shared<ReadNode>(1, testVariableNode);

                  // Create PrintNode with VariableNode
                  std::shared_ptr<VariableNode> testVariableNode2 = std::make_shared<VariableNode>("y", 2);
                  std::shared_ptr<PrintNode> testPrintNode = std::make_shared<PrintNode>(2, testVariableNode2);

                  // //Create AssignNode
                  std::shared_ptr<VariableNode> testVariableNode3 = std::make_shared<VariableNode>("z", 3);
                  std::shared_ptr<ConstantNode> testConstantNode1 = std::make_shared<ConstantNode>(10, 3);
                  std::shared_ptr<ConstantNode> testConstantNode2 = std::make_shared<ConstantNode>(20, 3);
                  std::shared_ptr<PlusNode> testPlusNode = std::make_shared<PlusNode>(testConstantNode1, testConstantNode2);
                  std::shared_ptr<AssignNode> testAssignNode = std::make_shared<AssignNode>(3, testVariableNode3, testPlusNode);

                  // Create PrintNode with VariableNode
                  std::shared_ptr<VariableNode> testVariableNode4 = std::make_shared<VariableNode>("x", 4);
                  std::shared_ptr<PrintNode> testPrintNode2 = std::make_shared<PrintNode>(4, testVariableNode4);

                  // Create ReadNode with VariableNode
                  std::shared_ptr<VariableNode> testVariableNode5 = std::make_shared<VariableNode>("x", 5);
                  std::shared_ptr<ReadNode> testReadNode2 = std::make_shared<ReadNode>(5, testVariableNode5);

                  // Create ifNode
                  std::shared_ptr<VariableNode> testVariableNode6 = std::make_shared<VariableNode>("z", 6);
                  std::shared_ptr<ConstantNode> testConstantNode3 = std::make_shared<ConstantNode>(10, 6);
                  std::shared_ptr<EqualsNode> testEqualNode = std::make_shared<EqualsNode>(testVariableNode6, testConstantNode3);
                  std::shared_ptr<VariableNode> testVariableNode7 = std::make_shared<VariableNode>("z", 7);
                  std::shared_ptr<ConstantNode> testConstantNode4 = std::make_shared<ConstantNode>(10, 7);
                  std::shared_ptr<ConstantNode> testConstantNode5 = std::make_shared<ConstantNode>(20, 7);
                  std::shared_ptr<PlusNode> testPlusNode2 = std::make_shared<PlusNode>(testConstantNode4, testConstantNode5);
                  std::shared_ptr<AssignNode> testAssignNode2 = std::make_shared<AssignNode>(7, testVariableNode7, testPlusNode2);
                  std::shared_ptr<VariableNode> testVariableNode10 = std::make_shared<VariableNode>("y", 8);
                  std::shared_ptr<ReadNode> testReadNode3 = std::make_shared<ReadNode>(8, testVariableNode10);
                  std::shared_ptr<VariableNode> testVariableNode8 = std::make_shared<VariableNode>("z", 9);
                  std::shared_ptr<ConstantNode> testConstantNode6 = std::make_shared<ConstantNode>(30, 9);
                  std::shared_ptr<ConstantNode> testConstantNode7 = std::make_shared<ConstantNode>(40, 9);
                  std::shared_ptr<PlusNode> testPlusNode3 = std::make_shared<PlusNode>(testConstantNode6, testConstantNode7);
                  std::shared_ptr<AssignNode> testAssignNode3 = std::make_shared<AssignNode>(9, testVariableNode8, testPlusNode3);
                  std::shared_ptr<VariableNode> testVariableNode11 = std::make_shared<VariableNode>("x", 10);
                  std::shared_ptr<PrintNode> testPrintNode3 = std::make_shared<PrintNode>(10, testVariableNode11);
                  std::vector<std::shared_ptr<StatementNode>> ifStatements = {testAssignNode2, testReadNode3};
                  std::vector<std::shared_ptr<StatementNode>> elseStatements = {testAssignNode3, testPrintNode3};
                  std::shared_ptr<IfNode> testIfNode = std::make_shared<IfNode>(6, testEqualNode, ifStatements, elseStatements);

                  // Add assign node
                  std::shared_ptr<VariableNode> testVariableNode9 = std::make_shared<VariableNode>("a", 11);
                  std::shared_ptr<ConstantNode> testConstantNode8 = std::make_shared<ConstantNode>(10, 11);
                  std::shared_ptr<AssignNode> testAssignNode4 = std::make_shared<AssignNode>(11, testVariableNode9, testConstantNode8);

                  // Create WhileNode
                  std::shared_ptr<VariableNode> testVariableNode13 = std::make_shared<VariableNode>("a", 12);
                  std::shared_ptr<ConstantNode> testConstantNode14 = std::make_shared<ConstantNode>(10, 12);
                  std::shared_ptr<EqualsNode> testEqualNode2 = std::make_shared<EqualsNode>(testVariableNode13, testConstantNode14);
                  std::shared_ptr<VariableNode> testVariableNode12 = std::make_shared<VariableNode>("a", 13);
                  std::shared_ptr<PrintNode> testPrintNode4 = std::make_shared<PrintNode>(13, testVariableNode12);
                  std::shared_ptr<ProcedureNode> testProcedureNode2 = std::make_shared<ProcedureNode>("procedure2", std::vector<std::shared_ptr<StatementNode>>());
                  std::shared_ptr<CallNode> testCallNode = std::make_shared<CallNode>(14, testProcedureNode2);
                  std::vector<std::shared_ptr<StatementNode>> whileStatements = {testPrintNode4, testCallNode};
                  std::shared_ptr<WhileNode> testWhileNode = std::make_shared<WhileNode>(12, testEqualNode2, whileStatements);

                  // Combine the above statement nodes
                  std::vector<std::shared_ptr<StatementNode>> testStatementNodes = {testReadNode, testPrintNode, testAssignNode, testPrintNode2, testReadNode2, testIfNode, testWhileNode};

                  // CREATE PROCEDURE NODE TO TEST
                  std::shared_ptr<ProcedureNode> testProcedureNode1 = std::make_shared<ProcedureNode>("procedure1", testStatementNodes);
                  std::vector<std::shared_ptr<ProcedureNode>> testProcedureNodes = {testProcedureNode1};

                  // Create program node
                  std::shared_ptr<ProgramNode> testProgramNode = std::make_shared<ProgramNode>(testProcedureNodes);

                  // Create Design Extractor
                  DesignExtractor testDesignExtractor = DesignExtractor();

                  // Extract the entities from the root node of the AST
                  testDesignExtractor.extractEntities(testProgramNode);

                  // Get entities from design extractor
                  std::shared_ptr<ProcedureEntity> testProcedureEntity = testDesignExtractor.getProcedureEntity();
                  std::shared_ptr<StatementEntity> testStatementEntity = testDesignExtractor.getStatementEntity();
                  std::shared_ptr<VariableEntity> testVariableEntity = testDesignExtractor.getVariableEntity();
                  std::shared_ptr<ConstantEntity> testConstantEntity = testDesignExtractor.getConstantEntity();

                  // Get maps from entities
                  procedureMap = *(testProcedureEntity->getMap());
                  statementMap = *(testStatementEntity->getMap());
                  variableMap = *(testVariableEntity->getMap());
                  constantMap = *(testConstantEntity->getMap());
            }

            TEST_METHOD(ProcedureExtraction)
            {
                  Assert::AreEqual(procedureMap["procedure1"].empty(), true);
            }

            TEST_METHOD(StatementExtraction)
            {
                  std::map<std::string, std::vector<std::string>> expectedValues = {
                  {"read", {"1", "5", "8"}},
                  {"print", {"2", "4", "10", "12"}},
                  {"assign", {"3", "7", "9"}},
                  {"stmt", {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14"}},
                  {"if", {"6"}},
                  {"while", {"12"}},
                  {"call", {"14"}}
                  };

                  // Loop through each statement type and check if the expected values are present
                  for (const auto& entry : expectedValues) {
                        const std::string& statementType = entry.first;
                        const std::vector<std::string>& expected = entry.second;
                        const std::vector<std::string>& actual = statementMap[statementType];

                        for (const auto& value : expected) {
                              Assert::AreEqual(value.c_str(), actual[std::distance(actual.begin(), std::find(actual.begin(), actual.end(), value))].c_str());
                        }
                  }
            }

            TEST_METHOD(VariableExtraction)
            {
                  std::map<std::string, std::vector<std::string>> expectedVariableValues = {
                  {"x", {"1", "4", "5", "10"}},
                  {"y", {"2", "8"}},
                  {"z", {"3", "6", "7", "9"}},
                  {"a", {"11", "12", "13"}}
                  };

                  // Loop through each statement type and check if the expected values are present
                  for (const auto& entry : expectedVariableValues) {
                        const std::string& variableName = entry.first;
                        const std::vector<std::string>& expected = entry.second;
                        const std::vector<std::string>& actual = variableMap[variableName];

                        for (const auto& value : expected) {
                              Assert::AreEqual(value.c_str(), actual[std::distance(actual.begin(), std::find(actual.begin(), actual.end(), value))].c_str());
                        }
                  }
            }

            TEST_METHOD(ConstantExtraction)
            {
                  std::map<std::string, std::vector<std::string>> expectedConstantValues = {
                  {"10", {"3", "6", "11", "12"}},
                  {"20", {"3", "7"}},
                  {"30", {"9"}},
                  {"40", {"9"}}
                  };

                  // Loop through each variable and check if the expected values are present
                  

                  // Loop through each constant and check if the expected values are present
                  for (const auto& entry : expectedConstantValues) {
                        const std::string& constantValue = entry.first;
                        const std::vector<std::string>& expected = entry.second;
                        const std::vector<std::string>& actual = constantMap[constantValue];

                        for (const auto& value : expected) {
                              Assert::AreEqual(value.c_str(), actual[std::distance(actual.begin(), std::find(actual.begin(), actual.end(), value))].c_str());
                        }
                  }
            }

    };
}