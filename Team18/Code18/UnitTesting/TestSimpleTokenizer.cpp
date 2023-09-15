#include "stdafx.h"
#include "CppUnitTest.h"

#include "../source/TokenizerClasses/SimpleTokenizer.h"
#include "../source/ExceptionMessages.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace SimpleTokeniser_Test
{
	TEST_CLASS(tokeniseMethod_Test) {
	public:
		TEST_METHOD(emptyProgram_failure) {
			try {
				std::string input = "";
				SimpleTokenizer::tokenizeProgram(input);
				assert(false);
			}
			catch (std::invalid_argument e) {
				assert(e.what() == ExceptionMessages::emptyProgramGiven);
			}
		}

		TEST_METHOD(codeOutsideOfProcedure_failure) {
			try {
				std::string input = "read x";
				SimpleTokenizer::tokenizeProgram(input);
				assert(false);
			}
			catch (std::invalid_argument e) {
				assert(e.what() == ExceptionMessages::invalidProgramDefinition);
			}
		}

		TEST_METHOD(emptyProcedure_failure) {
			try {
				std::string input = "procedure x {}";
				SimpleTokenizer::tokenizeProgram(input);
				assert(false);
			}
			catch (std::invalid_argument e) {
				assert(e.what() == ExceptionMessages::emptyStatementListGiven);
			}
		}

		TEST_METHOD(invalidProcedureName_failure) {
			try {
				std::string input = "procedure 1 {read x;}";
				SimpleTokenizer::tokenizeProgram(input);
				assert(false);
			}
			catch (std::invalid_argument e) {
				assert(e.what() == ExceptionMessages::invalidProcedureDefinition);
			}
			try {
				std::string input = "procedure 0x1 {read x;}";
				SimpleTokenizer::tokenizeProgram(input);
				assert(false);
			}
			catch (std::invalid_argument e) {
				assert(e.what() == ExceptionMessages::invalidProcedureDefinition);
			}
			try {
				std::string input = "procedure ' {read x;}";
				SimpleTokenizer::tokenizeProgram(input);
				assert(false);
			}
			catch (std::invalid_argument e) {
				assert(e.what() == ExceptionMessages::invalidProcedureDefinition);
			}
		}

		TEST_METHOD(invalidProcedureDeclaration_failure) {
			try {
				std::string input = "pr0cedure x {read x;}";
				SimpleTokenizer::tokenizeProgram(input);
				assert(false);
			}
			catch (std::invalid_argument e) {
				assert(e.what() == ExceptionMessages::invalidProcedureDefinition);
			}
			try {
				std::string input = "Procedure x {read x;}"; // capitals
				SimpleTokenizer::tokenizeProgram(input);
				assert(false);
			}
			catch (std::invalid_argument e) {
				assert(e.what() == ExceptionMessages::invalidProcedureDefinition);
			}
			try {
				std::string input = "procedure    {read x;}";
				SimpleTokenizer::tokenizeProgram(input);
				assert(false);
			}
			catch (std::invalid_argument e) {
				assert(e.what() == ExceptionMessages::invalidProcedureDefinition);
			}
			try {
				std::string input = "     procName   {read x;}";
				SimpleTokenizer::tokenizeProgram(input);
				assert(false);
			}
			catch (std::invalid_argument e) {
				assert(e.what() == ExceptionMessages::invalidProcedureDefinition);
			}
		}

		TEST_METHOD(validProcedureDeclaration_success) {
			std::string input = "procedure x {read x;}";
			std::shared_ptr<TokenizedProgram> output = SimpleTokenizer::tokenizeProgram(input);

			// Expected
			std::vector<std::shared_ptr<Token>> line1vec = { std::make_shared<ReadKeywordToken>(), std::make_shared<IdentifierToken>("x") };
			std::vector<std::shared_ptr<TokenizedStmt>> stmtlistVec = { std::make_shared<TokenizedSemicolonStmt>(1, line1vec) };
			std::shared_ptr<TokenizedStmtList> stmtList = std::make_shared<TokenizedStmtList>(stmtlistVec);
			std::vector< std::shared_ptr<TokenizedProcedure>> proceduresVec = { std::make_shared<TokenizedProcedure>("x", stmtList) };
			TokenizedProgram program = TokenizedProgram(proceduresVec);
			assert((*output).equalsTo(program));
		}

		TEST_METHOD(validProcedureDeclarationWhitespaces_success) {
			std::string input = " \t\f\v\n\r\b \t\f\v\n\r\bprocedure \t\f\v\n\r\b \t\f\v\n\r\b"
				"x  \t\f\v\n\r\b{ \t\f\v\n\r\bread \t\f\v\n\r\b  \t\f\v\n\r\bx \t\f\v\n\r\b; \t\f\v\n\r\b} \t\f\v\n\r\b";
			std::shared_ptr<TokenizedProgram> output = SimpleTokenizer::tokenizeProgram(input);

			// Expected
			std::vector<std::shared_ptr<Token>> line1vec = { std::make_shared<ReadKeywordToken>(), std::make_shared<IdentifierToken>("x") };
			std::vector<std::shared_ptr<TokenizedStmt>> stmtlistVec = { std::make_shared<TokenizedSemicolonStmt>(1, line1vec) };
			std::shared_ptr<TokenizedStmtList> stmtList = std::make_shared<TokenizedStmtList>(stmtlistVec);
			std::vector< std::shared_ptr<TokenizedProcedure>> proceduresVec = { std::make_shared<TokenizedProcedure>("x", stmtList) };
			TokenizedProgram program = TokenizedProgram(proceduresVec);
			assert((*output).equalsTo(program));
		}

		TEST_METHOD(emptyStatement_failure) {
			try {
				std::string input = "procedure x {}";
				SimpleTokenizer::tokenizeProgram(input);
				assert(false);
			}
			catch (std::invalid_argument e) {
				assert(e.what() == ExceptionMessages::emptyStatementListGiven);
			}
			try {
				std::string input = "procedure x { ;}";
				SimpleTokenizer::tokenizeProgram(input);
				assert(false);
			}
			catch (std::invalid_argument e) {
				assert(e.what() == ExceptionMessages::emptyStatementListGiven);
			}
			try {
				std::string input = "procedure x { ;;;;}";
				SimpleTokenizer::tokenizeProgram(input);
				assert(false);
			}
			catch (std::invalid_argument e) {
				assert(e.what() == ExceptionMessages::emptyStatementListGiven);
			}
		}

		TEST_METHOD(invalidStatement_failure) {
			try {
				std::string input = "procedure x {set y = 1; read z;}";
				SimpleTokenizer::tokenizeProgram(input);
				assert(false);
			}
			catch (std::invalid_argument e) {
				assert(e.what() == ExceptionMessages::invalidIdentifier);
			}
			try {
				std::string input = "procedure x {store y; print z;}";
				SimpleTokenizer::tokenizeProgram(input);
				assert(false);
			}
			catch (std::invalid_argument e) {
				assert(e.what() == ExceptionMessages::invalidSemicolonStmt);
			}
			try {
				std::string input = "procedure x {allocate here y; deallocate there z;}";
				SimpleTokenizer::tokenizeProgram(input);
				assert(false);
			}
			catch (std::invalid_argument e) {
				assert(e.what() == ExceptionMessages::invalidSemicolonStmt);
			}
			try {
				std::string input = "procedure x {read and store y; store and read z;}";
				SimpleTokenizer::tokenizeProgram(input);
				assert(false);
			}
			catch (std::invalid_argument e) {
				assert(e.what() == ExceptionMessages::invalidSemicolonStmt);
			}
		}

		TEST_METHOD(invalidNames_failure) {
			try {
				std::string input = "procedure x {read 0y;}";
				SimpleTokenizer::tokenizeProgram(input);
				assert(false);
			}
			catch (std::invalid_argument e) {
				assert(e.what() == ExceptionMessages::invalidIdentifier);
			}
			try {
				std::string input = "procedure x {read 10;}";
				SimpleTokenizer::tokenizeProgram(input);
				assert(false);
			}
			catch (std::invalid_argument e) {
				assert(e.what() == ExceptionMessages::invalidIdentifier);
			}
			try {
				std::string input = "procedure x {read ';}";
				SimpleTokenizer::tokenizeProgram(input);
				assert(false);
			}
			catch (std::invalid_argument e) {
				assert(e.what() == ExceptionMessages::invalidIdentifier);
			}
		}
		TEST_METHOD(validSemicolonStatements_success) {
			std::string input = "procedure x {read a; print b; c = ((0 + d )- 3 * 4 / e) % 6}";
			std::shared_ptr<TokenizedProgram> output = SimpleTokenizer::tokenizeProgram(input);

			// Expected
			std::vector<std::shared_ptr<Token>> line1vec = { std::make_shared<ReadKeywordToken>(), std::make_shared<IdentifierToken>("a") };
			std::vector<std::shared_ptr<Token>> line2vec = { std::make_shared<PrintKeywordToken>(), std::make_shared<IdentifierToken>("b") };
			std::vector<std::shared_ptr<Token>> line3vec = {
				std::make_shared<IdentifierToken>("c"),
				std::make_shared<EqualsOpToken>(),
				std::make_shared<ParenOpenSepToken>(),
				std::make_shared<ParenOpenSepToken>(),
				std::make_shared<IntegerLiteralToken>("0"),
				std::make_shared<PlusOpToken>(),
				std::make_shared<IdentifierToken>("d"),
				std::make_shared<ParenCloseSepToken>(),
				std::make_shared<MinusOpToken>(),
				std::make_shared<IntegerLiteralToken>("3"),
				std::make_shared<MultiplyOpToken>(),
				std::make_shared<IntegerLiteralToken>("4"),
				std::make_shared<DivideOpToken>(),
				std::make_shared<IdentifierToken>("e"),
				std::make_shared<ParenCloseSepToken>(),
				std::make_shared<ModuloOpToken>(),
				std::make_shared<IntegerLiteralToken>("6"),
			};
			std::vector<std::shared_ptr<TokenizedStmt>> stmtlistVec = {
				std::make_shared<TokenizedSemicolonStmt>(1, line1vec),
				std::make_shared<TokenizedSemicolonStmt>(2, line2vec),
				std::make_shared<TokenizedSemicolonStmt>(3, line3vec),
			};
			std::shared_ptr<TokenizedStmtList> stmtList = std::make_shared<TokenizedStmtList>(stmtlistVec);
			std::vector< std::shared_ptr<TokenizedProcedure>> proceduresVec = { std::make_shared<TokenizedProcedure>("x", stmtList) };
			TokenizedProgram program = TokenizedProgram(proceduresVec);
			assert((*output).equalsTo(program));
		}

		TEST_METHOD(validIfWhileStatements_success) {
			std::string input = "procedure procedure {if (a==b) then {read x;} else {print y;} while (c<1) {d = d + 1;}}";
			std::shared_ptr<TokenizedProgram> output = SimpleTokenizer::tokenizeProgram(input);

			// Expected
			std::vector<std::shared_ptr<Token>> ifCondition = {
				std::make_shared<IdentifierToken>("a"),
				std::make_shared<EqualityOpToken>(),
				std::make_shared<IdentifierToken>("b")
			};
			std::vector<std::shared_ptr<Token>> thenBlockLineVec = { std::make_shared<ReadKeywordToken>(), std::make_shared<IdentifierToken>("x") };
			std::vector<std::shared_ptr<TokenizedStmt>> thenBlockVec = {
				std::make_shared<TokenizedSemicolonStmt>(2, thenBlockLineVec)
			};
			std::shared_ptr<TokenizedStmtList> thenBlock = std::make_shared<TokenizedStmtList>(thenBlockVec);
			std::vector<std::shared_ptr<Token>> elseBlockLineVec = { std::make_shared<PrintKeywordToken>(), std::make_shared<IdentifierToken>("y") };
			std::vector<std::shared_ptr<TokenizedStmt>> elseBlockVec = {
				std::make_shared<TokenizedSemicolonStmt>(3, elseBlockLineVec)

			};
			std::shared_ptr<TokenizedStmtList> elseBlock = std::make_shared<TokenizedStmtList>(elseBlockVec);

			std::vector<std::shared_ptr<Token>> whileCondition = {
				std::make_shared<IdentifierToken>("c"),
				std::make_shared<LessThanOpToken>(),
				std::make_shared<IntegerLiteralToken>("1")
			};
			std::vector<std::shared_ptr<Token>> whileBlockLineVec = {
				std::make_shared<IdentifierToken>("d"),
				std::make_shared<EqualsOpToken>(),
				std::make_shared<IdentifierToken>("d"),
				std::make_shared<PlusOpToken>(),
				std::make_shared<IntegerLiteralToken>("1")
			};
			std::vector<std::shared_ptr<TokenizedStmt>> whileBlockVec = {
				std::make_shared<TokenizedSemicolonStmt>(5, whileBlockLineVec)
			};
			std::shared_ptr<TokenizedStmtList> whileBlock = std::make_shared<TokenizedStmtList>(whileBlockVec);

			std::vector<std::shared_ptr<TokenizedStmt>> stmtlistVec = {
				std::make_shared<TokenizedIfStmt>(1, ifCondition, thenBlock, elseBlock),
				std::make_shared<TokenizedWhileStmt>(4, whileCondition, whileBlock)
			};
			std::shared_ptr<TokenizedStmtList> stmtList = std::make_shared<TokenizedStmtList>(stmtlistVec);
			std::vector< std::shared_ptr<TokenizedProcedure>> proceduresVec = { std::make_shared<TokenizedProcedure>("procedure", stmtList) };
			TokenizedProgram program = TokenizedProgram(proceduresVec);
			assert((*output).equalsTo(program));
		}

		TEST_METHOD(combinationStatements_success) {
			std::string input = "procedure x {read a; if (a==b) then {read x;} else {print y;} read b; while (c<1) {d = d + 1;} read c;}";
			std::shared_ptr<TokenizedProgram> output = SimpleTokenizer::tokenizeProgram(input);

			// Expected
			std::vector<std::shared_ptr<Token>> readAVec = {
				std::make_shared<ReadKeywordToken>(),
				std::make_shared<IdentifierToken>("a")
			};
			std::vector<std::shared_ptr<Token>> readBVec = {
				std::make_shared<ReadKeywordToken>(),
				std::make_shared<IdentifierToken>("b")
			};
			std::vector<std::shared_ptr<Token>> readCVec = {
				std::make_shared<ReadKeywordToken>(),
				std::make_shared<IdentifierToken>("c")
			};

			std::vector<std::shared_ptr<Token>> ifCondition = { 
				std::make_shared<IdentifierToken>("a"),
				std::make_shared<EqualityOpToken>(),
				std::make_shared<IdentifierToken>("b")
			};
			std::vector<std::shared_ptr<Token>> thenBlockLineVec = { std::make_shared<ReadKeywordToken>(), std::make_shared<IdentifierToken>("x") };
			std::vector<std::shared_ptr<TokenizedStmt>> thenBlockVec = {
				std::make_shared<TokenizedSemicolonStmt>(2, thenBlockLineVec)
			};
			std::shared_ptr<TokenizedStmtList> thenBlock = std::make_shared<TokenizedStmtList>(thenBlockVec);
			std::vector<std::shared_ptr<Token>> elseBlockLineVec = { std::make_shared<PrintKeywordToken>(), std::make_shared<IdentifierToken>("y") };
			std::vector<std::shared_ptr<TokenizedStmt>> elseBlockVec = {
				std::make_shared<TokenizedSemicolonStmt>(3, elseBlockLineVec)

			};
			std::shared_ptr<TokenizedStmtList> elseBlock = std::make_shared<TokenizedStmtList>(elseBlockVec);

			std::vector<std::shared_ptr<Token>> whileCondition = {
				std::make_shared<IdentifierToken>("c"),
				std::make_shared<LessThanOpToken>(),
				std::make_shared<IntegerLiteralToken>("1")
			};
			std::vector<std::shared_ptr<Token>> whileBlockLineVec = {
				std::make_shared<IdentifierToken>("d"),
				std::make_shared<EqualsOpToken>(),
				std::make_shared<IdentifierToken>("d"),
				std::make_shared<PlusOpToken>(),
				std::make_shared<IntegerLiteralToken>("1")
			};
			std::vector<std::shared_ptr<TokenizedStmt>> whileBlockVec = {
				std::make_shared<TokenizedSemicolonStmt>(5, whileBlockLineVec)
			};
			std::shared_ptr<TokenizedStmtList> whileBlock = std::make_shared<TokenizedStmtList>(whileBlockVec);

			std::vector<std::shared_ptr<TokenizedStmt>> stmtlistVec = {
				std::make_shared<TokenizedSemicolonStmt>(1, readAVec),
				std::make_shared<TokenizedIfStmt>(2, ifCondition, thenBlock, elseBlock),
				std::make_shared<TokenizedSemicolonStmt>(5, readBVec),
				std::make_shared<TokenizedWhileStmt>(6, whileCondition, whileBlock),
				std::make_shared<TokenizedSemicolonStmt>(8, readCVec)
			};
			std::shared_ptr<TokenizedStmtList> stmtList = std::make_shared<TokenizedStmtList>(stmtlistVec);
			std::vector< std::shared_ptr<TokenizedProcedure>> proceduresVec = { std::make_shared<TokenizedProcedure>("x", stmtList) };
			TokenizedProgram program = TokenizedProgram(proceduresVec);
			assert((*output).equalsTo(program));
		}

		TEST_METHOD(nestedStatements_success) {
			std::string input = "procedure x {while (a>b) {while(c<d) {read x;}}}";
			std::shared_ptr<TokenizedProgram> output = SimpleTokenizer::tokenizeProgram(input);

			// Expected
			std::vector<std::shared_ptr<Token>> while1Condition = {
				std::make_shared<IdentifierToken>("a"),
				std::make_shared<MoreThanOpToken>(),
				std::make_shared<IdentifierToken>("b")
			};
			std::vector<std::shared_ptr<Token>> while2Condition = {
				std::make_shared<IdentifierToken>("c"),
				std::make_shared<LessThanOpToken>(),
				std::make_shared<IdentifierToken>("d")
			};

			std::vector<std::shared_ptr<Token>> while2ContentsLine = {
				std::make_shared<ReadKeywordToken>(),
				std::make_shared<IdentifierToken>("x")
			};
			std::vector<std::shared_ptr<TokenizedStmt>> while2ContentsVec = {
				std::make_shared<TokenizedSemicolonStmt>(3, while2ContentsLine)
			};
			std::shared_ptr<TokenizedStmtList> while2Block = std::make_shared<TokenizedStmtList>(while2ContentsVec);

			std::vector<std::shared_ptr<TokenizedStmt>> while1BlockVec = {
				std::make_shared<TokenizedWhileStmt>(2, while2Condition, while2Block)
			};
			std::shared_ptr<TokenizedStmtList> while2BlockVec = std::make_shared<TokenizedStmtList>(while1BlockVec);

			std::vector<std::shared_ptr<TokenizedStmt>> stmtlistVec = {
				std::make_shared<TokenizedWhileStmt>(1, while1Condition, while2BlockVec)
			};
			std::shared_ptr<TokenizedStmtList> stmtList = std::make_shared<TokenizedStmtList>(stmtlistVec);
			std::vector< std::shared_ptr<TokenizedProcedure>> proceduresVec = { std::make_shared<TokenizedProcedure>("x", stmtList) };
			TokenizedProgram program = TokenizedProgram(proceduresVec);
			assert((*output).equalsTo(program));
		}

		TEST_METHOD(multipleDeclaration_success) {
			std::string input = "procedure x {read z;} procedure a {print b;}";
			std::shared_ptr<TokenizedProgram> output = SimpleTokenizer::tokenizeProgram(input);

			// Expected
			std::vector<std::shared_ptr<Token>> line1vec = { std::make_shared<ReadKeywordToken>(), std::make_shared<IdentifierToken>("z") };
			std::vector<std::shared_ptr<TokenizedStmt>> stmtlistVec1 = { std::make_shared<TokenizedSemicolonStmt>(1, line1vec) };
			std::shared_ptr<TokenizedStmtList> stmtList1 = std::make_shared<TokenizedStmtList>(stmtlistVec1);

			std::vector<std::shared_ptr<Token>> line2vec = { std::make_shared<PrintKeywordToken>(), std::make_shared<IdentifierToken>("b") };
			std::vector<std::shared_ptr<TokenizedStmt>> stmtlistVec2 = { std::make_shared<TokenizedSemicolonStmt>(2, line2vec) };
			std::shared_ptr<TokenizedStmtList> stmtList2 = std::make_shared<TokenizedStmtList>(stmtlistVec2);

			std::vector< std::shared_ptr<TokenizedProcedure>> proceduresVec = { 
				std::make_shared<TokenizedProcedure>("x", stmtList1),
				std::make_shared<TokenizedProcedure>("a", stmtList2)
			};
			TokenizedProgram program = TokenizedProgram(proceduresVec);
			assert((*output).equalsTo(program));
		}
	};
}
