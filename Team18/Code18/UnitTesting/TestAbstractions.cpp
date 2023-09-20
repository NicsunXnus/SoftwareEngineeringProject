#include "stdafx.h"
#include "CppUnitTest.h"
#include <string>
#include "../source/DesignExtractor/ParentsAbstractionExtractor.h"
#include "../source/DesignExtractor/FollowsAbstractionExtractor.h"
#include "../source/DesignExtractor/UsesAbstractionExtractor.h"
#include "../source/DesignExtractor/ModifiesAbstractionExtractor.h"
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

                  // Create ParentsAbstractionExtractor and FollowsAbstractionExtractor
                  std::shared_ptr<ParentsAbstractionExtractor> testParentsAbstraction = std::make_shared<ParentsAbstractionExtractor>();
                  std::shared_ptr<FollowsAbstractionExtractor> testFollowsAbstraction = std::make_shared<FollowsAbstractionExtractor>();
                  
                  // Extract abstraction from program node
                  testParentsAbstraction->extractAbstractions(testProgramNode);
                  testFollowsAbstraction->extractAbstractions(testProgramNode);

                  // Get abstraction map from abstraction entity
                  std::map<std::string, std::vector<string>> parentsMap = *(testParentsAbstraction->getStorageMap());
                  std::map<std::string, std::vector<string>> followsMap = *(testFollowsAbstraction->getStorageMap());
                  
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

                  Assert::AreEqual(followsMap["7"][0], std::string("8"));

                  Assert::AreEqual(followsMap["8"].empty(), true);

                  Assert::AreEqual(followsMap["9"][0], std::string("10"));

                  Assert::AreEqual(followsMap["10"].empty(), true);
                  
                  Assert::AreEqual(followsMap["11"][0], std::string("12"));

                  Assert::AreEqual(followsMap["12"].empty(), true);

		}

            TEST_METHOD(TestSimpleUsesModifiesExtraction)
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
                        procedure procedure2 {
                              read b;
                              print b;
                              b = 3 + 4;
                              b = b + 5;
                              if (b == 12) then {
                                    b = 10 + 20;
                              } else {
                                    b = 30 + 40;
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
                  std::shared_ptr<ProcedureNode> testProcedureNode3 = std::make_shared<ProcedureNode>("procedure3", std::vector<std::shared_ptr<StatementNode>>());
                  std::shared_ptr<CallNode> testCallNode = std::make_shared<CallNode>(14, testProcedureNode3);
                  std::vector<std::shared_ptr<StatementNode>> whileStatements = {testPrintNode4, testCallNode};
                  std::shared_ptr<WhileNode> testWhileNode = std::make_shared<WhileNode>(12, testEqualNode2, whileStatements);

                  // Combine the above statement nodes
                  std::vector<std::shared_ptr<StatementNode>> testStatementNodes = {testReadNode, testPrintNode, testAssignNode, testPrintNode2, testReadNode2, testIfNode, testAssignNode4, testWhileNode};

                  // Create procedure node
                  std::shared_ptr<ProcedureNode> testProcedureNode1 = std::make_shared<ProcedureNode>("procedure1", testStatementNodes);

                  // Create ReadNode with VariableNode
                  std::shared_ptr<VariableNode> testVariableNode14 = std::make_shared<VariableNode>("b", 15);
                  std::shared_ptr<ReadNode> testReadNode4 = std::make_shared<ReadNode>(15, testVariableNode14);
                  
                  // Create PrintNode with VariableNode
                  std::shared_ptr<VariableNode> testVariableNode15 = std::make_shared<VariableNode>("b", 16);
                  std::shared_ptr<PrintNode> testPrintNode5 = std::make_shared<PrintNode>(16, testVariableNode15);

                  // Create AssignNode
                  std::shared_ptr<VariableNode> testVariableNode16 = std::make_shared<VariableNode>("b", 17);
                  std::shared_ptr<ConstantNode> testConstantNode9 = std::make_shared<ConstantNode>(3, 17);
                  std::shared_ptr<ConstantNode> testConstantNode10 = std::make_shared<ConstantNode>(4, 17);
                  std::shared_ptr<PlusNode> testPlusNode4 = std::make_shared<PlusNode>(testConstantNode9, testConstantNode10);
                  std::shared_ptr<AssignNode> testAssignNode5 = std::make_shared<AssignNode>(17, testVariableNode16, testPlusNode4);

                  // Create AssignNode
                  std::shared_ptr<VariableNode> testVariableNode17 = std::make_shared<VariableNode>("b", 18);
                  std::shared_ptr<VariableNode> testVariableNode18 = std::make_shared<VariableNode>("b", 18);
                  std::shared_ptr<ConstantNode> testConstantNode11 = std::make_shared<ConstantNode>(5, 18);
                  std::shared_ptr<PlusNode> testPlusNode5 = std::make_shared<PlusNode>(testVariableNode18, testConstantNode11);
                  std::shared_ptr<AssignNode> testAssignNode6 = std::make_shared<AssignNode>(18, testVariableNode17, testPlusNode5);

                  // Create ifNode
                  std::shared_ptr<VariableNode> testVariableNode19 = std::make_shared<VariableNode>("b", 19);
                  std::shared_ptr<ConstantNode> testConstantNode12 = std::make_shared<ConstantNode>(12, 19);
                  std::shared_ptr<EqualsNode> testEqualNode3 = std::make_shared<EqualsNode>(testVariableNode19, testConstantNode12);
                  std::shared_ptr<VariableNode> testVariableNode20 = std::make_shared<VariableNode>("b", 20);
                  std::shared_ptr<ConstantNode> testConstantNode13 = std::make_shared<ConstantNode>(10, 20);
                  std::shared_ptr<ConstantNode> testConstantNode17 = std::make_shared<ConstantNode>(20, 20);
                  std::shared_ptr<PlusNode> testPlusNode6 = std::make_shared<PlusNode>(testConstantNode13, testConstantNode17);
                  std::shared_ptr<AssignNode> testAssignNode7 = std::make_shared<AssignNode>(20, testVariableNode20, testPlusNode6);
                  std::shared_ptr<VariableNode> testVariableNode21 = std::make_shared<VariableNode>("b", 21);
                  std::shared_ptr<ConstantNode> testConstantNode15 = std::make_shared<ConstantNode>(30, 21);
                  std::shared_ptr<ConstantNode> testConstantNode16 = std::make_shared<ConstantNode>(40, 21);
                  std::shared_ptr<PlusNode> testPlusNode7 = std::make_shared<PlusNode>(testConstantNode15, testConstantNode16);
                  std::shared_ptr<AssignNode> testAssignNode8 = std::make_shared<AssignNode>(21, testVariableNode21, testPlusNode7);
                  std::vector<std::shared_ptr<StatementNode>> ifStatements2 = {testAssignNode7};
                  std::vector<std::shared_ptr<StatementNode>> elseStatements2 = {testAssignNode8};
                  std::shared_ptr<IfNode> testIfNode2 = std::make_shared<IfNode>(19, testEqualNode3, ifStatements2, elseStatements2);

                  // Combine the above statement nodes
                  std::vector<std::shared_ptr<StatementNode>> testStatementNodes2 = {testReadNode4, testPrintNode5, testAssignNode5, testAssignNode6, testIfNode2};

                  // Create procedure node
                  std::shared_ptr<ProcedureNode> testProcedureNode2 = std::make_shared<ProcedureNode>("procedure2", testStatementNodes2);

                  std::vector<std::shared_ptr<ProcedureNode>> testProcedureNodes = {testProcedureNode1, testProcedureNode2};

                  // Create program node
                  std::shared_ptr<ProgramNode> testProgramNode = std::make_shared<ProgramNode>(testProcedureNodes);

                  // Create UsesAbstraction and ModifiesAbstraction
                  std::shared_ptr<UsesAbstractionExtractor> testUsesAbstraction = std::make_shared<UsesAbstractionExtractor>();
                  std::shared_ptr<ModifiesAbstractionExtractor> testModifiesAbstraction = std::make_shared<ModifiesAbstractionExtractor>();

                  // Extract abstraction from program node
                  testUsesAbstraction->extractAbstractions(testProgramNode);
                  testModifiesAbstraction->extractAbstractions(testProgramNode);

                  // Get abstraction map s
                  std::shared_ptr<map<string, vector<string>>> usesMap = testUsesAbstraction->getStorageMap();
                  std::shared_ptr<map<string, vector<string>>> modifiesMap = testModifiesAbstraction->getStorageMap();

                  std::map<std::string, std::vector<std::string>>& usesMapRef = *usesMap;
                  std::map<std::string, std::vector<std::string>>& modifiesMapRef = *modifiesMap;

                  // // Check the existence of values in usesMap
                  Assert::IsTrue(std::find(usesMapRef["x"].begin(), usesMapRef["x"].end(), std::to_string(1)) == usesMapRef["x"].end());
                  Assert::IsTrue(std::find(usesMapRef["x"].begin(), usesMapRef["x"].end(), std::to_string(4)) != usesMapRef["x"].end());
                  Assert::IsTrue(std::find(usesMapRef["x"].begin(), usesMapRef["x"].end(), std::to_string(5)) == usesMapRef["x"].end());
                  Assert::IsTrue(std::find(usesMapRef["x"].begin(), usesMapRef["x"].end(), std::to_string(6)) != usesMapRef["x"].end());
                  Assert::IsTrue(std::find(usesMapRef["x"].begin(), usesMapRef["x"].end(), std::to_string(10)) != usesMapRef["x"].end());
                  Assert::IsTrue(std::find(usesMapRef["x"].begin(), usesMapRef["x"].end(), "procedure1") != usesMapRef["x"].end());

                  Assert::IsTrue(std::find(usesMapRef["y"].begin(), usesMapRef["y"].end(), std::to_string(2)) != usesMapRef["y"].end());
                  Assert::IsTrue(std::find(usesMapRef["y"].begin(), usesMapRef["y"].end(), std::to_string(6)) == usesMapRef["y"].end());
                  Assert::IsTrue(std::find(usesMapRef["y"].begin(), usesMapRef["y"].end(), std::to_string(8)) == usesMapRef["y"].end());
                  Assert::IsTrue(std::find(usesMapRef["y"].begin(), usesMapRef["y"].end(), "procedure1") != usesMapRef["y"].end());

                  Assert::IsTrue(std::find(usesMapRef["z"].begin(), usesMapRef["z"].end(), std::to_string(3)) == usesMapRef["z"].end());
                  Assert::IsTrue(std::find(usesMapRef["z"].begin(), usesMapRef["z"].end(), std::to_string(6)) != usesMapRef["z"].end());
                  Assert::IsTrue(std::find(usesMapRef["z"].begin(), usesMapRef["z"].end(), std::to_string(7)) == usesMapRef["z"].end());
                  Assert::IsTrue(std::find(usesMapRef["z"].begin(), usesMapRef["z"].end(), std::to_string(9)) == usesMapRef["z"].end());
                  Assert::IsTrue(std::find(usesMapRef["z"].begin(), usesMapRef["z"].end(), "procedure1") != usesMapRef["z"].end());

                  Assert::IsTrue(std::find(usesMapRef["a"].begin(), usesMapRef["a"].end(), std::to_string(11)) == usesMapRef["a"].end());
                  Assert::IsTrue(std::find(usesMapRef["a"].begin(), usesMapRef["a"].end(), std::to_string(12)) != usesMapRef["a"].end());
                  Assert::IsTrue(std::find(usesMapRef["a"].begin(), usesMapRef["a"].end(), std::to_string(13)) != usesMapRef["a"].end());
                  Assert::IsTrue(std::find(usesMapRef["a"].begin(), usesMapRef["a"].end(), "procedure1") != usesMapRef["a"].end());

                  Assert::IsTrue(std::find(usesMapRef["b"].begin(), usesMapRef["b"].end(), std::to_string(15)) == usesMapRef["b"].end());
                  Assert::IsTrue(std::find(usesMapRef["b"].begin(), usesMapRef["b"].end(), std::to_string(16)) != usesMapRef["b"].end());
                  Assert::IsTrue(std::find(usesMapRef["b"].begin(), usesMapRef["b"].end(), std::to_string(17)) == usesMapRef["b"].end());
                  Assert::IsTrue(std::find(usesMapRef["b"].begin(), usesMapRef["b"].end(), std::to_string(18)) != usesMapRef["b"].end());
                  Assert::IsTrue(std::find(usesMapRef["b"].begin(), usesMapRef["b"].end(), std::to_string(19)) != usesMapRef["b"].end());
                  Assert::IsTrue(std::find(usesMapRef["b"].begin(), usesMapRef["b"].end(), std::to_string(20)) == usesMapRef["b"].end());
                  Assert::IsTrue(std::find(usesMapRef["b"].begin(), usesMapRef["b"].end(), std::to_string(21)) == usesMapRef["b"].end());
                  Assert::IsTrue(std::find(usesMapRef["b"].begin(), usesMapRef["b"].end(), "procedure2") != usesMapRef["b"].end());

                  // Check the existence of values in modifiesMap
                  Assert::IsTrue(std::find(modifiesMapRef["x"].begin(), modifiesMapRef["x"].end(), std::to_string(1)) != modifiesMapRef["x"].end());
                  Assert::IsTrue(std::find(modifiesMapRef["x"].begin(), modifiesMapRef["x"].end(), std::to_string(4)) == modifiesMapRef["x"].end());
                  Assert::IsTrue(std::find(modifiesMapRef["x"].begin(), modifiesMapRef["x"].end(), std::to_string(5)) != modifiesMapRef["x"].end());
                  Assert::IsTrue(std::find(modifiesMapRef["x"].begin(), modifiesMapRef["x"].end(), std::to_string(6)) == modifiesMapRef["x"].end());
                  Assert::IsTrue(std::find(modifiesMapRef["x"].begin(), modifiesMapRef["x"].end(), std::to_string(10)) == modifiesMapRef["x"].end());
                  Assert::IsTrue(std::find(modifiesMapRef["x"].begin(), modifiesMapRef["x"].end(), "procedure1") != modifiesMapRef["x"].end());

                  Assert::IsTrue(std::find(modifiesMapRef["y"].begin(), modifiesMapRef["y"].end(), std::to_string(2)) == modifiesMapRef["y"].end());
                  Assert::IsTrue(std::find(modifiesMapRef["y"].begin(), modifiesMapRef["y"].end(), std::to_string(6)) != modifiesMapRef["y"].end());
                  Assert::IsTrue(std::find(modifiesMapRef["y"].begin(), modifiesMapRef["y"].end(), std::to_string(8)) != modifiesMapRef["y"].end());
                  Assert::IsTrue(std::find(modifiesMapRef["y"].begin(), modifiesMapRef["y"].end(), "procedure1") != modifiesMapRef["y"].end());

                  Assert::IsTrue(std::find(modifiesMapRef["z"].begin(), modifiesMapRef["z"].end(), std::to_string(3)) != modifiesMapRef["z"].end());
                  Assert::IsTrue(std::find(modifiesMapRef["z"].begin(), modifiesMapRef["z"].end(), std::to_string(6)) != modifiesMapRef["z"].end());
                  Assert::IsTrue(std::find(modifiesMapRef["z"].begin(), modifiesMapRef["z"].end(), std::to_string(7)) != modifiesMapRef["z"].end());
                  Assert::IsTrue(std::find(modifiesMapRef["z"].begin(), modifiesMapRef["z"].end(), std::to_string(9)) != modifiesMapRef["z"].end());
                  Assert::IsTrue(std::find(modifiesMapRef["z"].begin(), modifiesMapRef["z"].end(), "procedure1") != modifiesMapRef["z"].end());

                  Assert::IsTrue(std::find(modifiesMapRef["a"].begin(), modifiesMapRef["a"].end(), std::to_string(11)) != modifiesMapRef["a"].end());
                  Assert::IsTrue(std::find(modifiesMapRef["a"].begin(), modifiesMapRef["a"].end(), std::to_string(12)) == modifiesMapRef["a"].end());
                  Assert::IsTrue(std::find(modifiesMapRef["a"].begin(), modifiesMapRef["a"].end(), std::to_string(13)) == modifiesMapRef["a"].end());
                  Assert::IsTrue(std::find(modifiesMapRef["a"].begin(), modifiesMapRef["a"].end(), "procedure1") != modifiesMapRef["a"].end());

                  Assert::IsTrue(std::find(modifiesMapRef["b"].begin(), modifiesMapRef["b"].end(), std::to_string(15)) != modifiesMapRef["b"].end());
                  Assert::IsTrue(std::find(modifiesMapRef["b"].begin(), modifiesMapRef["b"].end(), std::to_string(16)) == modifiesMapRef["b"].end());
                  Assert::IsTrue(std::find(modifiesMapRef["b"].begin(), modifiesMapRef["b"].end(), std::to_string(17)) != modifiesMapRef["b"].end());
                  Assert::IsTrue(std::find(modifiesMapRef["b"].begin(), modifiesMapRef["b"].end(), std::to_string(18)) != modifiesMapRef["b"].end());
                  Assert::IsTrue(std::find(modifiesMapRef["b"].begin(), modifiesMapRef["b"].end(), std::to_string(19)) != modifiesMapRef["b"].end());
                  Assert::IsTrue(std::find(modifiesMapRef["b"].begin(), modifiesMapRef["b"].end(), std::to_string(20)) != modifiesMapRef["b"].end());
                  Assert::IsTrue(std::find(modifiesMapRef["b"].begin(), modifiesMapRef["b"].end(), std::to_string(21)) != modifiesMapRef["b"].end());
                  Assert::IsTrue(std::find(modifiesMapRef["b"].begin(), modifiesMapRef["b"].end(), "procedure2") != modifiesMapRef["b"].end());  

                 
            }

            TEST_METHOD(TestNestedAbstractionsExtraction)
            {
                  /* The code in mind
                        procedure testprocedure {
                              x = 10;
                              if (x == 10) then {
                                    x = 20;
                                    while(y == 10) {
                                          y = 20;
                                          if (z == 20) then {
                                                z = 30;
                                          } else {
                                                z = 40;
                                          }
                                    }
                              } else {
                                    x = 50;
                              }
                        }
                  */
                  // Create statement nodes
                  std::shared_ptr<VariableNode> testVariableNode = std::make_shared<VariableNode>("x", 1);
                  std::shared_ptr<ConstantNode> testConstantNode = std::make_shared<ConstantNode>(10, 1);
                  std::shared_ptr<AssignNode> testAssignNode = std::make_shared<AssignNode>(1, testVariableNode, testConstantNode);

                  std::shared_ptr<VariableNode> testVariableNode2 = std::make_shared<VariableNode>("x", 2);
                  std::shared_ptr<ConstantNode> testConstantNode2 = std::make_shared<ConstantNode>(10, 2);
                  std::shared_ptr<EqualsNode> testEqualNode = std::make_shared<EqualsNode>(testVariableNode2, testConstantNode2);

                  std::shared_ptr<VariableNode> testVariableNode3 = std::make_shared<VariableNode>("x", 3);
                  std::shared_ptr<ConstantNode> testConstantNode3 = std::make_shared<ConstantNode>(20, 3);
                  std::shared_ptr<AssignNode> testAssignNode2 = std::make_shared<AssignNode>(3, testVariableNode3, testConstantNode3);

                  std::shared_ptr<VariableNode> testVariableNode4 = std::make_shared<VariableNode>("y", 4);
                  std::shared_ptr<ConstantNode> testConstantNode4 = std::make_shared<ConstantNode>(10, 4);
                  std::shared_ptr<EqualsNode> testEqualNode2 = std::make_shared<EqualsNode>(testVariableNode4, testConstantNode4);

                  std::shared_ptr<VariableNode> testVariableNode5 = std::make_shared<VariableNode>("y", 5);
                  std::shared_ptr<ConstantNode> testConstantNode5 = std::make_shared<ConstantNode>(20, 5);
                  std::shared_ptr<AssignNode> testAssignNode3 = std::make_shared<AssignNode>(5, testVariableNode5, testConstantNode5);

                  std::shared_ptr<VariableNode> testVariableNode6 = std::make_shared<VariableNode>("z", 6);
                  std::shared_ptr<ConstantNode> testConstantNode6 = std::make_shared<ConstantNode>(20, 6);
                  std::shared_ptr<EqualsNode> testEqualNode3 = std::make_shared<EqualsNode>(testVariableNode6, testConstantNode6);

                  std::shared_ptr<VariableNode> testVariableNode7 = std::make_shared<VariableNode>("z", 7);
                  std::shared_ptr<ConstantNode> testConstantNode7 = std::make_shared<ConstantNode>(30, 7);
                  std::shared_ptr<AssignNode> testAssignNode4 = std::make_shared<AssignNode>(7, testVariableNode7, testConstantNode7);

                  std::shared_ptr<VariableNode> testVariableNode8 = std::make_shared<VariableNode>("z", 8);
                  std::shared_ptr<ConstantNode> testConstantNode8 = std::make_shared<ConstantNode>(40, 8);
                  std::shared_ptr<AssignNode> testAssignNode5 = std::make_shared<AssignNode>(8, testVariableNode8, testConstantNode8);

                  std::vector<std::shared_ptr<StatementNode>> ifStatements = {testAssignNode4};
                  std::vector<std::shared_ptr<StatementNode>> elseStatements = {testAssignNode5};
                  std::shared_ptr<IfNode> testIfNode = std::make_shared<IfNode>(6, testEqualNode3, ifStatements, elseStatements);

                  std::vector<std::shared_ptr<StatementNode>> whileStatements = {testAssignNode3, testIfNode};
                  std::shared_ptr<WhileNode> testWhileNode = std::make_shared<WhileNode>(4, testEqualNode2, whileStatements);

                  std::shared_ptr<VariableNode> testVariableNode9 = std::make_shared<VariableNode>("x", 9);
                  std::shared_ptr<ConstantNode> testConstantNode9 = std::make_shared<ConstantNode>(50, 9);
                  std::shared_ptr<AssignNode> testAssignNode6 = std::make_shared<AssignNode>(9, testVariableNode, testConstantNode);

                  std::vector<std::shared_ptr<StatementNode>> ifStatements2 = {testAssignNode2, testWhileNode};
                  std::vector<std::shared_ptr<StatementNode>> elseStatements2 = {testAssignNode6};
                  std::shared_ptr<IfNode> testIfNode2 = std::make_shared<IfNode>(2, testEqualNode, ifStatements2, elseStatements2);

                  // Combine the above statement nodes
                  std::vector<std::shared_ptr<StatementNode>> testStatementNodes = {testAssignNode, testIfNode2};

                  // Create procedure node
                  std::shared_ptr<ProcedureNode> testProcedureNode = std::make_shared<ProcedureNode>("testprocedure", testStatementNodes);

                  // Create program node
                  std::shared_ptr<ProgramNode> testProgramNode = std::make_shared<ProgramNode>(std::vector<std::shared_ptr<ProcedureNode>>{testProcedureNode});

                  // Create Abstraction Extractors
                  std::shared_ptr<UsesAbstractionExtractor> testUsesAbstraction = std::make_shared<UsesAbstractionExtractor>();
                  std::shared_ptr<ModifiesAbstractionExtractor> testModifiesAbstraction = std::make_shared<ModifiesAbstractionExtractor>();
                  std::shared_ptr<ParentsAbstractionExtractor> testParentsAbstraction = std::make_shared<ParentsAbstractionExtractor>();
                  std::shared_ptr<FollowsAbstractionExtractor> testFollowsAbstraction = std::make_shared<FollowsAbstractionExtractor>();

                  // Extract abstraction from program node
                  testUsesAbstraction->extractAbstractions(testProgramNode);
                  testModifiesAbstraction->extractAbstractions(testProgramNode);
                  testParentsAbstraction->extractAbstractions(testProgramNode);
                  testFollowsAbstraction->extractAbstractions(testProgramNode);

                  // Get abstraction maps
                  std::shared_ptr<map<string, vector<string>>> usesMap = testUsesAbstraction->getStorageMap();
                  std::shared_ptr<map<string, vector<string>>> modifiesMap = testModifiesAbstraction->getStorageMap();
                  std::shared_ptr<map<string, vector<string>>> parentsMap = testParentsAbstraction->getStorageMap();
                  std::shared_ptr<map<string, vector<string>>> followsMap = testFollowsAbstraction->getStorageMap();

                  std::map<std::string, std::vector<std::string>>& usesMapRef = *usesMap;
                  std::map<std::string, std::vector<std::string>>& modifiesMapRef = *modifiesMap;
                  std::map<std::string, std::vector<std::string>>& parentsMapRef = *parentsMap;
                  std::map<std::string, std::vector<std::string>>& followsMapRef = *followsMap;

                  // Check the existence of values in usesMap
                  Assert::IsTrue(std::find(usesMapRef["x"].begin(), usesMapRef["x"].end(), std::to_string(1)) == usesMapRef["x"].end());
                  Assert::IsTrue(std::find(usesMapRef["x"].begin(), usesMapRef["x"].end(), std::to_string(2)) != usesMapRef["x"].end());
                  Assert::IsTrue(std::find(usesMapRef["x"].begin(), usesMapRef["x"].end(), std::to_string(3)) == usesMapRef["x"].end());
                  Assert::IsTrue(std::find(usesMapRef["x"].begin(), usesMapRef["x"].end(), std::to_string(4)) == usesMapRef["x"].end());
                  Assert::IsTrue(std::find(usesMapRef["x"].begin(), usesMapRef["x"].end(), std::to_string(5)) == usesMapRef["x"].end());
                  Assert::IsTrue(std::find(usesMapRef["x"].begin(), usesMapRef["x"].end(), std::to_string(6)) == usesMapRef["x"].end());
                  Assert::IsTrue(std::find(usesMapRef["x"].begin(), usesMapRef["x"].end(), std::to_string(7)) == usesMapRef["x"].end());
                  Assert::IsTrue(std::find(usesMapRef["x"].begin(), usesMapRef["x"].end(), std::to_string(8)) == usesMapRef["x"].end());
                  Assert::IsTrue(std::find(usesMapRef["x"].begin(), usesMapRef["x"].end(), std::to_string(9)) == usesMapRef["x"].end());
                  // Assert::IsTrue(std::find(usesMapRef["x"].begin(), usesMapRef["x"].end(), "testprocedure") != usesMapRef["x"].end());

                  Assert::IsTrue(std::find(usesMapRef["y"].begin(), usesMapRef["y"].end(), std::to_string(1)) == usesMapRef["y"].end());
                  Assert::IsTrue(std::find(usesMapRef["y"].begin(), usesMapRef["y"].end(), std::to_string(2)) != usesMapRef["y"].end());
                  Assert::IsTrue(std::find(usesMapRef["y"].begin(), usesMapRef["y"].end(), std::to_string(3)) == usesMapRef["y"].end());
                  Assert::IsTrue(std::find(usesMapRef["y"].begin(), usesMapRef["y"].end(), std::to_string(4)) != usesMapRef["y"].end());
                  Assert::IsTrue(std::find(usesMapRef["y"].begin(), usesMapRef["y"].end(), std::to_string(5)) == usesMapRef["y"].end());
                  Assert::IsTrue(std::find(usesMapRef["y"].begin(), usesMapRef["y"].end(), std::to_string(6)) == usesMapRef["y"].end());
                  Assert::IsTrue(std::find(usesMapRef["y"].begin(), usesMapRef["y"].end(), std::to_string(7)) == usesMapRef["y"].end());
                  Assert::IsTrue(std::find(usesMapRef["y"].begin(), usesMapRef["y"].end(), std::to_string(8)) == usesMapRef["y"].end());
                  Assert::IsTrue(std::find(usesMapRef["y"].begin(), usesMapRef["y"].end(), std::to_string(9)) == usesMapRef["y"].end());
                  // Assert::IsTrue(std::find(usesMapRef["y"].begin(), usesMapRef["y"].end(), "testprocedure") != usesMapRef["y"].end());

                  Assert::IsTrue(std::find(usesMapRef["z"].begin(), usesMapRef["z"].end(), std::to_string(1)) == usesMapRef["z"].end());
                  Assert::IsTrue(std::find(usesMapRef["z"].begin(), usesMapRef["z"].end(), std::to_string(2)) != usesMapRef["z"].end());
                  Assert::IsTrue(std::find(usesMapRef["z"].begin(), usesMapRef["z"].end(), std::to_string(3)) == usesMapRef["z"].end());
                  Assert::IsTrue(std::find(usesMapRef["z"].begin(), usesMapRef["z"].end(), std::to_string(4)) != usesMapRef["z"].end());
                  Assert::IsTrue(std::find(usesMapRef["z"].begin(), usesMapRef["z"].end(), std::to_string(5)) == usesMapRef["z"].end());
                  Assert::IsTrue(std::find(usesMapRef["z"].begin(), usesMapRef["z"].end(), std::to_string(6)) != usesMapRef["z"].end());
                  Assert::IsTrue(std::find(usesMapRef["z"].begin(), usesMapRef["z"].end(), std::to_string(7)) == usesMapRef["z"].end());
                  Assert::IsTrue(std::find(usesMapRef["z"].begin(), usesMapRef["z"].end(), std::to_string(8)) == usesMapRef["z"].end());
                  Assert::IsTrue(std::find(usesMapRef["z"].begin(), usesMapRef["z"].end(), std::to_string(9)) == usesMapRef["z"].end());
                  // Assert::IsTrue(std::find(usesMapRef["z"].begin(), usesMapRef["z"].end(), "testprocedure") != usesMapRef["z"].end());

                  // Check the existence of values in modifiesMap
                  Assert::IsTrue(std::find(modifiesMapRef["x"].begin(), modifiesMapRef["x"].end(), std::to_string(1)) != modifiesMapRef["x"].end());
                  Assert::IsTrue(std::find(modifiesMapRef["x"].begin(), modifiesMapRef["x"].end(), std::to_string(2)) != modifiesMapRef["x"].end());
                  Assert::IsTrue(std::find(modifiesMapRef["x"].begin(), modifiesMapRef["x"].end(), std::to_string(3)) != modifiesMapRef["x"].end());
                  Assert::IsTrue(std::find(modifiesMapRef["x"].begin(), modifiesMapRef["x"].end(), std::to_string(4)) == modifiesMapRef["x"].end());
                  Assert::IsTrue(std::find(modifiesMapRef["x"].begin(), modifiesMapRef["x"].end(), std::to_string(5)) == modifiesMapRef["x"].end());
                  Assert::IsTrue(std::find(modifiesMapRef["x"].begin(), modifiesMapRef["x"].end(), std::to_string(6)) == modifiesMapRef["x"].end());
                  Assert::IsTrue(std::find(modifiesMapRef["x"].begin(), modifiesMapRef["x"].end(), std::to_string(7)) == modifiesMapRef["x"].end());
                  Assert::IsTrue(std::find(modifiesMapRef["x"].begin(), modifiesMapRef["x"].end(), std::to_string(8)) == modifiesMapRef["x"].end());
                  Assert::IsTrue(std::find(modifiesMapRef["x"].begin(), modifiesMapRef["x"].end(), std::to_string(9)) == modifiesMapRef["x"].end());
                  // Assert::IsTrue(std::find(modifiesMapRef["x"].begin(), modifiesMapRef["x"].end(), "testprocedure") != modifiesMapRef["x"].end());

                  Assert::IsTrue(std::find(modifiesMapRef["y"].begin(), modifiesMapRef["y"].end(), std::to_string(1)) == modifiesMapRef["y"].end());
                  Assert::IsTrue(std::find(modifiesMapRef["y"].begin(), modifiesMapRef["y"].end(), std::to_string(2)) != modifiesMapRef["y"].end());
                  Assert::IsTrue(std::find(modifiesMapRef["y"].begin(), modifiesMapRef["y"].end(), std::to_string(3)) == modifiesMapRef["y"].end());
                  Assert::IsTrue(std::find(modifiesMapRef["y"].begin(), modifiesMapRef["y"].end(), std::to_string(4)) != modifiesMapRef["y"].end());
                  Assert::IsTrue(std::find(modifiesMapRef["y"].begin(), modifiesMapRef["y"].end(), std::to_string(5)) != modifiesMapRef["y"].end());
                  Assert::IsTrue(std::find(modifiesMapRef["y"].begin(), modifiesMapRef["y"].end(), std::to_string(6)) == modifiesMapRef["y"].end());
                  Assert::IsTrue(std::find(modifiesMapRef["y"].begin(), modifiesMapRef["y"].end(), std::to_string(7)) == modifiesMapRef["y"].end());
                  Assert::IsTrue(std::find(modifiesMapRef["y"].begin(), modifiesMapRef["y"].end(), std::to_string(8)) == modifiesMapRef["y"].end());
                  Assert::IsTrue(std::find(modifiesMapRef["y"].begin(), modifiesMapRef["y"].end(), std::to_string(9)) == modifiesMapRef["y"].end());
                  // Assert::IsTrue(std::find(modifiesMapRef["y"].begin(), modifiesMapRef["y"].end(), "testprocedure") != modifiesMapRef["y"].end());

                  Assert::IsTrue(std::find(modifiesMapRef["z"].begin(), modifiesMapRef["z"].end(), std::to_string(1)) == modifiesMapRef["z"].end());
                  Assert::IsTrue(std::find(modifiesMapRef["z"].begin(), modifiesMapRef["z"].end(), std::to_string(2)) != modifiesMapRef["z"].end());
                  Assert::IsTrue(std::find(modifiesMapRef["z"].begin(), modifiesMapRef["z"].end(), std::to_string(3)) == modifiesMapRef["z"].end());
                  Assert::IsTrue(std::find(modifiesMapRef["z"].begin(), modifiesMapRef["z"].end(), std::to_string(4)) != modifiesMapRef["z"].end());
                  Assert::IsTrue(std::find(modifiesMapRef["z"].begin(), modifiesMapRef["z"].end(), std::to_string(5)) == modifiesMapRef["z"].end());
                  Assert::IsTrue(std::find(modifiesMapRef["z"].begin(), modifiesMapRef["z"].end(), std::to_string(6)) != modifiesMapRef["z"].end());
                  Assert::IsTrue(std::find(modifiesMapRef["z"].begin(), modifiesMapRef["z"].end(), std::to_string(7)) != modifiesMapRef["z"].end());
                  Assert::IsTrue(std::find(modifiesMapRef["z"].begin(), modifiesMapRef["z"].end(), std::to_string(8)) != modifiesMapRef["z"].end());
                  Assert::IsTrue(std::find(modifiesMapRef["z"].begin(), modifiesMapRef["z"].end(), std::to_string(9)) == modifiesMapRef["z"].end());
                  // Assert::IsTrue(std::find(modifiesMapRef["z"].begin(), modifiesMapRef["z"].end(), "testprocedure") != modifiesMapRef["z"].end());

                  // Check the existence of values in parentsMap
                  Assert::IsTrue(parentsMapRef["1"].empty());
                  Assert::IsTrue(parentsMapRef["2"].empty());
                  
                  Assert::AreEqual(parentsMapRef["3"][0], std::string("2"));
                  
                  Assert::AreEqual(parentsMapRef["4"][0], std::string("2"));
                  
                  Assert::AreEqual(parentsMapRef["5"][0], std::string("4"));
                  Assert::AreEqual(parentsMapRef["5"][1], std::string("2"));
                  
                  Assert::AreEqual(parentsMapRef["6"][0], std::string("4"));
                  Assert::AreEqual(parentsMapRef["6"][1], std::string("2"));

                  Assert::AreEqual(parentsMapRef["7"][0], std::string("6"));
                  Assert::AreEqual(parentsMapRef["7"][1], std::string("4"));
                  Assert::AreEqual(parentsMapRef["7"][2], std::string("2"));

                  Assert::AreEqual(parentsMapRef["8"][0], std::string("6"));
                  Assert::AreEqual(parentsMapRef["8"][1], std::string("4"));
                  Assert::AreEqual(parentsMapRef["8"][2], std::string("2"));

                  Assert::AreEqual(parentsMapRef["9"][0], std::string("2"));

                  // Check the existence of values in followsMap
                  Assert::AreEqual(followsMapRef["1"][0], std::string("2"));
                  
                  Assert::IsTrue(followsMapRef["2"].empty());

                  Assert::AreEqual(followsMapRef["3"][0], std::string("4"));
                  
                  Assert::IsTrue(followsMapRef["4"].empty());

                  Assert::AreEqual(followsMapRef["5"][0], std::string("6"));

                  Assert::IsTrue(followsMapRef["6"].empty());

                  Assert::IsTrue(followsMapRef["7"].empty());

                  Assert::IsTrue(followsMapRef["8"].empty());

                  Assert::IsTrue(followsMapRef["9"].empty());







                  


                  



  
            }




    };
}