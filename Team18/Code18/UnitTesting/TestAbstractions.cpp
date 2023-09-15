#include "stdafx.h"
#include "CppUnitTest.h"
#include <string>
#include "../source/DesignExtractor/ParentFollowsAbstraction.h"
#include "../source/DesignExtractor/UsesAbstraction.h"
#include "../source/DesignExtractor/ModifiesAbstraction.h"
#include "../source/AST/ASTNode.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(TestAbstraction)
	{
	public:
		TEST_METHOD(TestSimpleParentsFollowsAbstraction)
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

                  // Create ParentsFollowsAbstraction
                  std::shared_ptr<ParentFollowsAbstraction> testParentsFollowsAbstraction = std::make_shared<ParentFollowsAbstraction>();
                  
                  // Extract abstraction from program node
                  testParentsFollowsAbstraction->extractAbstraction(testProgramNode);

                  // Get abstraction map from abstraction entity
                  std::map<std::string, std::vector<string>> parentsMap = *(testParentsFollowsAbstraction->getParentsStorageMap());
                  std::map<std::string, std::vector<string>> followsMap = *(testParentsFollowsAbstraction->getFollowsStorageMap());
                  
                  // Check if the parentsMap is extracted correctly
                  Assert::AreEqual(parentsMap["1"].empty(), true);
                  Assert::AreEqual(parentsMap["2"].empty(), true);
                  Assert::AreEqual(parentsMap["3"].empty(), true);
                  Assert::AreEqual(parentsMap["4"].empty(), true);
                  Assert::AreEqual(parentsMap["5"].empty(), true);
                  Assert::AreEqual(parentsMap["6"].empty(), true);
                  Assert::AreEqual(parentsMap["11"].empty(), true);
                  Assert::AreEqual(parentsMap["12"].empty(), true);

                  Assert::AreEqual(parentsMap["7"][0], std::string("6"));
                  Assert::AreEqual(parentsMap["8"][0], std::string("6"));
                  Assert::AreEqual(parentsMap["9"][0], std::string("6"));
                  Assert::AreEqual(parentsMap["10"][0], std::string("6"));

                  Assert::AreEqual(parentsMap["13"][0], std::string("12"));
                  Assert::AreEqual(parentsMap["14"][0], std::string("12"));
                  
                  // Check if the followsMap is extracted correctly
                  Assert::AreEqual(followsMap["1"][0], std::string("2"));
                  Assert::AreEqual(followsMap["1"][1], std::string("3"));
                  Assert::AreEqual(followsMap["1"][2], std::string("4"));
                  Assert::AreEqual(followsMap["1"][3], std::string("5"));
                  Assert::AreEqual(followsMap["1"][4], std::string("6"));
                  Assert::AreEqual(followsMap["1"][5], std::string("11"));
                  Assert::AreEqual(followsMap["1"][6], std::string("12"));

                  Assert::AreEqual(followsMap["2"][0], std::string("3"));
                  Assert::AreEqual(followsMap["2"][1], std::string("4"));
                  Assert::AreEqual(followsMap["2"][2], std::string("5"));
                  Assert::AreEqual(followsMap["2"][3], std::string("6"));
                  Assert::AreEqual(followsMap["2"][4], std::string("11"));
                  Assert::AreEqual(followsMap["2"][5], std::string("12"));

                  Assert::AreEqual(followsMap["3"][0], std::string("4"));
                  Assert::AreEqual(followsMap["3"][1], std::string("5"));
                  Assert::AreEqual(followsMap["3"][2], std::string("6"));
                  Assert::AreEqual(followsMap["3"][3], std::string("11"));
                  Assert::AreEqual(followsMap["3"][4], std::string("12"));

                  Assert::AreEqual(followsMap["4"][0], std::string("5"));
                  Assert::AreEqual(followsMap["4"][1], std::string("6"));
                  Assert::AreEqual(followsMap["4"][2], std::string("11"));
                  Assert::AreEqual(followsMap["4"][3], std::string("12"));

                  Assert::AreEqual(followsMap["5"][0], std::string("6"));
                  Assert::AreEqual(followsMap["5"][1], std::string("11"));
                  Assert::AreEqual(followsMap["5"][2], std::string("12"));

                  Assert::AreEqual(followsMap["6"][0], std::string("11"));
                  Assert::AreEqual(followsMap["6"][1], std::string("12"));

                  Assert::AreEqual(followsMap["11"][0], std::string("12"));

                  Assert::AreEqual(followsMap["12"].empty(), true);

                  Assert::AreEqual(followsMap["7"][0], std::string("8"));
                  Assert::AreEqual(followsMap["7"][1], std::string("9"));
                  Assert::AreEqual(followsMap["7"][2], std::string("10"));

                  Assert::AreEqual(followsMap["8"][0], std::string("9"));
                  Assert::AreEqual(followsMap["8"][1], std::string("10"));

                  Assert::AreEqual(followsMap["9"][0], std::string("10"));

                  Assert::AreEqual(followsMap["10"].empty(), true);

		}

            TEST_METHOD(TestUsesExtraction)
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

                  // Create UsesAbstraction and ModifiesAbstraction
                  std::shared_ptr<UsesAbstraction> testUsesAbstraction = std::make_shared<UsesAbstraction>();

                  // Extract Uses and Modifies from procedure1
                  testUsesAbstraction->extractUsesAbstraction(testProgramNode);
                  
                  // Get UsesMap and ModifiesMap from UsesAbstraction and ModifiesAbstraction by using getUsesStorageMap and getModifiesStorageMap
                  std::shared_ptr<map<string, vector<string>>> usesMap = testUsesAbstraction->getUsesStorageMap();

                  std::map<std::string, std::vector<std::string>>& usesMapRef = *usesMap;

                  // // Check the existence of values in usesMap
                  Assert::IsTrue(std::find(usesMapRef["x"].begin(), usesMapRef["x"].end(), std::to_string(1)) == usesMapRef["x"].end());
                  Assert::IsTrue(std::find(usesMapRef["x"].begin(), usesMapRef["x"].end(), std::to_string(4)) != usesMapRef["x"].end());
                  Assert::IsTrue(std::find(usesMapRef["x"].begin(), usesMapRef["x"].end(), std::to_string(5)) == usesMapRef["x"].end());
                  Assert::IsTrue(std::find(usesMapRef["x"].begin(), usesMapRef["x"].end(), std::to_string(6)) != usesMapRef["x"].end());
                  Assert::IsTrue(std::find(usesMapRef["x"].begin(), usesMapRef["x"].end(), std::to_string(10)) != usesMapRef["x"].end());
                  // Assert::IsTrue(std::find(usesMapRef["x"].begin(), usesMapRef["x"].end(), "procedure1") != usesMapRef["x"].end());

                  Assert::IsTrue(std::find(usesMapRef["y"].begin(), usesMapRef["y"].end(), std::to_string(2)) != usesMapRef["y"].end());
                  Assert::IsTrue(std::find(usesMapRef["y"].begin(), usesMapRef["y"].end(), std::to_string(6)) == usesMapRef["y"].end());
                  Assert::IsTrue(std::find(usesMapRef["y"].begin(), usesMapRef["y"].end(), std::to_string(8)) == usesMapRef["y"].end());
                  // Assert::IsTrue(std::find(usesMapRef["y"].begin(), usesMapRef["y"].end(), "procedure1") != usesMapRef["y"].end());

                  Assert::IsTrue(std::find(usesMapRef["z"].begin(), usesMapRef["z"].end(), std::to_string(3)) != usesMapRef["z"].end());
                  Assert::IsTrue(std::find(usesMapRef["z"].begin(), usesMapRef["z"].end(), std::to_string(6)) != usesMapRef["z"].end());
                  Assert::IsTrue(std::find(usesMapRef["z"].begin(), usesMapRef["z"].end(), std::to_string(7)) != usesMapRef["z"].end());
                  Assert::IsTrue(std::find(usesMapRef["z"].begin(), usesMapRef["z"].end(), std::to_string(9)) != usesMapRef["z"].end());
                  // Assert::IsTrue(std::find(usesMapRef["z"].begin(), usesMapRef["z"].end(), "procedure1") != usesMapRef["z"].end());

                  Assert::IsTrue(std::find(usesMapRef["a"].begin(), usesMapRef["a"].end(), std::to_string(11)) != usesMapRef["a"].end());
                  Assert::IsTrue(std::find(usesMapRef["a"].begin(), usesMapRef["a"].end(), std::to_string(12)) != usesMapRef["a"].end());
                  Assert::IsTrue(std::find(usesMapRef["a"].begin(), usesMapRef["a"].end(), std::to_string(13)) != usesMapRef["a"].end());
                  // Assert::IsTrue(std::find(usesMapRef["a"].begin(), usesMapRef["a"].end(), "procedure1") != usesMapRef["a"].end());

                 
            }

            TEST_METHOD(TestModifiesExtraction)
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

                  // Create ModifiesAbstraction
                  std::shared_ptr<ModifiesAbstraction> testModifiesAbstraction = std::make_shared<ModifiesAbstraction>();

                  testModifiesAbstraction->extractModifiesAbstraction(testProgramNode);
                  std::shared_ptr<map<string, vector<string>>> modifiesMap = testModifiesAbstraction->getModifiesStorageMap();

                  std::map<std::string, std::vector<std::string>>& modifiesMapRef = *modifiesMap;

                  // // Check the existence of values in modifiesMap
                  Assert::IsTrue(std::find(modifiesMapRef["x"].begin(), modifiesMapRef["x"].end(), std::to_string(1)) != modifiesMapRef["x"].end());
                  Assert::IsTrue(std::find(modifiesMapRef["x"].begin(), modifiesMapRef["x"].end(), std::to_string(4)) == modifiesMapRef["x"].end());
                  Assert::IsTrue(std::find(modifiesMapRef["x"].begin(), modifiesMapRef["x"].end(), std::to_string(5)) != modifiesMapRef["x"].end());
                  Assert::IsTrue(std::find(modifiesMapRef["x"].begin(), modifiesMapRef["x"].end(), std::to_string(6)) == modifiesMapRef["x"].end());
                  Assert::IsTrue(std::find(modifiesMapRef["x"].begin(), modifiesMapRef["x"].end(), std::to_string(10)) == modifiesMapRef["x"].end());
                  // Assert::IsTrue(std::find(modifiesMapRef["x"].begin(), modifiesMapRef["x"].end(), "procedure1") != modifiesMapRef["x"].end());

                  Assert::IsTrue(std::find(modifiesMapRef["y"].begin(), modifiesMapRef["y"].end(), std::to_string(2)) == modifiesMapRef["y"].end());
                  Assert::IsTrue(std::find(modifiesMapRef["y"].begin(), modifiesMapRef["y"].end(), std::to_string(6)) != modifiesMapRef["y"].end());
                  Assert::IsTrue(std::find(modifiesMapRef["y"].begin(), modifiesMapRef["y"].end(), std::to_string(8)) != modifiesMapRef["y"].end());
                  // Assert::IsTrue(std::find(modifiesMapRef["y"].begin(), modifiesMapRef["y"].end(), "procedure1") != modifiesMapRef["y"].end());

                  Assert::IsTrue(std::find(modifiesMapRef["z"].begin(), modifiesMapRef["z"].end(), std::to_string(3)) != modifiesMapRef["z"].end());
                  Assert::IsTrue(std::find(modifiesMapRef["z"].begin(), modifiesMapRef["z"].end(), std::to_string(6)) != modifiesMapRef["z"].end());
                  Assert::IsTrue(std::find(modifiesMapRef["z"].begin(), modifiesMapRef["z"].end(), std::to_string(7)) != modifiesMapRef["z"].end());
                  Assert::IsTrue(std::find(modifiesMapRef["z"].begin(), modifiesMapRef["z"].end(), std::to_string(9)) != modifiesMapRef["z"].end());
                  // Assert::IsTrue(std::find(modifiesMapRef["z"].begin(), modifiesMapRef["z"].end(), "procedure1") != modifiesMapRef["z"].end());

                  Assert::IsTrue(std::find(modifiesMapRef["a"].begin(), modifiesMapRef["a"].end(), std::to_string(11)) != modifiesMapRef["a"].end());
                  Assert::IsTrue(std::find(modifiesMapRef["a"].begin(), modifiesMapRef["a"].end(), std::to_string(12)) == modifiesMapRef["a"].end());
                  Assert::IsTrue(std::find(modifiesMapRef["a"].begin(), modifiesMapRef["a"].end(), std::to_string(13)) == modifiesMapRef["a"].end());
                  // Assert::IsTrue(std::find(modifiesMapRef["a"].begin(), modifiesMapRef["a"].end(), "procedure1") != modifiesMapRef["a"].end());



                 
            }




    };
}