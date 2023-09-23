#include "stdafx.h"
#include "CppUnitTest.h"
#include <cassert>
#include <PKB.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(TestPKBInsertor) {
		public:
			TEST_METHOD(TestAddSingleEntityStatementSuccess) {
				// Create mock data to insert
				StringMap entityStatementData = { {"read", {"1"}}};
				shared_ptr<StringMap> toInsert = make_shared<StringMap>(entityStatementData);

				// Insertion
				PKB::insertor.addEntity(toInsert);

				// Create reference to EntityStorage to check database
				shared_ptr<EntityStorage> entity_storage = StorageManager::getEntityStorage();
				shared_ptr<map<ENTITY, vector<string>>> stmt_db = entity_storage->getStatementDatabase();

				// assert database contains the correct data
				vector<string> expected_vector = { "1" };

				Assert::IsTrue(compare_vectors(stmt_db->at(READ), expected_vector));
			}

			TEST_METHOD(TestAddInvalidEntityStatementFailure) {
				// Create mock data to insert
				StringMap entityStatementData = { {"invalid", {"1"}} };
				shared_ptr<StringMap> toInsert = make_shared<StringMap>(entityStatementData);

				// Insertion
				Assert::ExpectException<std::runtime_error>([toInsert] {
					PKB::insertor.addEntity(toInsert);
				});
			}

			TEST_METHOD(TestAddMultipleEntityStatementsSuccess) {
				// Create mock data to insert
				StringMap entityStatementData = { {"call", {"1", "2"}}, {"print", {"4"}} };
				shared_ptr<StringMap> toInsert = make_shared<StringMap>(entityStatementData);

				// Insertion
				PKB::insertor.addEntity(toInsert);

				// Create reference to EntityStorage to check database
				shared_ptr<EntityStorage> entity_storage = StorageManager::getEntityStorage();
				shared_ptr<map<ENTITY, vector<string>>> stmt_db = entity_storage->getStatementDatabase();

				vector<string> expected_vector1 = { "1", "2" };
				vector<string> expected_vector2 = { "4" };

				// assert database contains the correct data
				Assert::IsTrue(compare_vectors(stmt_db->at(CALL), expected_vector1));
				Assert::IsTrue(compare_vectors(stmt_db->at(PRINT), expected_vector2));

				// assert database does not contain data that was not inserted
				Assert::IsTrue(stmt_db->find(READ) == stmt_db->end());
			}

			TEST_METHOD(TestAddZeroEntityStatementsSuccess) {
				// Create mock data to insert
				StringMap entityStatementData = {};
				shared_ptr<StringMap> toInsert = make_shared<StringMap>(entityStatementData);

				// Insertion
				PKB::insertor.addEntity(toInsert);

				// Create reference to EntityStorage to check database
				shared_ptr<EntityStorage> entity_storage = StorageManager::getEntityStorage();
				shared_ptr<map<ENTITY, vector<string>>> stmt_db = entity_storage->getStatementDatabase();

				// assert database is empty
				Assert::IsTrue(stmt_db->empty());
			}

			TEST_METHOD(TestAddSingleProcedureSuccess) {
				// Create mock data to insert
				StringMap procedureData = { {"proc1", {"1"}} };
				shared_ptr<StringMap> toInsert = make_shared<StringMap>(procedureData);

				// Insertion
				PKB::insertor.addEntity(toInsert, PROCEDURE);

				// Create reference to EntityStorage to check database
				shared_ptr<EntityStorage> entity_storage = StorageManager::getEntityStorage();
				shared_ptr<StringMap> procedure_db = entity_storage->getProcedureDatabase();

				// assert database contains the correct data
				vector<string> expected_vector = { "1" };

				Assert::IsTrue(compare_vectors(procedure_db->at("proc1"), expected_vector));
			}

			TEST_METHOD(TestAddMultipleProceduresSuccess) {
				// Create mock data to insert
				StringMap procedureData = { {"proc1", {"1", "2"}}, {"proc2", {"5", "6", "7"}}};
				shared_ptr<StringMap> toInsert = make_shared<StringMap>(procedureData);

				// Insertion
				PKB::insertor.addEntity(toInsert, PROCEDURE);

				// Create reference to EntityStorage to check database
				shared_ptr<EntityStorage> entity_storage = StorageManager::getEntityStorage();
				shared_ptr<StringMap> procedure_db = entity_storage->getProcedureDatabase();

				vector<string> expected_vector1 = { "1", "2" };
				vector<string> expected_vector2 = { "5", "6", "7" };

				// assert database contains the correct data
				Assert::IsTrue(compare_vectors(procedure_db->at("proc1"), expected_vector1));
				Assert::IsTrue(compare_vectors(procedure_db->at("proc2"), expected_vector2));

				// assert database does not contain data that was not inserted
				Assert::IsTrue(procedure_db->find("unknown_proc") == procedure_db->end());
			}

			TEST_METHOD(TestAddZeroProceduresSuccess) {
				// Create mock data to insert
				StringMap procedureData = {};
				shared_ptr<StringMap> toInsert = make_shared<StringMap>(procedureData);

				// Insertion
				PKB::insertor.addEntity(toInsert, PROCEDURE);

				// Create reference to EntityStorage to check database
				shared_ptr<EntityStorage> entity_storage = StorageManager::getEntityStorage();
				shared_ptr<StringMap> procedure_db = entity_storage->getProcedureDatabase();

				// assert database is empty
				Assert::IsTrue(procedure_db->empty());
			}

			TEST_METHOD(TestAddSingleVariableSuccess) {
				// Create mock data to insert
				StringMap variableData = { {"x", {"5"}} };
				shared_ptr<StringMap> toInsert = make_shared<StringMap>(variableData);

				// Insertion
				PKB::insertor.addEntity(toInsert, VARIABLE);

				// Create reference to EntityStorage to check database
				shared_ptr<EntityStorage> entity_storage = StorageManager::getEntityStorage();
				shared_ptr<StringMap> variable_db = entity_storage->getVariableDatabase();

				// assert database contains the correct data
				vector<string> expected_vector = { "5" };

				Assert::IsTrue(compare_vectors(variable_db->at("x"), expected_vector));
			}

			TEST_METHOD(TestAddMultipleVariablesSuccess) {
				// Create mock data to insert
				StringMap variableData = { {"x", {"1", "2"}}, {"y", {"2", "5", "7"}} };
				shared_ptr<StringMap> toInsert = make_shared<StringMap>(variableData);

				// Insertion
				PKB::insertor.addEntity(toInsert, VARIABLE);

				// Create reference to EntityStorage to check database
				shared_ptr<EntityStorage> entity_storage = StorageManager::getEntityStorage();
				shared_ptr<StringMap> variable_db = entity_storage->getVariableDatabase();

				vector<string> expected_vector1 = { "1", "2" };
				vector<string> expected_vector2 = { "2", "5", "7" };

				// assert database contains the correct data
				Assert::IsTrue(compare_vectors(variable_db->at("x"), expected_vector1));
				Assert::IsTrue(compare_vectors(variable_db->at("y"), expected_vector2));

				// assert database does not contain data that was not inserted
				Assert::IsTrue(variable_db->find("z") == variable_db->end());
			}

			TEST_METHOD(TestAddZeroVariablesSuccess) {
				// Create mock data to insert
				StringMap variableData = {};
				shared_ptr<StringMap> toInsert = make_shared<StringMap>(variableData);

				// Insertion
				PKB::insertor.addEntity(toInsert, VARIABLE);

				// Create reference to EntityStorage to check database
				shared_ptr<EntityStorage> entity_storage = StorageManager::getEntityStorage();
				shared_ptr<StringMap> variable_db = entity_storage->getVariableDatabase();

				// assert database is empty
				Assert::IsTrue(variable_db->empty());
			}

			TEST_METHOD(TestAddSingleConstantSuccess) {
				// Create mock data to insert
				StringMap constantData = { {"10", {"1"}} };
				shared_ptr<StringMap> toInsert = make_shared<StringMap>(constantData);

				// Insertion
				PKB::insertor.addEntity(toInsert, CONSTANT);

				// Create reference to EntityStorage to check database
				shared_ptr<EntityStorage> entity_storage = StorageManager::getEntityStorage();
				shared_ptr<StringMap> constant_db = entity_storage->getConstantDatabase();

				// assert database contains the correct data
				vector<string> expected_vector = { "1" };

				Assert::IsTrue(compare_vectors(constant_db->at("10"), expected_vector));
			}

			TEST_METHOD(TestAddMultipleConstantsSuccess) {
				// Create mock data to insert
				StringMap constantData = { {"10", {"1"}}, {"HERE", {"4", "7", "10"}}};
				shared_ptr<StringMap> toInsert = make_shared<StringMap>(constantData);

				// Insertion
				PKB::insertor.addEntity(toInsert, CONSTANT);

				// Create reference to EntityStorage to check database
				shared_ptr<EntityStorage> entity_storage = StorageManager::getEntityStorage();
				shared_ptr<StringMap> constant_db = entity_storage->getConstantDatabase();

				vector<string> expected_vector1 = { "1" };
				vector<string> expected_vector2 = { "4", "7", "10" };

				// assert database contains the correct data
				Assert::IsTrue(compare_vectors(constant_db->at("10"), expected_vector1));
				Assert::IsTrue(compare_vectors(constant_db->at("HERE"), expected_vector2));

				// assert database does not contain data that was not inserted
				Assert::IsTrue(constant_db->find("unknown_const") == constant_db->end());
			}

			TEST_METHOD(TestAddZeroConstantsSuccess) {
				// Create mock data to insert
				StringMap constantData = {};
				shared_ptr<StringMap> toInsert = make_shared<StringMap>(constantData);

				// Insertion
				PKB::insertor.addEntity(toInsert, CONSTANT);

				// Create reference to EntityStorage to check database
				shared_ptr<EntityStorage> entity_storage = StorageManager::getEntityStorage();
				shared_ptr<StringMap> constant_db = entity_storage->getConstantDatabase();

				// assert database is empty
				Assert::IsTrue(constant_db->empty());
			}

			TEST_METHOD(TestAddMultipleEntitiesSuccess) {
				// Create mock data to insert
				StringMap entityStatementData = { {"call", {"1", "2"}}, {"print", {"4"}} };
				StringMap variableData = { {"x", {"1", "2"}}, {"y", {"2", "5", "7"}} };
				StringMap procedureData = { {"proc1", {"1", "2"}}, {"proc2", {"5", "6", "7"}}};
				StringMap constantData = { {"10", {"1"}}, {"HERE", {"4", "7", "10"}}};

				shared_ptr<StringMap> toInsertStatement = make_shared<StringMap>(entityStatementData);
				shared_ptr<StringMap> toInsertVariable = make_shared<StringMap>(variableData);
				shared_ptr<StringMap> toInsertProcedure = make_shared<StringMap>(procedureData);
				shared_ptr<StringMap> toInsertConstant = make_shared<StringMap>(constantData);

				// Insertion
				PKB::insertor.addEntity(toInsertStatement);
				PKB::insertor.addEntity(toInsertConstant, CONSTANT);
				PKB::insertor.addEntity(toInsertProcedure, PROCEDURE);
				PKB::insertor.addEntity(toInsertVariable, VARIABLE);

				// Create reference to EntityStorage to check database
				shared_ptr<EntityStorage> entity_storage = StorageManager::getEntityStorage();
				shared_ptr<StringMap> constant_db = entity_storage->getConstantDatabase();
				shared_ptr<StringMap> procedure_db = entity_storage->getProcedureDatabase();
				shared_ptr<StringMap> variable_db = entity_storage->getVariableDatabase();
				shared_ptr<map<ENTITY, vector<string>>> stmt_db = entity_storage->getStatementDatabase();

				// assert database has correct entries
				Assert::IsTrue(compare_maps(*variable_db, variableData));
				Assert::IsTrue(compare_maps(*procedure_db, procedureData));
				Assert::IsTrue(compare_maps(*constant_db, constantData));
				Assert::IsTrue(compare_vectors(stmt_db->at(CALL), { "1", "2" }));
				Assert::IsTrue(compare_vectors(stmt_db->at(PRINT), { "4" }));
			}

			TEST_METHOD(TestAddUsesAbstractionSuccess) {
				// Create mock data to insert
				StringMap usesData = { {"x", {"main", "3", "6"}}};
				shared_ptr<StringMap> toInsert = make_shared<StringMap>(usesData);

				// Insertion
				PKB::insertor.addAbstraction(toInsert, USES);

				// Create reference to AbstractionStorage to check database
				shared_ptr<AbstractionStorage> uses_storage = StorageManager::getAbstractionStorage(USES);
				shared_ptr<StringMap> db = uses_storage->getDatabase();

				// assert database contains the correct data
				Assert::IsTrue(compare_maps(*db, usesData));
			}

			TEST_METHOD(TestAddModifiesAbstractionSuccess) {
				// Create mock data to insert
				StringMap modifiesData = { {"y", {"main", "8", "proc1", "2"}} };
				shared_ptr<StringMap> toInsert = make_shared<StringMap>(modifiesData);

				// Insertion
				PKB::insertor.addAbstraction(toInsert, MODIFIES);

				// Create reference to AbstractionStorage to check database
				shared_ptr<AbstractionStorage> modifies_storage = StorageManager::getAbstractionStorage(MODIFIES);
				shared_ptr<StringMap> db = modifies_storage->getDatabase();

				// assert database contains the correct data
				Assert::IsTrue(compare_maps(*db, modifiesData));
			}

			TEST_METHOD(TestAddFollowsAbstractionSuccess) {
				// Create mock data to insert
				StringMap followsStarData = { {"1", {"2", "3", "6", "7"}}, {"2", {"3", "6", "7"}}};
				StringMap followsData = { {"1", {"2"}}, {"2", {"3"}} };
				shared_ptr<StringMap> toInsert = make_shared<StringMap>(followsStarData);

				// Insertion
				PKB::insertor.addAbstraction(toInsert, FOLLOWS);

				// Create reference to AbstractionStorage to check database
				shared_ptr<AbstractionStorage> follows_storage = StorageManager::getAbstractionStorage(FOLLOWS);
				shared_ptr<StringMap> db = follows_storage->getTruncatedDatabase();

				// assert database contains the correct data (only first element of vector)
				Assert::IsTrue(compare_maps(*db, followsData));
			}

			TEST_METHOD(TestAddFollowsStarAbstractionSuccess) {
				// Create mock data to insert
				StringMap followsStarData = { {"1", {"2", "4", "5"}} };
				shared_ptr<StringMap> toInsert = make_shared<StringMap>(followsStarData);

				// Insertion
				PKB::insertor.addAbstraction(toInsert, FOLLOWSSTAR);

				// Create reference to AbstractionStorage to check database
				shared_ptr<AbstractionStorage> follows_storage = StorageManager::getAbstractionStorage(FOLLOWS);
				shared_ptr<AbstractionStorage> followsstar_storage = StorageManager::getAbstractionStorage(FOLLOWSSTAR);

				// assert that same storage (and database) is used for follows and followsstar
				Assert::IsTrue(follows_storage == followsstar_storage);

				shared_ptr<StringMap> db = followsstar_storage->getDatabase();

				// assert database contains the correct data
				Assert::IsTrue(compare_maps(*db, followsStarData));
			}

			TEST_METHOD(TestAddParentAbstractionSuccess) {
				// Create mock data to insert
				StringMap parentStarData = { {"10", {"7", "3", "1"}}, {"6", {"5", "1"}} };
				StringMap parentData = { {"10", {"7"}}, {"6", {"5"}} };
				shared_ptr<StringMap> toInsert = make_shared<StringMap>(parentStarData);

				// Insertion
				PKB::insertor.addAbstraction(toInsert, PARENT);

				// Create reference to AbstractionStorage to check database
				shared_ptr<AbstractionStorage> parent_storage = StorageManager::getAbstractionStorage(PARENT);
				shared_ptr<StringMap> db = parent_storage->getTruncatedDatabase();

				// assert database contains the correct data (only first element of vector)
				Assert::IsTrue(compare_maps(*db, parentData));
			}

			TEST_METHOD(TestAddParentStarAbstractionSuccess) {
				// Create mock data to insert
				StringMap parentStarData = { {"10", {"9", "7", "5"}} };
				shared_ptr<StringMap> toInsert = make_shared<StringMap>(parentStarData);

				// Insertion
				PKB::insertor.addAbstraction(toInsert, PARENTSTAR);

				// Create reference to AbstractionStorage to check database
				shared_ptr<AbstractionStorage> parent_storage = StorageManager::getAbstractionStorage(PARENT);
				shared_ptr<AbstractionStorage> parentstar_storage = StorageManager::getAbstractionStorage(PARENTSTAR);

				// assert that same storage (and database) is used for follows and followsstar
				Assert::IsTrue(parent_storage == parentstar_storage);

				shared_ptr<StringMap> db = parentstar_storage->getDatabase();

				// assert database contains the correct data
				Assert::IsTrue(compare_maps(*db, parentStarData));
			}
			
			TEST_METHOD(TestAddZeroAbstractionSuccess) {
				// Create mock data to insert
				StringMap emptyAbstractionData = {};
				shared_ptr<StringMap> toInsert = make_shared<StringMap>(emptyAbstractionData);

				// Insertion
				PKB::insertor.addAbstraction(toInsert, PARENT);
				PKB::insertor.addAbstraction(toInsert, PARENTSTAR);
				PKB::insertor.addAbstraction(toInsert, FOLLOWS);
				PKB::insertor.addAbstraction(toInsert, FOLLOWSSTAR);
				PKB::insertor.addAbstraction(toInsert, MODIFIES);
				PKB::insertor.addAbstraction(toInsert, USES);

				// Create reference to AbstractionStorage to check database
				shared_ptr<AbstractionStorage> parent_storage = StorageManager::getAbstractionStorage(PARENT);
				shared_ptr<AbstractionStorage> follows_storage = StorageManager::getAbstractionStorage(FOLLOWS);
				shared_ptr<AbstractionStorage> modifies_storage = StorageManager::getAbstractionStorage(MODIFIES);
				shared_ptr<AbstractionStorage> uses_storage = StorageManager::getAbstractionStorage(USES);	

				shared_ptr<StringMap> parent_db = parent_storage->getTruncatedDatabase();
				shared_ptr<StringMap> parentstar_db = parent_storage->getDatabase();
				shared_ptr<StringMap> follows_db = follows_storage->getTruncatedDatabase();
				shared_ptr<StringMap> followsstar_db = follows_storage->getDatabase();
				shared_ptr<StringMap> modifies_db = modifies_storage->getDatabase();
				shared_ptr<StringMap> uses_db = uses_storage->getDatabase();

				// assert database is empty
				Assert::IsTrue(parent_db->empty());
				Assert::IsTrue(parentstar_db->empty());
				Assert::IsTrue(follows_db->empty());
				Assert::IsTrue(followsstar_db->empty());
				Assert::IsTrue(modifies_db->empty());
				Assert::IsTrue(uses_db->empty());
			}

			//TEST_METHOD(TestAddMultipleAbstractionsSuccess) {
			//	// Create mock data to insert
			//	StringMap usesData = { {"x", {"main", "3", "6"}} };
			//	StringMap modifiesData = { {"y", {"main", "8", "proc1", "2"}} };
			//	StringMap followsStarData = { {"1", {"2", "3", "6", "7"}}, {"2", {"3", "6", "7"}} };
			//	StringMap followsData = { {"1", {"2"}}, {"2", {"3"}} };
			//	StringMap parentStarData = { {"10", {"7", "3", "1"}}, {"6", {"5", "1"}} };
			//	StringMap parentData = { {"10", {"7"}}, {"6", {"5"}} };
			//
			//	shared_ptr<StringMap> toInsertUses = make_shared<StringMap>(usesData);
			//	shared_ptr<StringMap> toInsertModifies = make_shared<StringMap>(modifiesData);
			//	shared_ptr<StringMap> toInsertFollows = make_shared<StringMap>(followsStarData);
			//	shared_ptr<StringMap> toInsertParent = make_shared<StringMap>(parentStarData);
			//
			//	// Insertion
			//	PKB::insertor.addAbstraction(toInsertUses, USES);
			//	PKB::insertor.addAbstraction(toInsertModifies, MODIFIES);
			//	PKB::insertor.addAbstraction(toInsertFollows, FOLLOWS);
			//	PKB::insertor.addAbstraction(toInsertParent, PARENT);
			//
			//	// Create reference to AbstractionStorage to check database
			//	shared_ptr<AbstractionStorage> parent_storage = StorageManager::getAbstractionStorage(PARENT);
			//	shared_ptr<AbstractionStorage> follows_storage = StorageManager::getAbstractionStorage(FOLLOWS);
			//	shared_ptr<AbstractionStorage> modifies_storage = StorageManager::getAbstractionStorage(MODIFIES);
			//	shared_ptr<AbstractionStorage> uses_storage = StorageManager::getAbstractionStorage(USES);
			//
			//	shared_ptr<StringMap> parent_db = parent_storage->getTruncatedDatabase();
			//	shared_ptr<StringMap> parentstar_db = parent_storage->getDatabase();
			//	shared_ptr<StringMap> follows_db = follows_storage->getTruncatedDatabase();
			//	shared_ptr<StringMap> followsstar_db = follows_storage->getDatabase();
			//	shared_ptr<StringMap> modifies_db = modifies_storage->getDatabase();
			//	shared_ptr<StringMap> uses_db = uses_storage->getDatabase();
			//
			//	// assert database has correct entries
			//	Assert::IsTrue(compare_maps(*parent_db, parentData));
			//	Assert::IsTrue(compare_maps(*parentstar_db, parentStarData));
			//	Assert::IsTrue(compare_maps(*follows_db, followsData));
			//	Assert::IsTrue(compare_maps(*followsstar_db, followsStarData));
			//	Assert::IsTrue(compare_maps(*modifies_db, modifiesData));
			//	Assert::IsTrue(compare_maps(*uses_db, usesData));
			//}
	};
}