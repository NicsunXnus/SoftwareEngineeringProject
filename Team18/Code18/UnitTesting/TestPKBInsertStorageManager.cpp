#include "stdafx.h"
#include "CppUnitTest.h"
#include <cassert>
#include <PKB.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(TestPKBInsertStorageManager) {
		public:
			TEST_METHOD(TestAddSingleEntityStatementSuccess) {
				// Create mock data to insert
				map<string, unordered_set<string>> entityStatementData = { {"read", {"1"}}};
				shared_ptr<map<string, unordered_set<string>>> toInsert = make_shared<map<string, unordered_set<string>>>(entityStatementData);

				// Insertion
				PKB::insertor.addEntity(toInsert);

				// Create reference to EntityStorage to check database
				shared_ptr<EntityStorage> entity_storage = StorageManager::getEntityStorage();
				shared_ptr<map<ENTITY, unordered_set<string>>> stmt_db = entity_storage->getStatementDatabase();

				// assert database contains the correct data
				unordered_set<string> expected_vector = { "1" };

				unordered_set<string> sss = stmt_db->at(READ);
				Assert::IsTrue(compare_sets(stmt_db->at(READ), expected_vector));
			}

			TEST_METHOD(TestAddInvalidEntityStatementFailure) {
				// Create mock data to insert
				map<string, unordered_set<string>> entityStatementData = { {"invalid", {"1"}} };
				shared_ptr<map<string, unordered_set<string>>> toInsert = make_shared<map<string, unordered_set<string>>>(entityStatementData);

				// Insertion
				Assert::ExpectException<runtime_error>([toInsert] {
					PKB::insertor.addEntity(toInsert);
				});
			}

			TEST_METHOD(TestAddMultipleEntityStatementsSuccess) {
				// Create mock data to insert
				map<string, unordered_set<string>> entityStatementData = { {"call", {"1", "2"}}, {"print", {"4"}} };
				shared_ptr<map<string, unordered_set<string>>> toInsert = make_shared<map<string, unordered_set<string>>>(entityStatementData);

				// Insertion
				PKB::insertor.addEntity(toInsert);

				// Create reference to EntityStorage to check database
				shared_ptr<EntityStorage> entity_storage = StorageManager::getEntityStorage();
				shared_ptr<map<ENTITY, unordered_set<string>>> stmt_db = entity_storage->getStatementDatabase();

				unordered_set<string> expected_vector1 = { "1", "2" };
				unordered_set<string> expected_vector2 = { "4" };

				// assert database contains the correct data
				Assert::IsTrue(compare_sets(stmt_db->at(CALL), expected_vector1));
				Assert::IsTrue(compare_sets(stmt_db->at(PRINT), expected_vector2));

				// assert database does not contain data that was not inserted
				Assert::IsTrue(stmt_db->find(READ) == stmt_db->end());
			}

			TEST_METHOD(TestAddZeroEntityStatementsSuccess) {
				// Create mock data to insert
				map<string, unordered_set<string>> entityStatementData = {};
				shared_ptr<map<string, unordered_set<string>>> toInsert = make_shared<map<string, unordered_set<string>>>(entityStatementData);

				// Insertion
				PKB::insertor.addEntity(toInsert);

				// Create reference to EntityStorage to check database
				shared_ptr<EntityStorage> entity_storage = StorageManager::getEntityStorage();
				shared_ptr<map<ENTITY, unordered_set<string>>> stmt_db = entity_storage->getStatementDatabase();

				// assert database is empty
				Assert::IsTrue(stmt_db->empty());
			}

			TEST_METHOD(TestAddSingleProcedureSuccess) {
				// Create mock data to insert
				map<string, unordered_set<string>> procedureData = { {"proc1", {"1"}} };
				shared_ptr<map<string, unordered_set<string>>> toInsert = make_shared<map<string, unordered_set<string>>>(procedureData);

				// Insertion
				PKB::insertor.addEntity(toInsert, PROCEDURE);

				// Create reference to EntityStorage to check database
				shared_ptr<EntityStorage> entity_storage = StorageManager::getEntityStorage();
				shared_ptr<map<string, unordered_set<string>>> procedure_db = entity_storage->getProcedureDatabase();

				// assert database contains the correct data
				unordered_set<string> expected_vector = { "1" };

				Assert::IsTrue(compare_sets(procedure_db->at("proc1"), expected_vector));
			}

			TEST_METHOD(TestAddMultipleProceduresSuccess) {
				// Create mock data to insert
				map<string, unordered_set<string>> procedureData = { {"proc1", {"1", "2"}}, {"proc2", {"5", "6", "7"}}};
				shared_ptr<map<string, unordered_set<string>>> toInsert = make_shared<map<string, unordered_set<string>>>(procedureData);

				// Insertion
				PKB::insertor.addEntity(toInsert, PROCEDURE);

				// Create reference to EntityStorage to check database
				shared_ptr<EntityStorage> entity_storage = StorageManager::getEntityStorage();
				shared_ptr<map<string, unordered_set<string>>> procedure_db = entity_storage->getProcedureDatabase();

				unordered_set<string> expected_vector1 = { "1", "2" };
				unordered_set<string> expected_vector2 = { "5", "6", "7" };

				// assert database contains the correct data
				Assert::IsTrue(compare_sets(procedure_db->at("proc1"), expected_vector1));
				Assert::IsTrue(compare_sets(procedure_db->at("proc2"), expected_vector2));

				// assert database does not contain data that was not inserted
				Assert::IsTrue(procedure_db->find("unknown_proc") == procedure_db->end());
			}

			TEST_METHOD(TestAddZeroProceduresSuccess) {
				// Create mock data to insert
				map<string, unordered_set<string>> procedureData = {};
				shared_ptr<map<string, unordered_set<string>>> toInsert = make_shared<map<string, unordered_set<string>>>(procedureData);

				// Insertion
				PKB::insertor.addEntity(toInsert, PROCEDURE);

				// Create reference to EntityStorage to check database
				shared_ptr<EntityStorage> entity_storage = StorageManager::getEntityStorage();
				shared_ptr<map<string, unordered_set<string>>> procedure_db = entity_storage->getProcedureDatabase();

				// assert database is empty
				Assert::IsTrue(procedure_db->empty());
			}

			TEST_METHOD(TestAddSingleVariableSuccess) {
				// Create mock data to insert
				map<string, unordered_set<string>> variableData = { {"x", {"5"}} };
				shared_ptr<map<string, unordered_set<string>>> toInsert = make_shared<map<string, unordered_set<string>>>(variableData);

				// Insertion
				PKB::insertor.addEntity(toInsert, VARIABLE);

				// Create reference to EntityStorage to check database
				shared_ptr<EntityStorage> entity_storage = StorageManager::getEntityStorage();
				shared_ptr<map<string, unordered_set<string>>> variable_db = entity_storage->getVariableDatabase();

				// assert database contains the correct data
				unordered_set<string> expected_vector = { "5" };

				Assert::IsTrue(compare_sets(variable_db->at("x"), expected_vector));
			}

			TEST_METHOD(TestAddMultipleVariablesSuccess) {
				// Create mock data to insert
				map<string, unordered_set<string>> variableData = { {"x", {"1", "2"}}, {"y", {"2", "5", "7"}} };
				shared_ptr<map<string, unordered_set<string>>> toInsert = make_shared<map<string, unordered_set<string>>>(variableData);

				// Insertion
				PKB::insertor.addEntity(toInsert, VARIABLE);

				// Create reference to EntityStorage to check database
				shared_ptr<EntityStorage> entity_storage = StorageManager::getEntityStorage();
				shared_ptr<map<string, unordered_set<string>>> variable_db = entity_storage->getVariableDatabase();

				unordered_set<string> expected_vector1 = { "1", "2" };
				unordered_set<string> expected_vector2 = { "2", "5", "7" };

				// assert database contains the correct data
				Assert::IsTrue(compare_sets(variable_db->at("x"), expected_vector1));
				Assert::IsTrue(compare_sets(variable_db->at("y"), expected_vector2));

				// assert database does not contain data that was not inserted
				Assert::IsTrue(variable_db->find("z") == variable_db->end());
			}

			TEST_METHOD(TestAddZeroVariablesSuccess) {
				// Create mock data to insert
				map<string, unordered_set<string>> variableData = {};
				shared_ptr<map<string, unordered_set<string>>> toInsert = make_shared<map<string, unordered_set<string>>>(variableData);

				// Insertion
				PKB::insertor.addEntity(toInsert, VARIABLE);

				// Create reference to EntityStorage to check database
				shared_ptr<EntityStorage> entity_storage = StorageManager::getEntityStorage();
				shared_ptr<map<string, unordered_set<string>>> variable_db = entity_storage->getVariableDatabase();

				// assert database is empty
				Assert::IsTrue(variable_db->empty());
			}

			TEST_METHOD(TestAddSingleConstantSuccess) {
				// Create mock data to insert
				map<string, unordered_set<string>> constantData = { {"10", {"1"}} };
				shared_ptr<map<string, unordered_set<string>>> toInsert = make_shared<map<string, unordered_set<string>>>(constantData);

				// Insertion
				PKB::insertor.addEntity(toInsert, CONSTANT);

				// Create reference to EntityStorage to check database
				shared_ptr<EntityStorage> entity_storage = StorageManager::getEntityStorage();
				shared_ptr<map<string, unordered_set<string>>> constant_db = entity_storage->getConstantDatabase();

				// assert database contains the correct data
				unordered_set<string> expected_vector = { "1" };

				Assert::IsTrue(compare_sets(constant_db->at("10"), expected_vector));
			}

			TEST_METHOD(TestAddMultipleConstantsSuccess) {
				// Create mock data to insert
				map<string, unordered_set<string>> constantData = { {"10", {"1"}}, {"HERE", {"4", "7", "10"}}};
				shared_ptr<map<string, unordered_set<string>>> toInsert = make_shared<map<string, unordered_set<string>>>(constantData);

				// Insertion
				PKB::insertor.addEntity(toInsert, CONSTANT);

				// Create reference to EntityStorage to check database
				shared_ptr<EntityStorage> entity_storage = StorageManager::getEntityStorage();
				shared_ptr<map<string, unordered_set<string>>> constant_db = entity_storage->getConstantDatabase();

				unordered_set<string> expected_vector1 = { "1" };
				unordered_set<string> expected_vector2 = { "4", "7", "10" };

				// assert database contains the correct data
				Assert::IsTrue(compare_sets(constant_db->at("10"), expected_vector1));
				Assert::IsTrue(compare_sets(constant_db->at("HERE"), expected_vector2));

				// assert database does not contain data that was not inserted
				Assert::IsTrue(constant_db->find("unknown_const") == constant_db->end());
			}

			TEST_METHOD(TestAddZeroConstantsSuccess) {
				// Create mock data to insert
				map<string, unordered_set<string>> constantData = {};
				shared_ptr<map<string, unordered_set<string>>> toInsert = make_shared<map<string, unordered_set<string>>>(constantData);

				// Insertion
				PKB::insertor.addEntity(toInsert, CONSTANT);

				// Create reference to EntityStorage to check database
				shared_ptr<EntityStorage> entity_storage = StorageManager::getEntityStorage();
				shared_ptr<map<string, unordered_set<string>>> constant_db = entity_storage->getConstantDatabase();

				// assert database is empty
				Assert::IsTrue(constant_db->empty());
			}

			TEST_METHOD(TestAddCallProcNameSuccess) {
				// Create mock data to insert
				map<string, unordered_set<string>> callProcNameData = { {"2", {"proc1"}}, {"4", {"proc2"}}, {"11", {"proc3"}} };
				shared_ptr<map<string, unordered_set<string>>> toInsert = make_shared<map<string, unordered_set<string>>>(callProcNameData);

				// Insertion
				PKB::insertor.addEntityNames(toInsert, CALL);

				// Create reference to EntityStorage to check database
				shared_ptr<EntityStorage> entity_storage = StorageManager::getEntityStorage();
				shared_ptr<map<string, unordered_set<string>>> call_procname_db = entity_storage->getCallProcnameDatabase();

				// assert database contains the correct data
				Assert::IsTrue(compare_maps(*call_procname_db, callProcNameData));
			}

			TEST_METHOD(TestAddReadVarNameSuccess) {
				// Create mock data to insert
				map<string, unordered_set<string>> readVarNameData = { {"4", {"x"}}, {"13", {"y"}}, {"21", {"z"}} };
				shared_ptr<map<string, unordered_set<string>>> toInsert = make_shared<map<string, unordered_set<string>>>(readVarNameData);

				// Insertion
				PKB::insertor.addEntityNames(toInsert, READ);

				// Create reference to EntityStorage to check database
				shared_ptr<EntityStorage> entity_storage = StorageManager::getEntityStorage();
				shared_ptr<map<string, unordered_set<string>>> read_varname_db = entity_storage->getReadVarnameDatabase();

				// assert database contains the correct data
				Assert::IsTrue(compare_maps(*read_varname_db, readVarNameData));
			}

			TEST_METHOD(TestAddPrintVarNameSuccess) {
				// Create mock data to insert
				map<string, unordered_set<string>> printVarNameData = { {"3", {"x"}}, {"7", {"y"}}, {"19", {"z"}} };
				shared_ptr<map<string, unordered_set<string>>> toInsert = make_shared<map<string, unordered_set<string>>>(printVarNameData);

				// Insertion
				PKB::insertor.addEntityNames(toInsert, PRINT);

				// Create reference to EntityStorage to check database
				shared_ptr<EntityStorage> entity_storage = StorageManager::getEntityStorage();
				shared_ptr<map<string, unordered_set<string>>> print_varname_db = entity_storage->getPrintVarnameDatabase();

				// assert database contains the correct data
				Assert::IsTrue(compare_maps(*print_varname_db, printVarNameData));
			}

			TEST_METHOD(TestAddInvalidEntityNameFailure) {
				// Create mock data to insert
				map<string, unordered_set<string>> dummyNameData = { {"3", {"x"}}, {"7", {"y"}}, {"19", {"z"}} };
				shared_ptr<map<string, unordered_set<string>>> toInsert = make_shared<map<string, unordered_set<string>>>(dummyNameData);

				Assert::ExpectException<runtime_error>([toInsert] {
					PKB::insertor.addEntityNames(toInsert, STMT);
				});
			}

			TEST_METHOD(TestAddMultipleEntitiesSuccess) {
				// Create mock data to insert
				map<string, unordered_set<string>> entityStatementData = { {"call", {"1", "2"}}, {"print", {"4"}} };
				map<string, unordered_set<string>> variableData = { {"x", {"1", "2"}}, {"y", {"2", "5", "7"}} };
				map<string, unordered_set<string>> procedureData = { {"proc1", {"1", "2"}}, {"proc2", {"5", "6", "7"}}};
				map<string, unordered_set<string>> constantData = { {"10", {"1"}}, {"HERE", {"4", "7", "10"}}};

				shared_ptr<map<string, unordered_set<string>>> toInsertStatement = make_shared<map<string, unordered_set<string>>>(entityStatementData);
				shared_ptr<map<string, unordered_set<string>>> toInsertVariable = make_shared<map<string, unordered_set<string>>>(variableData);
				shared_ptr<map<string, unordered_set<string>>> toInsertProcedure = make_shared<map<string, unordered_set<string>>>(procedureData);
				shared_ptr<map<string, unordered_set<string>>> toInsertConstant = make_shared<map<string, unordered_set<string>>>(constantData);

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
				shared_ptr<map<ENTITY, unordered_set<string>>> stmt_db = entity_storage->getStatementDatabase();

				// assert database has correct entries
				Assert::IsTrue(compare_maps(*variable_db, variableData));
				Assert::IsTrue(compare_maps(*procedure_db, procedureData));
				Assert::IsTrue(compare_maps(*constant_db, constantData));
				Assert::IsTrue(compare_sets(stmt_db->at(CALL), { "1", "2" }));
				Assert::IsTrue(compare_sets(stmt_db->at(PRINT), { "4" }));
			}

			TEST_METHOD(TestAddUsesAbstractionSuccess) {
				// Create mock data to insert
				map<string, unordered_set<string>> usesData = { {"x", {"main", "3", "6"}}};
				shared_ptr<map<string, unordered_set<string>>> toInsert = make_shared<map<string, unordered_set<string>>>(usesData);

				// Insertion
				PKB::insertor.addAbstraction(toInsert, USES);

				// Create reference to AbstractionStorage to check database
				shared_ptr<AbstractionStorage> uses_storage = StorageManager::getAbstractionStorage(USES);
				shared_ptr<map<string, unordered_set<string>>> db = uses_storage->getDatabase();

				// assert database contains the correct data
				Assert::IsTrue(compare_maps(*db, usesData));
			}

			TEST_METHOD(TestAddModifiesAbstractionSuccess) {
				// Create mock data to insert
				map<string, unordered_set<string>> modifiesData = { {"y", {"main", "8", "proc1", "2"}} };
				shared_ptr<map<string, unordered_set<string>>> toInsert = make_shared<map<string, unordered_set<string>>>(modifiesData);

				// Insertion
				PKB::insertor.addAbstraction(toInsert, MODIFIES);

				// Create reference to AbstractionStorage to check database
				shared_ptr<AbstractionStorage> modifies_storage = StorageManager::getAbstractionStorage(MODIFIES);
				shared_ptr<map<string, unordered_set<string>>> db = modifies_storage->getDatabase();

				// assert database contains the correct data
				Assert::IsTrue(compare_maps(*db, modifiesData));
			}

			TEST_METHOD(TestAddFollowsAbstractionSuccess) {
				// Create mock data to insert
				StringMap followsData = { {"1", {"2"}}, {"2", {"3"}} };
				shared_ptr<StringMap> toInsert = make_shared<StringMap>(followsData);

				// Insertion
				PKB::insertor.addAbstraction(toInsert, FOLLOWS);

				// Create reference to AbstractionStorage to check database
				shared_ptr<AbstractionStorage> follows_storage = StorageManager::getAbstractionStorage(FOLLOWS);
				shared_ptr<StringMap> db = follows_storage->getDatabase();

				// assert database contains the correct data (only first element of vector)
				Assert::IsTrue(compare_maps(*db, followsData));
			}

			TEST_METHOD(TestAddFollowsStarAbstractionSuccess) {
				// Create mock data to insert
				StringMap followsStarData = { {"1", {"2", "3", "6", "7"}}, {"2", {"3", "6", "7"}} };
				shared_ptr<StringMap> toInsert = make_shared<StringMap>(followsStarData);

				// Insertion
				PKB::insertor.addAbstraction(toInsert, FOLLOWSSTAR);

				// Create reference to AbstractionStorage to check database
				shared_ptr<AbstractionStorage> followsstar_storage = StorageManager::getAbstractionStorage(FOLLOWSSTAR);
				shared_ptr<StringMap> db = followsstar_storage->getDatabase();

				// assert database contains the correct data
				Assert::IsTrue(compare_maps(*db, followsStarData));
			}

			TEST_METHOD(TestAddParentAbstractionSuccess) {
				// Create mock data to insert
				StringMap parentData = { {"10", {"7"}}, {"6", {"5"}} };
				shared_ptr<StringMap> toInsert = make_shared<StringMap>(parentData);

				// Insertion
				PKB::insertor.addAbstraction(toInsert, PARENT);

				// Create reference to AbstractionStorage to check database
				shared_ptr<AbstractionStorage> parent_storage = StorageManager::getAbstractionStorage(PARENT);
				shared_ptr<StringMap> db = parent_storage->getDatabase();

				// assert database contains the correct data (only first element of vector)
				Assert::IsTrue(compare_maps(*db, parentData));
			}

			TEST_METHOD(TestAddParentStarAbstractionSuccess) {
				// Create mock data to insert
				StringMap parentStarData = { {"10", {"7", "3", "1"}}, {"6", {"5", "1"}} };
				shared_ptr<StringMap> toInsert = make_shared<StringMap>(parentStarData);

				// Insertion
				PKB::insertor.addAbstraction(toInsert, PARENTSTAR);

				// Create reference to AbstractionStorage to check database
				shared_ptr<AbstractionStorage> parentstar_storage = StorageManager::getAbstractionStorage(PARENTSTAR);
				shared_ptr<StringMap> db = parentstar_storage->getDatabase();

				// assert database contains the correct data
				Assert::IsTrue(compare_maps(*db, parentStarData));
			}

			TEST_METHOD(TestAddCallsAbstractionSuccess) {
				// Create mock data to insert
				StringMap callsData = { {"main", {"proc1"}}, {"proc1", {"proc2"}}, {"proc2", {"proc3"}} };
				shared_ptr<StringMap> toInsert = make_shared<StringMap>(callsData);

				// Insertion
				PKB::insertor.addAbstraction(toInsert, CALLS);

				// Create reference to AbstractionStorage to check database
				shared_ptr<AbstractionStorage> calls_storage = StorageManager::getAbstractionStorage(CALLS);
				shared_ptr<StringMap> db = calls_storage->getDatabase();

				// assert database contains the correct data (only first element of vector)
				Assert::IsTrue(compare_maps(*db, callsData));
			}

			TEST_METHOD(TestAddCallsStarAbstractionSuccess) {
				// Create mock data to insert
				map<string, unordered_set<string>> callsStarData = { {"main", {"proc1", "proc2", "proc3"}}, {"proc1", {"proc2", "proc3"}}, {"proc2", {"proc3"}} };
				shared_ptr<map<string, unordered_set<string>>> toInsert = make_shared<map<string, unordered_set<string>>>(callsStarData);

				// Insertion
				PKB::insertor.addAbstraction(toInsert, CALLSSTAR);

				// Create reference to AbstractionStorage to check database
				shared_ptr<AbstractionStorage> callsstar_storage = StorageManager::getAbstractionStorage(CALLSSTAR);
				shared_ptr<StringMap> db = callsstar_storage->getDatabase();

				// assert database contains the correct data
				Assert::IsTrue(compare_maps(*db, callsStarData));
			}

			TEST_METHOD(TestAddNextAbstractionSuccess) {
				// Create mock data to insert
				map<string, unordered_set<string>> nextDataValid = { {"1", {"2"}}, {"2", {"3"}}, {"3", {"7"}}, {"7", {"1"}} };
				shared_ptr<map<string, unordered_set<string>>> toInsert = make_shared<map<string, unordered_set<string>>>(nextDataValid);

				// Insertion
				PKB::insertor.addAbstraction(toInsert, NEXT);

				// Create reference to AbstractionStorage to check database
				shared_ptr<AbstractionStorage> next_storage = StorageManager::getAbstractionStorage(NEXT);
				shared_ptr<map<string, unordered_set<string>>> db = next_storage->getDatabase();

				// assert database contains the correct data
				Assert::IsTrue(compare_maps(*db, nextDataValid));
			}
			
			TEST_METHOD(TestAddZeroAbstractionSuccess) {
				// Create mock data to insert
				map<string, unordered_set<string>> emptyAbstractionData = {};
				shared_ptr<map<string, unordered_set<string>>> toInsert = make_shared<map<string, unordered_set<string>>>(emptyAbstractionData);

				// Insertion
				PKB::insertor.addAbstraction(toInsert, PARENT);
				PKB::insertor.addAbstraction(toInsert, PARENTSTAR);
				PKB::insertor.addAbstraction(toInsert, FOLLOWS);
				PKB::insertor.addAbstraction(toInsert, FOLLOWSSTAR);
				PKB::insertor.addAbstraction(toInsert, MODIFIES);
				PKB::insertor.addAbstraction(toInsert, USES);

				// Create reference to AbstractionStorage to check database
				shared_ptr<AbstractionStorage> parent_storage = StorageManager::getAbstractionStorage(PARENT);
				shared_ptr<AbstractionStorage> parentstar_storage = StorageManager::getAbstractionStorage(PARENTSTAR);
				shared_ptr<AbstractionStorage> follows_storage = StorageManager::getAbstractionStorage(FOLLOWS);
				shared_ptr<AbstractionStorage> followsstar_storage = StorageManager::getAbstractionStorage(FOLLOWSSTAR);
				shared_ptr<AbstractionStorage> modifies_storage = StorageManager::getAbstractionStorage(MODIFIES);
				shared_ptr<AbstractionStorage> uses_storage = StorageManager::getAbstractionStorage(USES);	

				shared_ptr<StringMap> parent_db = parent_storage->getDatabase();
				shared_ptr<StringMap> parentstar_db = parentstar_storage->getDatabase();
				shared_ptr<StringMap> follows_db = follows_storage->getDatabase();
				shared_ptr<StringMap> followsstar_db = followsstar_storage->getDatabase();
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

			TEST_METHOD(TestAddMultipleAbstractionsSuccess) {
				// Create mock data to insert
				map<string, unordered_set<string>> usesData = { {"x", {"main", "3", "6"}} };
				map<string, unordered_set<string>> modifiesData = { {"y", {"main", "8", "proc1", "2"}} };
				map<string, unordered_set<string>> followsStarData = { {"1", {"2", "3", "6", "7"}}, {"2", {"3", "6", "7"}} };
				map<string, unordered_set<string>> followsData = { {"1", {"2"}}, {"2", {"3"}} };
				map<string, unordered_set<string>> parentStarData = { {"10", {"7", "3", "1"}}, {"6", {"5", "1"}} };
				map<string, unordered_set<string>> parentData = { {"10", {"7"}}, {"6", {"5"}} };
			
				shared_ptr<StringMap> toInsertUses = make_shared<StringMap>(usesData);
				shared_ptr<StringMap> toInsertModifies = make_shared<StringMap>(modifiesData);
				shared_ptr<StringMap> toInsertFollowsStar = make_shared<StringMap>(followsStarData);
				shared_ptr<StringMap> toInsertFollows = make_shared<StringMap>(followsData);
				shared_ptr<StringMap> toInsertParentStar = make_shared<StringMap>(parentStarData);
				shared_ptr<StringMap> toInsertParent = make_shared<StringMap>(parentData);
			
				// Insertion
				PKB::insertor.addAbstraction(toInsertUses, USES);
				PKB::insertor.addAbstraction(toInsertModifies, MODIFIES);
				PKB::insertor.addAbstraction(toInsertFollows, FOLLOWS);
				PKB::insertor.addAbstraction(toInsertFollowsStar, FOLLOWSSTAR);
				PKB::insertor.addAbstraction(toInsertParent, PARENT);
				PKB::insertor.addAbstraction(toInsertParentStar, PARENTSTAR);
			
				// Create reference to AbstractionStorage to check database
				shared_ptr<AbstractionStorage> parent_storage = StorageManager::getAbstractionStorage(PARENT);
				shared_ptr<AbstractionStorage> parentstar_storage = StorageManager::getAbstractionStorage(PARENTSTAR);
				shared_ptr<AbstractionStorage> follows_storage = StorageManager::getAbstractionStorage(FOLLOWS);
				shared_ptr<AbstractionStorage> followsstar_storage = StorageManager::getAbstractionStorage(FOLLOWSSTAR);
				shared_ptr<AbstractionStorage> modifies_storage = StorageManager::getAbstractionStorage(MODIFIES);
				shared_ptr<AbstractionStorage> uses_storage = StorageManager::getAbstractionStorage(USES);
			
				shared_ptr<StringMap> parent_db = parent_storage->getDatabase();
				shared_ptr<StringMap> parentstar_db = parentstar_storage->getDatabase();
				shared_ptr<StringMap> follows_db = follows_storage->getDatabase();
				shared_ptr<StringMap> followsstar_db = followsstar_storage->getDatabase();
				shared_ptr<StringMap> modifies_db = modifies_storage->getDatabase();
				shared_ptr<StringMap> uses_db = uses_storage->getDatabase();

				// assert database has correct entries
				Assert::IsTrue(compare_maps(*parent_db, parentData));
				Assert::IsTrue(compare_maps(*parentstar_db, parentStarData));
				Assert::IsTrue(compare_maps(*follows_db, followsData));
				Assert::IsTrue(compare_maps(*followsstar_db, followsStarData));
				Assert::IsTrue(compare_maps(*modifies_db, modifiesData));
				Assert::IsTrue(compare_maps(*uses_db, usesData));
			}
	};
}