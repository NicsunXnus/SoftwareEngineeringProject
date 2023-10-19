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

namespace SimpleProcessor_Test
{
	TEST_CLASS(processProgram_Test) {
	private:
		bool processPrgTestWrapper(std::string input, ProcessedProgram expected) {
			std::shared_ptr<ProcessedProgram> result;
			try {
				result = SimpleProcessor::processProgram(input);
			}
			catch (std::invalid_argument e) {
				return false;
			}
			return result->equalsTo(expected);
		}

		std::vector<std::shared_ptr<ProcessedProcedure>> dummy = { nullptr };
	public:
		TEST_METHOD(emptyProgram_failure) {
			std::string input = "";
			ProcessedProgram expected = ProcessedProgram(dummy);
			bool result = processPrgTestWrapper(input, expected);
			assert(result == false);
		}

		TEST_METHOD(codeOutsideOfProcedure_failure) {
			std::string input = "read x";
			ProcessedProgram expected = ProcessedProgram(dummy);
			bool result = processPrgTestWrapper(input, expected);
			assert(result == false);
		}

		TEST_METHOD(emptyProcedure_failure) {
			std::string input = "procedure x {}";
			ProcessedProgram expected = ProcessedProgram(dummy);
			bool result = processPrgTestWrapper(input, expected);
			assert(result == false);
		}

		TEST_METHOD(invalidProcedureName_failure_1) {
			std::string input = "procedure 1 {read x;}";
			ProcessedProgram expected = ProcessedProgram(dummy);
			bool result = processPrgTestWrapper(input, expected);
			assert(result == false);
		}

		TEST_METHOD(invalidProcedureName_failure_2) {
			std::string input = "procedure 0x1 {read x;}";
			ProcessedProgram expected = ProcessedProgram(dummy);
			bool result = processPrgTestWrapper(input, expected);
			assert(result == false);
		}

		TEST_METHOD(invalidProcedureName_failure_3) {
			std::string input = "procedure ' {read x;}";
			ProcessedProgram expected = ProcessedProgram(dummy);
			bool result = processPrgTestWrapper(input, expected);
			assert(result == false);
		}

		TEST_METHOD(invalidProcedureDeclaration_failure) {
			std::string input = "pr0cedure x {read x;}";
			ProcessedProgram expected = ProcessedProgram(dummy);
			bool result = processPrgTestWrapper(input, expected);
			assert(result == false);
		}

		TEST_METHOD(capitalisedProcedureDeclaration_failure) {
			std::string input = "Procedure x {read x;}";
			ProcessedProgram expected = ProcessedProgram(dummy);
			bool result = processPrgTestWrapper(input, expected);
			assert(result == false);
		}

		TEST_METHOD(missingProcName_failure) {
			std::string input = "  procedure    {read x;}";
			ProcessedProgram expected = ProcessedProgram(dummy);
			bool result = processPrgTestWrapper(input, expected);
			assert(result == false);
		}

		TEST_METHOD(missingProcedureKeyword_failure) {
			std::string input = "     procName   {read x;}";
			ProcessedProgram expected = ProcessedProgram(dummy);
			bool result = processPrgTestWrapper(input, expected);
			assert(result == false);
		}

		TEST_METHOD(validProcedureDeclaration_success) {
			std::string input = "procedure x {read x;}";
			std::shared_ptr<ProcessedProgram> output = SimpleProcessor::processProgram(input);

			// Expected
			std::vector<std::shared_ptr<ProcessedStmt>> stmtList = { std::make_shared<ProcessedReadStmt>(1, std::make_shared<IdentifierToken>("x")) };
			std::vector< std::shared_ptr<ProcessedProcedure>> proceduresVec = { std::make_shared<ProcessedProcedure>("x", stmtList) };
			ProcessedProgram program = ProcessedProgram(proceduresVec);
			assert((*output).equalsTo(program));
		}

		TEST_METHOD(validProcedureDeclarationWhitespaces_success) {
			std::string input = " \t\f\v\n\r\b \t\f\v\n\r\bprocedure \t\f\v\n\r\b \t\f\v\n\r\b"
				"x  \t\f\v\n\r\b{ \t\f\v\n\r\bread \t\f\v\n\r\b  \t\f\v\n\r\bx \t\f\v\n\r\b; \t\f\v\n\r\b} \t\f\v\n\r\b";
			std::shared_ptr<ProcessedProgram> output = SimpleProcessor::processProgram(input);

			// Expected
			std::vector<std::shared_ptr<ProcessedStmt>> stmtList = { std::make_shared<ProcessedReadStmt>(1, std::make_shared<IdentifierToken>("x")) };
			std::vector< std::shared_ptr<ProcessedProcedure>> proceduresVec = { std::make_shared<ProcessedProcedure>("x", stmtList) };
			ProcessedProgram program = ProcessedProgram(proceduresVec);
			assert((*output).equalsTo(program));
		}

		TEST_METHOD(emptyStatement_failure_1) {
			std::string input = "procedure x {}";
			ProcessedProgram expected = ProcessedProgram(dummy);
			bool result = processPrgTestWrapper(input, expected);
			assert(result == false);
		}

		TEST_METHOD(emptyStatement_failure_2) {
			std::string input = "procedure x { ;}";
			ProcessedProgram expected = ProcessedProgram(dummy);
			bool result = processPrgTestWrapper(input, expected);
			assert(result == false);
		}

		TEST_METHOD(emptyStatement_failure_3) {
			std::string input = "procedure x { ;;;;}";
			ProcessedProgram expected = ProcessedProgram(dummy);
			bool result = processPrgTestWrapper(input, expected);
			assert(result == false);
		}

		TEST_METHOD(invalidStatement_failure_1) {
			std::string input = "procedure x {set y = 1; read z;}";
			ProcessedProgram expected = ProcessedProgram(dummy);
			bool result = processPrgTestWrapper(input, expected);
			assert(result == false);
		}

		TEST_METHOD(invalidStatement_failure_2) {
			std::string input = "procedure x {store y; print z;}";
			ProcessedProgram expected = ProcessedProgram(dummy);
			bool result = processPrgTestWrapper(input, expected);
			assert(result == false);
		}

		TEST_METHOD(invalidStatement_failure_3) {
			std::string input = "procedure x {allocate here y; deallocate there z;}";
			ProcessedProgram expected = ProcessedProgram(dummy);
			bool result = processPrgTestWrapper(input, expected);
			assert(result == false);
		}

		TEST_METHOD(invalidStatement_failure_4) {
			std::string input = "procedure x {read and store y; store and read z;}";
			ProcessedProgram expected = ProcessedProgram(dummy);
			bool result = processPrgTestWrapper(input, expected);
			assert(result == false);
		}

		TEST_METHOD(invalidNames_failure_1) {
			std::string input = "procedure x {read 0y;}";
			ProcessedProgram expected = ProcessedProgram(dummy);
			bool result = processPrgTestWrapper(input, expected);
			assert(result == false);
		}

		TEST_METHOD(invalidNames_failure_2) {
			std::string input = "procedure x {read 10;}";
			ProcessedProgram expected = ProcessedProgram(dummy);
			bool result = processPrgTestWrapper(input, expected);
			assert(result == false);
		}

		TEST_METHOD(invalidNames_failure_3) {
			std::string input = "procedure x {read ';}";
			ProcessedProgram expected = ProcessedProgram(dummy);
			bool result = processPrgTestWrapper(input, expected);
			assert(result == false);
		}

		TEST_METHOD(validSemicolonStatements_success) {
			std::string input = "procedure x {read a; print b; call c; d = a + (b - c) * d / e % f;}";
			std::shared_ptr<ProcessedProgram> output = SimpleProcessor::processProgram(input);

			// Expected
			ExpressionProcessor ep4 = ExpressionProcessor(4);
			std::shared_ptr<Node> RHS = ep4.nodifyArithmeticExpression("a + (b - c) * d / e % f");
			std::vector<std::shared_ptr<ProcessedStmt>> stmtlist = {
				std::make_shared<ProcessedReadStmt>(1, std::make_shared<IdentifierToken>("a")),
				std::make_shared<ProcessedPrintStmt>(2, std::make_shared<IdentifierToken>("b")),
				std::make_shared<ProcessedCallStmt>(3, std::make_shared<IdentifierToken>("c")),
				std::make_shared<ProcessedAssignStmt>(4, std::make_shared<IdentifierToken>("d"), RHS)
			};
			std::vector< std::shared_ptr<ProcessedProcedure>> proceduresVec = { std::make_shared<ProcessedProcedure>("x", stmtlist) };
			ProcessedProgram program = ProcessedProgram(proceduresVec);
			assert((*output).equalsTo(program));
		}

		TEST_METHOD(validIfWhileStatements_success) {
			std::string input = "procedure procedure {if (a==b) then {read x;} else {print y;} while ((!(a > b)) || (c < d)) {call z;}}";
			std::shared_ptr<ProcessedProgram> output = SimpleProcessor::processProgram(input);

			// Expected
			ExpressionProcessor ep1 = ExpressionProcessor(1);
			std::shared_ptr<Node> ifCond = ep1.nodifyConditionalExpression("a==b");
			std::vector<std::shared_ptr<ProcessedStmt>> thenBlockVec = {
				std::make_shared<ProcessedReadStmt>(2, std::make_shared<IdentifierToken>("x"))
			};
			std::shared_ptr<ProcessedStmtList> thenBlock = std::make_shared<ProcessedStmtList>(thenBlockVec);
			std::vector<std::shared_ptr<ProcessedStmt>> elseBlockVec = {
				std::make_shared<ProcessedPrintStmt>(3, std::make_shared<IdentifierToken>("y"))

			};
			std::shared_ptr<ProcessedStmtList> elseBlock = std::make_shared<ProcessedStmtList>(elseBlockVec);

			ExpressionProcessor ep4 = ExpressionProcessor(4);
			std::shared_ptr<Node> whileCond = ep4.nodifyConditionalExpression("(!(a > b)) || (c < d)");
			std::vector<std::shared_ptr<ProcessedStmt>> whileBlockVec = {
				std::make_shared<ProcessedCallStmt>(5, std::make_shared<IdentifierToken>("z"))
			};
			std::shared_ptr<ProcessedStmtList> whileBlock = std::make_shared<ProcessedStmtList>(whileBlockVec);

			std::vector<std::shared_ptr<ProcessedStmt>> stmtlist = {
				std::make_shared<ProcessedIfStmt>(1, ifCond, thenBlock, elseBlock),
				std::make_shared<ProcessedWhileStmt>(4, whileCond, whileBlock)
			};

			std::vector< std::shared_ptr<ProcessedProcedure>> proceduresVec = { std::make_shared<ProcessedProcedure>("procedure", stmtlist) };
			ProcessedProgram program = ProcessedProgram(proceduresVec);
			assert((*output).equalsTo(program));
		}

		TEST_METHOD(validComplicatedConditionExpr_success) {
			std::string complicatedExpr = "(3 + ((6 - 3) % (8 * (4 + 5))))";
			std::string complicatedConditionExpr = "((!(" + complicatedExpr + " > b)) && ((c <= d) || (e != 3))) && (((g < 5) || (i >= j)) && (a == 5))";

			std::string input = "procedure procedure {if (" + complicatedConditionExpr + ") then{read x; } else { print y; } }";
			std::shared_ptr<ProcessedProgram> output = SimpleProcessor::processProgram(input);

			// Expected
			ExpressionProcessor ep1 = ExpressionProcessor(1);
			std::shared_ptr<Node> ifCond = ep1.nodifyConditionalExpression(complicatedConditionExpr);
			std::vector<std::shared_ptr<ProcessedStmt>> thenBlockVec = {
				std::make_shared<ProcessedReadStmt>(2, std::make_shared<IdentifierToken>("x"))
			};
			std::shared_ptr<ProcessedStmtList> thenBlock = std::make_shared<ProcessedStmtList>(thenBlockVec);
			std::vector<std::shared_ptr<ProcessedStmt>> elseBlockVec = {
				std::make_shared<ProcessedPrintStmt>(3, std::make_shared<IdentifierToken>("y"))

			};
			std::shared_ptr<ProcessedStmtList> elseBlock = std::make_shared<ProcessedStmtList>(elseBlockVec);

			std::vector<std::shared_ptr<ProcessedStmt>> stmtlist = {
				std::make_shared<ProcessedIfStmt>(1, ifCond, thenBlock, elseBlock)
			};

			std::vector< std::shared_ptr<ProcessedProcedure>> proceduresVec = { std::make_shared<ProcessedProcedure>("procedure", stmtlist) };
			ProcessedProgram program = ProcessedProgram(proceduresVec);
			assert((*output).equalsTo(program));
		}

		TEST_METHOD(combinationStatements_success) {
			std::string input = "procedure x {read a; if (a==b) then {read x;} else {print y;} print b; while (c<1) {d = d + 1;} call c;}";
			std::shared_ptr<ProcessedProgram> output = SimpleProcessor::processProgram(input);

			// Expected
			ExpressionProcessor ep2 = ExpressionProcessor(2);
			std::shared_ptr<Node> ifCond = ep2.nodifyConditionalExpression("a==b");
			std::vector<std::shared_ptr<ProcessedStmt>> thenBlockVec = {
				std::make_shared<ProcessedReadStmt>(3, std::make_shared<IdentifierToken>("x"))
			};
			std::shared_ptr<ProcessedStmtList> thenBlock = std::make_shared<ProcessedStmtList>(thenBlockVec);
			std::vector<std::shared_ptr<ProcessedStmt>> elseBlockVec = {
				std::make_shared<ProcessedPrintStmt>(4, std::make_shared<IdentifierToken>("y"))
			};
			std::shared_ptr<ProcessedStmtList> elseBlock = std::make_shared<ProcessedStmtList>(elseBlockVec);

			ExpressionProcessor ep6 = ExpressionProcessor(6);
			std::shared_ptr<Node> whileCond = ep6.nodifyConditionalExpression("c<1");
			ExpressionProcessor ep7 = ExpressionProcessor(7);
			std::vector<std::shared_ptr<ProcessedStmt>> whileBlockVec = {
				std::make_shared<ProcessedAssignStmt>(7, std::make_shared<IdentifierToken>("d"), ep7.nodifyArithmeticExpression("d + 1"))
			};
			std::shared_ptr<ProcessedStmtList> whileBlock = std::make_shared<ProcessedStmtList>(whileBlockVec);
			
			std::vector<std::shared_ptr<ProcessedStmt>> stmtlist = {
				std::make_shared<ProcessedReadStmt>(1, std::make_shared<IdentifierToken>("a")),
				std::make_shared<ProcessedIfStmt>(2, ifCond, thenBlock, elseBlock),
				std::make_shared<ProcessedPrintStmt>(5, std::make_shared<IdentifierToken>("b")),
				std::make_shared<ProcessedWhileStmt>(6, whileCond, whileBlock),
				std::make_shared<ProcessedCallStmt>(8, std::make_shared<IdentifierToken>("c"))
			};
			std::vector< std::shared_ptr<ProcessedProcedure>> proceduresVec = { std::make_shared<ProcessedProcedure>("x", stmtlist) };
			ProcessedProgram program = ProcessedProgram(proceduresVec);
			assert((*output).equalsTo(program));
		}

		TEST_METHOD(nestedStatements_success) {
			std::string input = "procedure x {while (a>b) {while(c<d) {read x;}}}";
			std::shared_ptr<ProcessedProgram> output = SimpleProcessor::processProgram(input);

			// Expected
			ExpressionProcessor ep1 = ExpressionProcessor(1);
			ExpressionProcessor ep2 = ExpressionProcessor(2);
			std::shared_ptr<Node> while1Condition = ep1.nodifyConditionalExpression("a>b");
			std::shared_ptr<Node> while2Condition = ep2.nodifyConditionalExpression("c<d");

			std::vector<std::shared_ptr<ProcessedStmt>> while2Contents = {
				std::make_shared<ProcessedReadStmt>(3, std::make_shared<IdentifierToken>("x"))
			};
			std::shared_ptr<ProcessedStmtList> while2Block = std::make_shared<ProcessedStmtList>(while2Contents);

			std::vector<std::shared_ptr<ProcessedStmt>> while1BlockVec = {
				std::make_shared<ProcessedWhileStmt>(2, while2Condition, while2Block)
			};
			std::shared_ptr<ProcessedStmtList> while1Block = std::make_shared<ProcessedStmtList>(while1BlockVec);

			std::vector<std::shared_ptr<ProcessedStmt>> stmtlist = {
				std::make_shared<ProcessedWhileStmt>(1, while1Condition, while1Block)
			};
			std::vector< std::shared_ptr<ProcessedProcedure>> proceduresVec = { std::make_shared<ProcessedProcedure>("x", stmtlist) };
			ProcessedProgram program = ProcessedProgram(proceduresVec);
			assert((*output).equalsTo(program));
		}

		TEST_METHOD(multipleDeclaration_success) {
			std::string input = "procedure x {read z;} procedure a {print b;}";
			std::shared_ptr<ProcessedProgram> output = SimpleProcessor::processProgram(input);

			// Expected
			std::vector<std::shared_ptr<ProcessedStmt>> proc1StmtList = { std::make_shared<ProcessedReadStmt>(1, std::make_shared<IdentifierToken>("z"))};

			std::vector<std::shared_ptr<ProcessedStmt>> proc2StmtList = { std::make_shared<ProcessedPrintStmt>(2, std::make_shared<IdentifierToken>("b")) };

			std::vector< std::shared_ptr<ProcessedProcedure>> proceduresVec = { 
				std::make_shared<ProcessedProcedure>("x", proc1StmtList),
				std::make_shared<ProcessedProcedure>("a", proc2StmtList)
			};
			ProcessedProgram program = ProcessedProgram(proceduresVec);
			assert((*output).equalsTo(program));
		}
	};
}
