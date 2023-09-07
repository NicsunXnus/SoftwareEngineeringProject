#include "stdafx.h"
#include "CppUnitTest.h"
#include "../source/DesignExtractor/Entity.h"
#include "../source/AST/ASTNode.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(TestSP)
	{
	public:
		TEST_METHOD(TestProcedureEntity)
		{
            //Create procedure nodes
            std::shared_ptr<ProcedureNode> testProcdeureNode1 = std::make_shared<ProcedureNode>("procedure1")
			std::shared_ptr<ProcedureNode> testProcdeureNode2 = std::make_shared<ProcedureNode>("procedure2")
            std::shared_ptr<ProcedureNode> testProcdeureNode3 = std::make_shared<ProcedureNode>("procedure3")
            std::shared_ptr<ProcedureNode> testProcdeureNode4 = std::make_shared<ProcedureNode>("procedure4")
            std::shared_ptr<ProcedureNode> testProcdeureNode5 = std::make_shared<ProcedureNode>("procedure5")
            
            //Combine procedure nodes into a vector
            std::vector<std::shared_ptr<ProcedureNode>> testProcedureNodes = {testProcdeureNode1, testProcdeureNode2, testProcdeureNode3, testProcdeureNode4, testProcdeureNode5};
            
            //Create procedure entity
            std::shared_ptr<ProcedureEntity> testProcedureEntity = std::make_shared<ProcedureEntity>();

            //Extract procedure entity from each procedure node
            for (const auto& procedure : testProcedureNodes) {
                testProcedureEntity->extractEntity(procedure);
            }

            //Get procedure map from procedure entity
            procedureMap = testProcedureEntity->getProcedureMap();

            //Check if procedure entity is extracted correctly
            Assert::IsTrue(procedureMap["procedure1"] == 1);
            Assert::IsTrue(procedureMap["procedure2"] == 2);
            Assert::IsTrue(procedureMap["procedure3"] == 3);
            Assert::IsTrue(procedureMap["procedure4"] == 4);
            Assert::IsTrue(procedureMap["procedure5"] == 5);
            
            //Create a program node
            // std::shared_ptr<ProgramNode> testProgramNode = std::make_shared<ProgramNode>("test", testProcedureNodes)

		}
		TEST_METHOD(TestStatement)
		{

		}

	};
}