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

				Assert::IsTrue(vectorEqualityWrapper(stmt_db->at(READ), expected_vector));
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
				Assert::IsTrue(vectorEqualityWrapper(stmt_db->at(CALL), expected_vector1));
				Assert::IsTrue(vectorEqualityWrapper(stmt_db->at(PRINT), expected_vector2));

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

				Assert::IsTrue(vectorEqualityWrapper(procedure_db->at("proc1"), expected_vector));
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
				Assert::IsTrue(vectorEqualityWrapper(procedure_db->at("proc1"), expected_vector1));
				Assert::IsTrue(vectorEqualityWrapper(procedure_db->at("proc2"), expected_vector2));

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

				Assert::IsTrue(vectorEqualityWrapper(variable_db->at("x"), expected_vector));
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
				Assert::IsTrue(vectorEqualityWrapper(variable_db->at("x"), expected_vector1));
				Assert::IsTrue(vectorEqualityWrapper(variable_db->at("y"), expected_vector2));

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

				Assert::IsTrue(vectorEqualityWrapper(constant_db->at("10"), expected_vector));
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
				Assert::IsTrue(vectorEqualityWrapper(constant_db->at("10"), expected_vector1));
				Assert::IsTrue(vectorEqualityWrapper(constant_db->at("HERE"), expected_vector2));

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

				// assert database is empty
				Assert::IsTrue(stringMapEqualityWrapper(*variable_db, { {"x", {"1", "2"}}, {"y", {"2", "5", "7"}} }));
				Assert::IsTrue(stringMapEqualityWrapper(*procedure_db, { {"proc1", {"1", "2"}}, {"proc2", {"5", "6", "7"}} }));
				Assert::IsTrue(stringMapEqualityWrapper(*constant_db, { {"10", {"1"}}, {"HERE", {"4", "7", "10"}} }));
				Assert::IsTrue(vectorEqualityWrapper(stmt_db->at(CALL), { "1", "2" }));
				Assert::IsTrue(vectorEqualityWrapper(stmt_db->at(PRINT), { "4" }));
			}
		};
}