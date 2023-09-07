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
		TEST_METHOD(TestStatement)
		{

		}

	};
}