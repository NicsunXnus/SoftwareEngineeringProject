#include "stdafx.h"
#include "CppUnitTest.h"
#include <cassert>
#include <PKB.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(TestPKBInsertor) {
		public:
			TEST_METHOD(TestAddEntities) {
				// Create mock data to insert
				StringMap entityStatementData1 = { {"read", {"1"}}};
				shared_ptr<StringMap> toInsert = make_shared<StringMap>(entityStatementData1);

				// Insertion
				PKB::insertor.addEntity(toInsert);

				// Create reference to EntityStorage to check database
				shared_ptr<EntityStorage> entity_storage = StorageManager::getEntityStorage();
				shared_ptr<map<ENTITY, vector<string>>> stmt_db = entity_storage->getStatementDatabase();

				vector<string> expected_vector = { "1" };

				Assert::IsTrue(vectorEqualityWrapper(stmt_db->at(READ), expected_vector));
			}
		};
}