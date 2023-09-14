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
      public :
            TEST_METHOD(TestSimpleASTExtraction)
            {
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
                  std::vector<std::shared_ptr<StatementNode>> testStatementNodes = {testReadNode, testPrintNode, testAssignNode, testPrintNode2, testReadNode2, testIfNode, testAssignNode4, testWhileNode};

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

                  Assert::AreEqual(procedureMap["procedure1"].empty(), true);

                  Assert::IsTrue(std::find(statementMap["read"].begin(), statementMap["read"].end(), std::to_string(1)) != statementMap["read"].end());
                  Assert::IsTrue(std::find(statementMap["read"].begin(), statementMap["read"].end(), std::to_string(5)) != statementMap["read"].end());
                  Assert::IsTrue(std::find(statementMap["read"].begin(), statementMap["read"].end(), std::to_string(8)) != statementMap["read"].end());

                  Assert::IsTrue(std::find(statementMap["print"].begin(), statementMap["print"].end(), std::to_string(2)) != statementMap["print"].end());
                  Assert::IsTrue(std::find(statementMap["print"].begin(), statementMap["print"].end(), std::to_string(4)) != statementMap["print"].end());
                  Assert::IsTrue(std::find(statementMap["print"].begin(), statementMap["print"].end(), std::to_string(10)) != statementMap["print"].end());
                  Assert::IsTrue(std::find(statementMap["print"].begin(), statementMap["print"].end(), std::to_string(13)) != statementMap["print"].end());

                  Assert::IsTrue(std::find(statementMap["stmt"].begin(), statementMap["stmt"].end(), std::to_string(1)) != statementMap["stmt"].end());
                  Assert::IsTrue(std::find(statementMap["stmt"].begin(), statementMap["stmt"].end(), std::to_string(2)) != statementMap["stmt"].end());
                  Assert::IsTrue(std::find(statementMap["stmt"].begin(), statementMap["stmt"].end(), std::to_string(3)) != statementMap["stmt"].end());
                  Assert::IsTrue(std::find(statementMap["stmt"].begin(), statementMap["stmt"].end(), std::to_string(4)) != statementMap["stmt"].end());
                  Assert::IsTrue(std::find(statementMap["stmt"].begin(), statementMap["stmt"].end(), std::to_string(5)) != statementMap["stmt"].end());
                  Assert::IsTrue(std::find(statementMap["stmt"].begin(), statementMap["stmt"].end(), std::to_string(6)) != statementMap["stmt"].end());
                  Assert::IsTrue(std::find(statementMap["stmt"].begin(), statementMap["stmt"].end(), std::to_string(7)) != statementMap["stmt"].end());
                  Assert::IsTrue(std::find(statementMap["stmt"].begin(), statementMap["stmt"].end(), std::to_string(8)) != statementMap["stmt"].end());
                  Assert::IsTrue(std::find(statementMap["stmt"].begin(), statementMap["stmt"].end(), std::to_string(9)) != statementMap["stmt"].end());
                  Assert::IsTrue(std::find(statementMap["stmt"].begin(), statementMap["stmt"].end(), std::to_string(10)) != statementMap["stmt"].end());
                  Assert::IsTrue(std::find(statementMap["stmt"].begin(), statementMap["stmt"].end(), std::to_string(11)) != statementMap["stmt"].end());
                  Assert::IsTrue(std::find(statementMap["stmt"].begin(), statementMap["stmt"].end(), std::to_string(12)) != statementMap["stmt"].end());
                  Assert::IsTrue(std::find(statementMap["stmt"].begin(), statementMap["stmt"].end(), std::to_string(13)) != statementMap["stmt"].end());
                  Assert::IsTrue(std::find(statementMap["stmt"].begin(), statementMap["stmt"].end(), std::to_string(14)) != statementMap["stmt"].end());

                  Assert::IsTrue(std::find(statementMap["if"].begin(), statementMap["if"].end(), std::to_string(6)) != statementMap["if"].end());

                  Assert::IsTrue(std::find(statementMap["while"].begin(), statementMap["while"].end(), std::to_string(12)) != statementMap["while"].end());

                  Assert::IsTrue(std::find(statementMap["call"].begin(), statementMap["call"].end(), std::to_string(14)) != statementMap["call"].end());
            
                  // Check the existence of values in variableMap
                  Assert::IsTrue(std::find(variableMap["x"].begin(), variableMap["x"].end(), std::to_string(1)) != variableMap["x"].end());
                  Assert::IsTrue(std::find(variableMap["x"].begin(), variableMap["x"].end(), std::to_string(4)) != variableMap["x"].end());
                  Assert::IsTrue(std::find(variableMap["x"].begin(), variableMap["x"].end(), std::to_string(5)) != variableMap["x"].end());
                  Assert::IsTrue(std::find(variableMap["x"].begin(), variableMap["x"].end(), std::to_string(10)) != variableMap["x"].end());

                  Assert::IsTrue(std::find(variableMap["y"].begin(), variableMap["y"].end(), std::to_string(2)) != variableMap["y"].end());
                  Assert::IsTrue(std::find(variableMap["y"].begin(), variableMap["y"].end(), std::to_string(8)) != variableMap["y"].end());

                  Assert::IsTrue(std::find(variableMap["z"].begin(), variableMap["z"].end(), std::to_string(3)) != variableMap["z"].end());
                  Assert::IsTrue(std::find(variableMap["z"].begin(), variableMap["z"].end(), std::to_string(6)) != variableMap["z"].end());
                  Assert::IsTrue(std::find(variableMap["z"].begin(), variableMap["z"].end(), std::to_string(7)) != variableMap["z"].end());
                  Assert::IsTrue(std::find(variableMap["z"].begin(), variableMap["z"].end(), std::to_string(9)) != variableMap["z"].end());

                  Assert::IsTrue(std::find(variableMap["a"].begin(), variableMap["a"].end(), std::to_string(11)) != variableMap["a"].end());
                  Assert::IsTrue(std::find(variableMap["a"].begin(), variableMap["a"].end(), std::to_string(12)) != variableMap["a"].end());
                  Assert::IsTrue(std::find(variableMap["a"].begin(), variableMap["a"].end(), std::to_string(13)) != variableMap["a"].end());

                  // Check the existence of values in constantMap
                  Assert::IsTrue(std::find(constantMap["10"].begin(), constantMap["10"].end(), std::to_string(3)) != constantMap["10"].end());
                  Assert::IsTrue(std::find(constantMap["10"].begin(), constantMap["10"].end(), std::to_string(6)) != constantMap["10"].end());
                  Assert::IsTrue(std::find(constantMap["10"].begin(), constantMap["10"].end(), std::to_string(11)) != constantMap["10"].end());
                  Assert::IsTrue(std::find(constantMap["10"].begin(), constantMap["10"].end(), std::to_string(12)) != constantMap["10"].end());

                  Assert::IsTrue(std::find(constantMap["20"].begin(), constantMap["20"].end(), std::to_string(3)) != constantMap["20"].end());
                  Assert::IsTrue(std::find(constantMap["20"].begin(), constantMap["20"].end(), std::to_string(7)) != constantMap["20"].end());

                  Assert::IsTrue(std::find(constantMap["30"].begin(), constantMap["30"].end(), std::to_string(9)) != constantMap["30"].end());

                  Assert::IsTrue(std::find(constantMap["40"].begin(), constantMap["40"].end(), std::to_string(9)) != constantMap["40"].end());

            }

            TEST_METHOD(TestUsesModifiesExtraction)
            {
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
                  std::vector<std::shared_ptr<StatementNode>> testStatementNodes = {testReadNode, testPrintNode, testAssignNode, testPrintNode2, testReadNode2, testIfNode, testAssignNode4, testWhileNode};

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

                  // Create UsesAbstraction and ModifiesAbstraction
                  testUsesAbstraction = UsesAbstraction();
                  std::shared_ptr<ModifiesAbstraction> testModifiesAbstraction = std::make_shared<ModifiesAbstraction>();

                  // Get shared pointer to procedure map
                  std::shared_ptr<std::map<std::string, std::vector<std::string>>> procedureMap = testProcedureEntity->getMap();

                  // Extract Uses and Modifies from procedure1
                  testUsesAbstraction->extractUsesAbstraction(testProgramNode);
                  testModifiesAbstraction->extractModifiesAbstraction(testProgramNode);
                  
                  // Get UsesMap and ModifiesMap from UsesAbstraction and ModifiesAbstraction by using getUsesStorageMap and getModifiesStorageMap
                  std::shared_ptr<map<string, vector<string>>> usesMap = testUsesAbstraction->getUsesStorageMap();
                  std::shared_ptr<map<string, vector<string>>> modifiesMap = testModifiesAbstraction->getModifiesStorageMap();

                  usesMap = testDesignExtractor.addProcedureNames(procedureMap, usesMap);
                  modifiesMap = testDesignExtractor.addProcedureNames(procedureMap, modifiesMap);

                  std::map<std::string, std::vector<std::string>>& usesMapRef = *usesMap;
                  std::map<std::string, std::vector<std::string>>& modifiesMapRef = *modifiesMap;

                  // Check the existence of values in usesMap
                  Assert::IsTrue(std::find(usesMapRef["x"].begin(), usesMapRef["x"].end(), std::to_string(1)) != usesMapRef["x"].end());
                  Assert::IsTrue(std::find(usesMapRef["x"].begin(), usesMapRef["x"].end(), std::to_string(4)) == usesMapRef["x"].end());
                  Assert::IsTrue(std::find(usesMapRef["x"].begin(), usesMapRef["x"].end(), std::to_string(5)) != usesMapRef["x"].end());
                  Assert::IsTrue(std::find(usesMapRef["x"].begin(), usesMapRef["x"].end(), std::to_string(6)) != usesMapRef["x"].end());
                  Assert::IsTrue(std::find(usesMapRef["x"].begin(), usesMapRef["x"].end(), std::to_string(10)) != usesMapRef["x"].end());
                  Assert::IsTrue(std::find(usesMapRef["x"].begin(), usesMapRef["x"].end(), "procedure1") != usesMapRef["x"].end());

                  Assert::IsTrue(std::find(usesMapRef["y"].begin(), usesMapRef["y"].end(), std::to_string(2)) != usesMapRef["y"].end());
                  Assert::IsTrue(std::find(usesMapRef["y"].begin(), usesMapRef["y"].end(), std::to_string(8)) == usesMapRef["y"].end());
                  Assert::IsTrue(std::find(usesMapRef["y"].begin(), usesMapRef["y"].end(), "procedure1") != usesMapRef["y"].end());

                  Assert::IsTrue(std::find(usesMapRef["z"].begin(), usesMapRef["z"].end(), std::to_string(3)) != usesMapRef["z"].end());
                  Assert::IsTrue(std::find(usesMapRef["z"].begin(), usesMapRef["z"].end(), std::to_string(6)) != usesMapRef["z"].end());
                  Assert::IsTrue(std::find(usesMapRef["z"].begin(), usesMapRef["z"].end(), std::to_string(7)) != usesMapRef["z"].end());
                  Assert::IsTrue(std::find(usesMapRef["z"].begin(), usesMapRef["z"].end(), std::to_string(9)) != usesMapRef["z"].end());
                  Assert::IsTrue(std::find(usesMapRef["z"].begin(), usesMapRef["z"].end(), "procedure1") != usesMapRef["z"].end());

                  Assert::IsTrue(std::find(usesMapRef["a"].begin(), usesMapRef["a"].end(), std::to_string(11)) != usesMapRef["a"].end());
                  Assert::IsTrue(std::find(usesMapRef["a"].begin(), usesMapRef["a"].end(), std::to_string(12)) != usesMapRef["a"].end());
                  Assert::IsTrue(std::find(usesMapRef["a"].begin(), usesMapRef["a"].end(), std::to_string(13)) != usesMapRef["a"].end());
                  Assert::IsTrue(std::find(usesMapRef["a"].begin(), usesMapRef["a"].end(), "procedure1") != usesMapRef["a"].end());

                  // Check the existence of values in modifiesMap
                  Assert::IsTrue(std::find(modifiesMapRef["x"].begin(), modifiesMapRef["x"].end(), std::to_string(1)) == modifiesMapRef["x"].end());
                  Assert::IsTrue(std::find(modifiesMapRef["x"].begin(), modifiesMapRef["x"].end(), std::to_string(4)) != modifiesMapRef["x"].end());
                  Assert::IsTrue(std::find(modifiesMapRef["x"].begin(), modifiesMapRef["x"].end(), std::to_string(5)) == modifiesMapRef["x"].end());
                  Assert::IsTrue(std::find(modifiesMapRef["x"].begin(), modifiesMapRef["x"].end(), std::to_string(6)) != modifiesMapRef["x"].end());
                  Assert::IsTrue(std::find(modifiesMapRef["x"].begin(), modifiesMapRef["x"].end(), std::to_string(10)) != modifiesMapRef["x"].end());
                  Assert::IsTrue(std::find(modifiesMapRef["x"].begin(), modifiesMapRef["x"].end(), "procedure1") != modifiesMapRef["x"].end());

                  Assert::IsTrue(std::find(modifiesMapRef["y"].begin(), modifiesMapRef["y"].end(), std::to_string(2)) == modifiesMapRef["y"].end());
                  Assert::IsTrue(std::find(modifiesMapRef["y"].begin(), modifiesMapRef["y"].end(), std::to_string(8)) != modifiesMapRef["y"].end());
                  Assert::IsTrue(std::find(modifiesMapRef["y"].begin(), modifiesMapRef["y"].end(), "procedure1") != modifiesMapRef["y"].end());

                  Assert::IsTrue(std::find(modifiesMapRef["z"].begin(), modifiesMapRef["z"].end(), std::to_string(3)) != modifiesMapRef["z"].end());
                  Assert::IsTrue(std::find(modifiesMapRef["z"].begin(), modifiesMapRef["z"].end(), std::to_string(6)) != modifiesMapRef["z"].end());
                  Assert::IsTrue(std::find(modifiesMapRef["z"].begin(), modifiesMapRef["z"].end(), std::to_string(7)) != modifiesMapRef["z"].end());
                  Assert::IsTrue(std::find(modifiesMapRef["z"].begin(), modifiesMapRef["z"].end(), std::to_string(9)) != modifiesMapRef["z"].end());
                  Assert::IsTrue(std::find(modifiesMapRef["z"].begin(), modifiesMapRef["z"].end(), "procedure1") != modifiesMapRef["z"].end());

                  Assert::IsTrue(std::find(modifiesMapRef["a"].begin(), modifiesMapRef["a"].end(), std::to_string(11)) != modifiesMapRef["a"].end());
                  Assert::IsTrue(std::find(modifiesMapRef["a"].begin(), modifiesMapRef["a"].end(), std::to_string(12)) != modifiesMapRef["a"].end());
                  Assert::IsTrue(std::find(modifiesMapRef["a"].begin(), modifiesMapRef["a"].end(), std::to_string(13)) != modifiesMapRef["a"].end());
                  Assert::IsTrue(std::find(modifiesMapRef["a"].begin(), modifiesMapRef["a"].end(), "procedure1") != modifiesMapRef["a"].end());



                 
            }


    };
      
}