#include "stdafx.h"
#include "CppUnitTest.h"
#include <string>
#include "../source/DesignExtractor/Entity.h"
#include "../source/DesignExtractor/DesignExtractor.h"
#include "../source/AST/ASTNode.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
      TEST_CLASS(TestDesignExtractor){
            public :
                TEST_METHOD(TestSimpleASTExtraction){
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
                  std::shared_ptr<EqualNode> testEqualNode = std::make_shared<EqualNode>(testVariableNode6, testConstantNode3);
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
                  std::shared_ptr<VariableNode> testVariableNode9 = std::make_shared<VariableNode>("a", 12);
                  std::shared_ptr<ConstantNode> testConstantNode8 = std::make_shared<ConstantNode>(10, 12);
                  std::shared_ptr<EqualNode> testEqualNode2 = std::make_shared<EqualNode>(testVariableNode9, testConstantNode8);
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
                  std::map<std::string, std::vector<std::string>> procedureMap = *(testProcedureEntity->getMap());
                  std::map<std::string, std::vector<std::string>> statementMap = *(testStatementEntity->getMap());
                  std::map<std::string, std::vector<std::string>> variableMap = *(testVariableEntity->getMap());
                  std::map<std::string, std::vector<std::string>> constantMap = *(testConstantEntity->getMap());

                  // Check if the procedure entity is extracted correctly
                  Assert::AreEqual(procedureMap["procedure1"].empty(), true);

                  // Check if the statement entity is extracted correctly
                  Assert::AreEqual(statementMap["read"][0], std::to_string(1));
                  Assert::AreEqual(statementMap["read"][1], std::to_string(5));
                  Assert::AreEqual(statementMap["read"][2], std::to_string(8));
                  Assert::AreEqual(statementMap["print"][0], std::to_string(2));
                  Assert::AreEqual(statementMap["print"][1], std::to_string(4));
                  Assert::AreEqual(statementMap["print"][2], std::to_string(10));
                  Assert::AreEqual(statementMap["print"][3], std::to_string(12));
                  Assert::AreEqual(statementMap["assign"][0], std::to_string(3));
                  Assert::AreEqual(statementMap["assign"][1], std::to_string(7));
                  Assert::AreEqual(statementMap["assign"][2], std::to_string(9));
                  Assert::AreEqual(statementMap["stmt"][0], std::to_string(1));
                  Assert::AreEqual(statementMap["stmt"][1], std::to_string(2));
                  Assert::AreEqual(statementMap["stmt"][2], std::to_string(3));
                  Assert::AreEqual(statementMap["stmt"][3], std::to_string(4));
                  Assert::AreEqual(statementMap["stmt"][4], std::to_string(5));
                  Assert::AreEqual(statementMap["stmt"][5], std::to_string(6));
                  Assert::AreEqual(statementMap["stmt"][6], std::to_string(7));
                  Assert::AreEqual(statementMap["stmt"][7], std::to_string(8));
                  Assert::AreEqual(statementMap["stmt"][8], std::to_string(9));
                  Assert::AreEqual(statementMap["stmt"][9], std::to_string(10));
                  Assert::AreEqual(statementMap["stmt"][10], std::to_string(11));
                  Assert::AreEqual(statementMap["stmt"][11], std::to_string(12));
                  Assert::AreEqual(statementMap["stmt"][12], std::to_string(13));
                  Assert::AreEqual(statementMap["stmt"][13], std::to_string(14));
                  Assert::AreEqual(statementMap["if"][0], std::to_string(6));
                  Assert::AreEqual(statementMap["while"][0], std::to_string(12));
                  Assert::AreEqual(statementMap["call"][0], std::to_string(14));


                  // Check if the variable entity is extracted correctly
                  Assert::AreEqual(variableMap["x"][0], std::to_string(1));
                  Assert::AreEqual(variableMap["x"][1], std::to_string(4));
                  Assert::AreEqual(variableMap["x"][2], std::to_string(5));
                  Assert::AreEqual(variableMap["x"][3], std::to_string(10));

                  Assert::AreEqual(variableMap["y"][0], std::to_string(2));
                  Assert::AreEqual(variableMap["y"][1], std::to_string(8));

                  Assert::AreEqual(variableMap["z"][0], std::to_string(3));
                  Assert::AreEqual(variableMap["z"][1], std::to_string(6));
                  Assert::AreEqual(variableMap["z"][2], std::to_string(7));
                  Assert::AreEqual(variableMap["z"][3], std::to_string(9));
                  
                  Assert::AreEqual(variableMap["a"][0], std::to_string(11));
                  Assert::AreEqual(variableMap["a"][1], std::to_string(12));
                  Assert::AreEqual(variableMap["a"][2], std::to_string(13));

                  // Check if the constant entity is extracted correctly
                  Assert::AreEqual(constantMap["10"][0], std::to_string(3));
                  Assert::AreEqual(constantMap["10"][1], std::to_string(6));
                  Assert::AreEqual(constantMap["10"][2], std::to_string(11));
                  Assert::AreEqual(constantMap["10"][3], std::to_string(12));

                  Assert::AreEqual(constantMap["20"][0], std::to_string(3));
                  Assert::AreEqual(constantMap["20"][1], std::to_string(7));
                  
                  Assert::AreEqual(constantMap["30"][0], std::to_string(9));

                  Assert::AreEqual(constantMap["40"][0], std::to_string(9));
                  }

    };
}