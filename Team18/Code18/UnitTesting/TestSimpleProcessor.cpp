#include "stdafx.h"
#include "CppUnitTest.h"

#include "../source/SP/SimpleProcessor/SimpleProcessor.h"
#include "../source/SP/SimpleProcessor/ExpressionProcessor.h"
#include "../source/SP/SimpleProcessor/ProcessedReadStmt.h"
#include "../source/SP/SimpleProcessor/ProcessedPrintStmt.h"
#include "../source/SP/SimpleProcessor/ProcessedCallStmt.h"
#include "../source/SP/SimpleProcessor/ProcessedAssignStmt.h"
#include "../source/SP/SimpleProcessor/ProcessedIfStmt.h"
#include "../source/SP/SimpleProcessor/ProcessedWhileStmt.h"
#include "../source/SP/SimpleTokens/IdentifierToken.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace SimpleProcessor_Test
{
	TEST_CLASS(processProgram_Test) {
	private:
		bool processPrgTestWrapper(string input, shared_ptr<ProcessedProgram> expected) {
			shared_ptr<ProcessedProgram> result;
			try {
				result = SimpleProcessor::processProgram(input);
			}
			catch (invalid_argument e) {
				return false;
			}
			return ProcessedProgram::checkEquality(result, expected);
		}

		vector<shared_ptr<ProcessedProcedure>> dummy = { nullptr };
	public:
		TEST_METHOD(emptyProgram_failure) {
			string input = "";
			shared_ptr<ProcessedProgram> expected = make_shared<ProcessedProgram>(dummy);
			bool result = processPrgTestWrapper(input, expected);
			assert(result == false);
		}

		TEST_METHOD(codeOutsideOfProcedure_failure) {
			string input = "read x";
			shared_ptr<ProcessedProgram> expected = make_shared<ProcessedProgram>(dummy);
			bool result = processPrgTestWrapper(input, expected);
			assert(result == false);
		}

		TEST_METHOD(emptyProcedure_failure) {
			string input = "procedure x {}";
			shared_ptr<ProcessedProgram> expected = make_shared<ProcessedProgram>(dummy);
			bool result = processPrgTestWrapper(input, expected);
			assert(result == false);
		}

		TEST_METHOD(invalidProcedureName_failure_1) {
			string input = "procedure 1 {read x;}";
			shared_ptr<ProcessedProgram> expected = make_shared<ProcessedProgram>(dummy);
			bool result = processPrgTestWrapper(input, expected);
			assert(result == false);
		}

		TEST_METHOD(invalidProcedureName_failure_2) {
			string input = "procedure 0x1 {read x;}";
			shared_ptr<ProcessedProgram> expected = make_shared<ProcessedProgram>(dummy);
			bool result = processPrgTestWrapper(input, expected);
			assert(result == false);
		}

		TEST_METHOD(invalidProcedureName_failure_3) {
			string input = "procedure ' {read x;}";
			shared_ptr<ProcessedProgram> expected = make_shared<ProcessedProgram>(dummy);
			bool result = processPrgTestWrapper(input, expected);
			assert(result == false);
		}

		TEST_METHOD(invalidProcedureDeclaration_failure) {
			string input = "pr0cedure x {read x;}";
			shared_ptr<ProcessedProgram> expected = make_shared<ProcessedProgram>(dummy);
			bool result = processPrgTestWrapper(input, expected);
			assert(result == false);
		}

		TEST_METHOD(capitalisedProcedureDeclaration_failure) {
			string input = "Procedure x {read x;}";
			shared_ptr<ProcessedProgram> expected = make_shared<ProcessedProgram>(dummy);
			bool result = processPrgTestWrapper(input, expected);
			assert(result == false);
		}

		TEST_METHOD(missingProcName_failure) {
			string input = "  procedure    {read x;}";
			shared_ptr<ProcessedProgram> expected = make_shared<ProcessedProgram>(dummy);
			bool result = processPrgTestWrapper(input, expected);
			assert(result == false);
		}

		TEST_METHOD(missingProcedureKeyword_failure) {
			string input = "     procName   {read x;}";
			shared_ptr<ProcessedProgram> expected = make_shared<ProcessedProgram>(dummy);
			bool result = processPrgTestWrapper(input, expected);
			assert(result == false);
		}

		TEST_METHOD(validProcedureDeclaration_success) {
			string input = "procedure x {read x;}";

			// Expected
			vector<shared_ptr<ProcessedStmt>> stmtList = { make_shared<ProcessedReadStmt>(1, make_shared<IdentifierToken>("x")) };
			vector< shared_ptr<ProcessedProcedure>> proceduresVec = { make_shared<ProcessedProcedure>("x", stmtList) };
			shared_ptr<ProcessedProgram> expected = make_shared<ProcessedProgram>(proceduresVec);
			bool result = processPrgTestWrapper(input, expected);
			assert(result == true);
		}

		TEST_METHOD(validProcedureDeclarationWhitespaces_success) {
			string input = " \t\f\v\n\r\b \t\f\v\n\r\bprocedure \t\f\v\n\r\b \t\f\v\n\r\b"
				"x  \t\f\v\n\r\b{ \t\f\v\n\r\bread \t\f\v\n\r\b  \t\f\v\n\r\bx \t\f\v\n\r\b; \t\f\v\n\r\b} \t\f\v\n\r\b";

			// Expected
			vector<shared_ptr<ProcessedStmt>> stmtList = { make_shared<ProcessedReadStmt>(1, make_shared<IdentifierToken>("x")) };
			vector< shared_ptr<ProcessedProcedure>> proceduresVec = { make_shared<ProcessedProcedure>("x", stmtList) };
			shared_ptr<ProcessedProgram> expected = make_shared<ProcessedProgram>(proceduresVec);
			bool result = processPrgTestWrapper(input, expected);
			assert(result == true);
		}

		TEST_METHOD(emptyStatement_failure_1) {
			string input = "procedure x {}";
			shared_ptr<ProcessedProgram> expected = make_shared<ProcessedProgram>(dummy);
			bool result = processPrgTestWrapper(input, expected);
			assert(result == false);
		}

		TEST_METHOD(emptyStatement_failure_2) {
			string input = "procedure x { ;}";
			shared_ptr<ProcessedProgram> expected = make_shared<ProcessedProgram>(dummy);
			bool result = processPrgTestWrapper(input, expected);
			assert(result == false);
		}

		TEST_METHOD(emptyStatement_failure_3) {
			string input = "procedure x { ;;;;}";
			shared_ptr<ProcessedProgram> expected = make_shared<ProcessedProgram>(dummy);
			bool result = processPrgTestWrapper(input, expected);
			assert(result == false);
		}

		TEST_METHOD(invalidStatement_failure_1) {
			string input = "procedure x {set y = 1; read z;}";
			shared_ptr<ProcessedProgram> expected = make_shared<ProcessedProgram>(dummy);
			bool result = processPrgTestWrapper(input, expected);
			assert(result == false);
		}

		TEST_METHOD(invalidStatement_failure_2) {
			string input = "procedure x {store y; print z;}";
			shared_ptr<ProcessedProgram> expected = make_shared<ProcessedProgram>(dummy);
			bool result = processPrgTestWrapper(input, expected);
			assert(result == false);
		}

		TEST_METHOD(invalidStatement_failure_3) {
			string input = "procedure x {allocate here y; deallocate there z;}";
			shared_ptr<ProcessedProgram> expected = make_shared<ProcessedProgram>(dummy);
			bool result = processPrgTestWrapper(input, expected);
			assert(result == false);
		}

		TEST_METHOD(invalidStatement_failure_4) {
			string input = "procedure x {read and store y; store and read z;}";
			shared_ptr<ProcessedProgram> expected = make_shared<ProcessedProgram>(dummy);
			bool result = processPrgTestWrapper(input, expected);
			assert(result == false);
		}

		TEST_METHOD(invalidNames_failure_1) {
			string input = "procedure x {read 0y;}";
			shared_ptr<ProcessedProgram> expected = make_shared<ProcessedProgram>(dummy);
			bool result = processPrgTestWrapper(input, expected);
			assert(result == false);
		}

		TEST_METHOD(invalidNames_failure_2) {
			string input = "procedure x {read 10;}";
			shared_ptr<ProcessedProgram> expected = make_shared<ProcessedProgram>(dummy);
			bool result = processPrgTestWrapper(input, expected);
			assert(result == false);
		}

		TEST_METHOD(invalidNames_failure_3) {
			string input = "procedure x {read ';}";
			shared_ptr<ProcessedProgram> expected = make_shared<ProcessedProgram>(dummy);
			bool result = processPrgTestWrapper(input, expected);
			assert(result == false);
		}

		TEST_METHOD(validSemicolonStatements_success) {
			string input = "procedure x {read a; print b; call c; d = a + (b - c) * d / e % f;}";

			// Expected
			ExpressionProcessor ep4 = ExpressionProcessor(4);
			shared_ptr<Node> RHS = ep4.nodifyArithmeticExpression("a + (b - c) * d / e % f");
			vector<shared_ptr<ProcessedStmt>> stmtlist = {
				make_shared<ProcessedReadStmt>(1, make_shared<IdentifierToken>("a")),
				make_shared<ProcessedPrintStmt>(2, make_shared<IdentifierToken>("b")),
				make_shared<ProcessedCallStmt>(3, make_shared<IdentifierToken>("c")),
				make_shared<ProcessedAssignStmt>(4, make_shared<IdentifierToken>("d"), RHS)
			};
			vector< shared_ptr<ProcessedProcedure>> proceduresVec = { make_shared<ProcessedProcedure>("x", stmtlist) };
			shared_ptr<ProcessedProgram> expected = make_shared<ProcessedProgram>(proceduresVec);
			bool result = processPrgTestWrapper(input, expected);
			assert(result == true);
		}

		TEST_METHOD(validIfWhileStatements_success) {
			string input = "procedure procedure {if (a==b) then {read x;} else {print y;} while ((!(a > b)) || (c < d)) {call z;}}";

			// Expected
			ExpressionProcessor ep1 = ExpressionProcessor(1);
			shared_ptr<Node> ifCond = ep1.nodifyConditionalExpression("a==b");
			vector<shared_ptr<ProcessedStmt>> thenBlockVec = {
				make_shared<ProcessedReadStmt>(2, make_shared<IdentifierToken>("x"))
			};
			shared_ptr<ProcessedStmtList> thenBlock = make_shared<ProcessedStmtList>(thenBlockVec);
			vector<shared_ptr<ProcessedStmt>> elseBlockVec = {
				make_shared<ProcessedPrintStmt>(3, make_shared<IdentifierToken>("y"))

			};
			shared_ptr<ProcessedStmtList> elseBlock = make_shared<ProcessedStmtList>(elseBlockVec);

			ExpressionProcessor ep4 = ExpressionProcessor(4);
			shared_ptr<Node> whileCond = ep4.nodifyConditionalExpression("(!(a > b)) || (c < d)");
			vector<shared_ptr<ProcessedStmt>> whileBlockVec = {
				make_shared<ProcessedCallStmt>(5, make_shared<IdentifierToken>("z"))
			};
			shared_ptr<ProcessedStmtList> whileBlock = make_shared<ProcessedStmtList>(whileBlockVec);

			vector<shared_ptr<ProcessedStmt>> stmtlist = {
				make_shared<ProcessedIfStmt>(1, ifCond, thenBlock, elseBlock),
				make_shared<ProcessedWhileStmt>(4, whileCond, whileBlock)
			};

			vector< shared_ptr<ProcessedProcedure>> proceduresVec = { make_shared<ProcessedProcedure>("procedure", stmtlist) };
			shared_ptr<ProcessedProgram> expected = make_shared<ProcessedProgram>(proceduresVec);
			bool result = processPrgTestWrapper(input, expected);
			assert(result == true);
		}

		TEST_METHOD(validComplicatedConditionExpr_success) {
			string complicatedExpr = "(3 + ((6 - 3) % (8 * (4 + 5))))";
			string complicatedConditionExpr = "((!(" + complicatedExpr + " > b)) && ((c <= d) || (e != 3))) && (((g < 5) || (i >= j)) && (a == 5))";

			string input = "procedure procedure {if (" + complicatedConditionExpr + ") then{read x; } else { print y; } }";

			// Expected
			ExpressionProcessor ep1 = ExpressionProcessor(1);
			shared_ptr<Node> ifCond = ep1.nodifyConditionalExpression(complicatedConditionExpr);
			vector<shared_ptr<ProcessedStmt>> thenBlockVec = {
				make_shared<ProcessedReadStmt>(2, make_shared<IdentifierToken>("x"))
			};
			shared_ptr<ProcessedStmtList> thenBlock = make_shared<ProcessedStmtList>(thenBlockVec);
			vector<shared_ptr<ProcessedStmt>> elseBlockVec = {
				make_shared<ProcessedPrintStmt>(3, make_shared<IdentifierToken>("y"))

			};
			shared_ptr<ProcessedStmtList> elseBlock = make_shared<ProcessedStmtList>(elseBlockVec);

			vector<shared_ptr<ProcessedStmt>> stmtlist = {
				make_shared<ProcessedIfStmt>(1, ifCond, thenBlock, elseBlock)
			};

			vector< shared_ptr<ProcessedProcedure>> proceduresVec = { make_shared<ProcessedProcedure>("procedure", stmtlist) };
			shared_ptr<ProcessedProgram> expected = make_shared<ProcessedProgram>(proceduresVec);
			bool result = processPrgTestWrapper(input, expected);
			assert(result == true);
		}

		TEST_METHOD(combinationStatements_success) {
			string input = "procedure x {read a; if (a==b) then {read x;} else {print y;} print b; while (c<1) {d = d + 1;} call c;}";

			// Expected
			ExpressionProcessor ep2 = ExpressionProcessor(2);
			shared_ptr<Node> ifCond = ep2.nodifyConditionalExpression("a==b");
			vector<shared_ptr<ProcessedStmt>> thenBlockVec = {
				make_shared<ProcessedReadStmt>(3, make_shared<IdentifierToken>("x"))
			};
			shared_ptr<ProcessedStmtList> thenBlock = make_shared<ProcessedStmtList>(thenBlockVec);
			vector<shared_ptr<ProcessedStmt>> elseBlockVec = {
				make_shared<ProcessedPrintStmt>(4, make_shared<IdentifierToken>("y"))
			};
			shared_ptr<ProcessedStmtList> elseBlock = make_shared<ProcessedStmtList>(elseBlockVec);

			ExpressionProcessor ep6 = ExpressionProcessor(6);
			shared_ptr<Node> whileCond = ep6.nodifyConditionalExpression("c<1");
			ExpressionProcessor ep7 = ExpressionProcessor(7);
			vector<shared_ptr<ProcessedStmt>> whileBlockVec = {
				make_shared<ProcessedAssignStmt>(7, make_shared<IdentifierToken>("d"), ep7.nodifyArithmeticExpression("d + 1"))
			};
			shared_ptr<ProcessedStmtList> whileBlock = make_shared<ProcessedStmtList>(whileBlockVec);
			
			vector<shared_ptr<ProcessedStmt>> stmtlist = {
				make_shared<ProcessedReadStmt>(1, make_shared<IdentifierToken>("a")),
				make_shared<ProcessedIfStmt>(2, ifCond, thenBlock, elseBlock),
				make_shared<ProcessedPrintStmt>(5, make_shared<IdentifierToken>("b")),
				make_shared<ProcessedWhileStmt>(6, whileCond, whileBlock),
				make_shared<ProcessedCallStmt>(8, make_shared<IdentifierToken>("c"))
			};
			vector< shared_ptr<ProcessedProcedure>> proceduresVec = { make_shared<ProcessedProcedure>("x", stmtlist) };
			shared_ptr<ProcessedProgram> expected = make_shared<ProcessedProgram>(proceduresVec);
			bool result = processPrgTestWrapper(input, expected);
			assert(result == true);
		}

		TEST_METHOD(nestedStatements_success) {
			string input = "procedure x {while (a>b) {while(c<d) {read x;}}}";

			// Expected
			ExpressionProcessor ep1 = ExpressionProcessor(1);
			ExpressionProcessor ep2 = ExpressionProcessor(2);
			shared_ptr<Node> while1Condition = ep1.nodifyConditionalExpression("a>b");
			shared_ptr<Node> while2Condition = ep2.nodifyConditionalExpression("c<d");

			vector<shared_ptr<ProcessedStmt>> while2Contents = {
				make_shared<ProcessedReadStmt>(3, make_shared<IdentifierToken>("x"))
			};
			shared_ptr<ProcessedStmtList> while2Block = make_shared<ProcessedStmtList>(while2Contents);

			vector<shared_ptr<ProcessedStmt>> while1BlockVec = {
				make_shared<ProcessedWhileStmt>(2, while2Condition, while2Block)
			};
			shared_ptr<ProcessedStmtList> while1Block = make_shared<ProcessedStmtList>(while1BlockVec);

			vector<shared_ptr<ProcessedStmt>> stmtlist = {
				make_shared<ProcessedWhileStmt>(1, while1Condition, while1Block)
			};
			vector< shared_ptr<ProcessedProcedure>> proceduresVec = { make_shared<ProcessedProcedure>("x", stmtlist) };
			shared_ptr<ProcessedProgram> expected = make_shared<ProcessedProgram>(proceduresVec);
			bool result = processPrgTestWrapper(input, expected);
			assert(result == true);
		}

		TEST_METHOD(multipleDeclaration_success) {
			string input = "procedure x {read z;} procedure a {print b;}";

			// Expected
			vector<shared_ptr<ProcessedStmt>> proc1StmtList = { make_shared<ProcessedReadStmt>(1, make_shared<IdentifierToken>("z"))};

			vector<shared_ptr<ProcessedStmt>> proc2StmtList = { make_shared<ProcessedPrintStmt>(2, make_shared<IdentifierToken>("b")) };

			vector< shared_ptr<ProcessedProcedure>> proceduresVec = { 
				make_shared<ProcessedProcedure>("x", proc1StmtList),
				make_shared<ProcessedProcedure>("a", proc2StmtList)
			};
			shared_ptr<ProcessedProgram> expected = make_shared<ProcessedProgram>(proceduresVec);
			bool result = processPrgTestWrapper(input, expected);
			assert(result == true);
		}
	};
}
