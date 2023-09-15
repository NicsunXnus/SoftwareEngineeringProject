#include "stdafx.h"
#include "CppUnitTest.h"
#include "../source/TokenizerClasses/SimpleTokenizer.h"
#include "../source/AST/ASTBuilder.h"
#include "../source/AST/ASTBuilderHelperFunctions.h"
#include <cassert>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
namespace UnitTesting
{
	TEST_CLASS(TestSP)
	{
	public:
		TEST_METHOD(TestValidExpression)
		{
			std::string complicatedExpr = "(3 + ((6*3) % (8*(4 + 5))))";// > (3 + ((6*3) % (8*(4 + 5))))";
			std::vector<std::shared_ptr<Token>> exprTest;
			exprTest.emplace_back(std::make_shared<SeparatorToken>("("));
			exprTest.emplace_back(std::make_shared<LiteralToken>("3"));
			exprTest.emplace_back(std::make_shared<OperatorToken>("+"));
			exprTest.emplace_back(std::make_shared<SeparatorToken>("("));
			exprTest.emplace_back(std::make_shared<SeparatorToken>("("));
			exprTest.emplace_back(std::make_shared<LiteralToken>("6"));
			exprTest.emplace_back(std::make_shared<OperatorToken>("*"));
			exprTest.emplace_back(std::make_shared<LiteralToken>("3"));
			exprTest.emplace_back(std::make_shared<SeparatorToken>(")"));
			exprTest.emplace_back(std::make_shared<OperatorToken>("%"));
			exprTest.emplace_back(std::make_shared<SeparatorToken>("("));
			exprTest.emplace_back(std::make_shared<LiteralToken>("8"));
			exprTest.emplace_back(std::make_shared<OperatorToken>("*"));
			exprTest.emplace_back(std::make_shared<SeparatorToken>("("));
			exprTest.emplace_back(std::make_shared<LiteralToken>("4"));
			exprTest.emplace_back(std::make_shared<OperatorToken>("+"));
			exprTest.emplace_back(std::make_shared<LiteralToken>("5"));
			exprTest.emplace_back(std::make_shared<SeparatorToken>(")"));
			exprTest.emplace_back(std::make_shared<SeparatorToken>(")"));
			exprTest.emplace_back(std::make_shared<SeparatorToken>(")"));
			exprTest.emplace_back(std::make_shared<SeparatorToken>(")"));

			std::vector<std::shared_ptr<Token>> exprTest2 = {
				std::make_shared<Token>("4"),
				std::make_shared<Token>("+"),
				std::make_shared<Token>("("),
				std::make_shared<Token>("1"),
				std::make_shared<Token>("+"),
				std::make_shared<Token>("2"),
				std::make_shared<Token>("+"),
				std::make_shared<Token>("3"),
				std::make_shared<Token>("*"),
				std::make_shared<Token>("4"),
				std::make_shared<Token>(")")
			};
			
			std::shared_ptr<ExprNode> e = ASTBuilder::parseExpr(exprTest2);
			std::stringstream output;
			std::streambuf* oldCoutBuffer = std::cout.rdbuf(output.rdbuf());

			std::cout << printExpr(e);

			std::cout.rdbuf(oldCoutBuffer);

			Logger::WriteMessage("Output of parseExpr:\n");
			Logger::WriteMessage(output.str().c_str());

		}
		//Old method with tokens manually added
		/*TEST_METHOD(TestStatement)
		{
			std::string complicatedExpr = "(3 + ((6*3) % (8*(4 + 5))))";// > (3 + ((6 * 3) % (8 * (4 + 5))))";
			std::vector<std::shared_ptr<Token>> procedureTest;
			//start of procedure

			//read x;
			procedureTest.emplace_back(std::make_shared<ReadKeywordToken>());
			procedureTest.emplace_back(std::make_shared<IdentifierToken>("x"sv));
			procedureTest.emplace_back(std::make_shared<SemicolonSepToken>());

			//a = (3 + ((6*3) % (8*(4 + 5))));
			procedureTest.emplace_back(std::make_shared<IdentifierToken>("a"sv));
			procedureTest.emplace_back(std::make_shared<EqualsOpToken>());
			//expression part
			procedureTest.emplace_back(std::make_shared<ParenOpenSepToken>());
			procedureTest.emplace_back(std::make_shared<LiteralToken>("3"));
			procedureTest.emplace_back(std::make_shared<PlusOpToken>());
			procedureTest.emplace_back(std::make_shared<ParenOpenSepToken>());
			procedureTest.emplace_back(std::make_shared<ParenOpenSepToken>());
			procedureTest.emplace_back(std::make_shared<LiteralToken>("6"));
			procedureTest.emplace_back(std::make_shared<MultiplyOpToken>());
			procedureTest.emplace_back(std::make_shared<LiteralToken>("3"));
			procedureTest.emplace_back(std::make_shared<ParenCloseSepToken>());
			procedureTest.emplace_back(std::make_shared<ModuloOpToken>());
			procedureTest.emplace_back(std::make_shared<ParenOpenSepToken>());
			procedureTest.emplace_back(std::make_shared<LiteralToken>("8"));
			procedureTest.emplace_back(std::make_shared<MultiplyOpToken>());
			procedureTest.emplace_back(std::make_shared<ParenOpenSepToken>());
			procedureTest.emplace_back(std::make_shared<LiteralToken>("4"));
			procedureTest.emplace_back(std::make_shared<PlusOpToken>());
			procedureTest.emplace_back(std::make_shared<LiteralToken>("5"));
			procedureTest.emplace_back(std::make_shared<ParenCloseSepToken>());
			procedureTest.emplace_back(std::make_shared<ParenCloseSepToken>());
			procedureTest.emplace_back(std::make_shared<ParenCloseSepToken>());
			procedureTest.emplace_back(std::make_shared<ParenCloseSepToken>());
			//end of expression
			procedureTest.emplace_back(std::make_shared<SemicolonSepToken>());

			//a = a + 1;
			procedureTest.emplace_back(std::make_shared<IdentifierToken>("a"sv));
			procedureTest.emplace_back(std::make_shared<EqualsOpToken>());
			procedureTest.emplace_back(std::make_shared<IdentifierToken>("a"sv));
			procedureTest.emplace_back(std::make_shared<PlusOpToken>());
			procedureTest.emplace_back(std::make_shared<LiteralToken>("1"));
			procedureTest.emplace_back(std::make_shared<SemicolonSepToken>());

			//print x
			procedureTest.emplace_back(std::make_shared<PrintKeywordToken>());
			procedureTest.emplace_back(std::make_shared<IdentifierToken>("x"sv));
			procedureTest.emplace_back(std::make_shared<SemicolonSepToken>());

			std::vector<std::shared_ptr<StatementNode>> statements = ASTBuilder::parseStatements(procedureTest);
			std::stringstream output;
			std::streambuf* oldCoutBuffer = std::cout.rdbuf(output.rdbuf());

			std::string res = "";
			res += std::to_string(statements[0]->getStatementNumber()) + " read " + ASTBuilder::printExpr(statements[0]->getVar()) + "\n";
			res += std::to_string(statements[1]->getStatementNumber()) + " " + ASTBuilder::printExpr(statements[1]->getVar()) + " = " + ASTBuilder::printExpr(statements[1]->getExpr()) + "\n";
			res += std::to_string(statements[2]->getStatementNumber()) + " " + ASTBuilder::printExpr(statements[2]->getVar()) + " = " + ASTBuilder::printExpr(statements[2]->getExpr()) + "\n";
			res += std::to_string(statements[3]->getStatementNumber()) + " print " + ASTBuilder::printExpr(statements[0]->getVar());
			std::cout << res;

			std::cout.rdbuf(oldCoutBuffer);

			Logger::WriteMessage("Output of parseStatements:\n");
			Logger::WriteMessage(output.str().c_str());
		}*/

		TEST_METHOD(TestValidSimpleProgram) {
			std::shared_ptr<TokenizedProgram> test = SimpleTokenizer::tokenizeProgram(
				"procedure p { read num1; read num2; read num3; sum = num1 + num2 + num3;"
				"ave = sum / 3;"
				"print ave;}"
			);

			std::shared_ptr<ProgramNode> program = ASTBuilder::parseProgram(test);
			std::stringstream output;
			std::streambuf* oldCoutBuffer = std::cout.rdbuf(output.rdbuf());

			vector<shared_ptr<StatementNode>> statements = program->getProcedures()[0]->getStatements();
			std::string res = "";
			res += std::to_string(statements[0]->getStatementNumber()) + " read " + printExpr(statements[0]->getVar()) + "\n";
			res += std::to_string(statements[1]->getStatementNumber()) + " read " + printExpr(statements[1]->getVar()) + "\n";
			res += std::to_string(statements[2]->getStatementNumber()) + " read " + printExpr(statements[2]->getVar()) + "\n";
			res += std::to_string(statements[3]->getStatementNumber()) + " " + printExpr(statements[3]->getVar()) + " = " + printExpr(statements[3]->getExpr()) + "\n";
			res += std::to_string(statements[4]->getStatementNumber()) + " " + printExpr(statements[4]->getVar()) + " = " + printExpr(statements[4]->getExpr()) + "\n";
			res += std::to_string(statements[5]->getStatementNumber()) + " print " + printExpr(statements[5]->getVar());
			std::cout << res;

			std::cout.rdbuf(oldCoutBuffer);

			Logger::WriteMessage("Output:\n");
			Logger::WriteMessage(output.str().c_str());
		}

		TEST_METHOD(TestSimpleIfWhileStatement) {
			std::shared_ptr<TokenizedProgram> test = SimpleTokenizer::tokenizeProgram(
				"procedure p {"
				"if (x == 3) then {"
				"read num1;"
				"print num3;"
				"sum = num1 + num2 + num3;}"
				"else{read num2;}"
				"while (y == 4){"
				"print num3;"
				"read num1;"
				"sum = num1 + num2 + num3;"
				"}}"
			);

			std::shared_ptr<ProgramNode> program = ASTBuilder::parseProgram(test);

			std::stringstream output;
			std::streambuf* oldCoutBuffer = std::cout.rdbuf(output.rdbuf());

			std::shared_ptr<ProcedureNode> proc = program->getProcedures()[0];
			std::cout << printProcedure(proc);
			/*vector<shared_ptr<StatementNode>> statements = program->getProcedures()[0]->getStatements();

			std::cout << statements[0]->getStatementNumber() << " if (" << printCondExpr(statements[0]->getCondExpr()) << ") {\n";
			std::vector<std::shared_ptr<StatementNode>> thenStmts = statements[0]->getStatements();
			for (std::shared_ptr<StatementNode> stmtNode : thenStmts) {
				if (stmtNode->getName() == "read" || stmtNode->getName() == "print") {
					std::cout << stmtNode->getStatementNumber() << " " << stmtNode->getName() << " " << stmtNode->getVar()->getValue() << std::endl;
				}
				else if (stmtNode->getName() == "assign") {
					std::cout << stmtNode->getStatementNumber() << " " << stmtNode->getVar()->getValue() << " = " << printExpr(stmtNode->getExpr()) << std::endl;
				}
			}
			std::cout << "} else {\n";
			std::vector<std::shared_ptr<StatementNode>> elseStmts = statements[0]->getElseStatements();
			for (std::shared_ptr<StatementNode> stmtNode : elseStmts) {
				if (stmtNode->getName() == "read" || stmtNode->getName() == "print") {
					std::cout << stmtNode->getStatementNumber() << " "  << stmtNode->getName() << " " << stmtNode->getVar()->getValue() << std::endl;
				}
				else if (stmtNode->getName() == "assign") {
					std::cout << stmtNode->getStatementNumber() << " " << stmtNode->getVar()->getValue() << " = " << printExpr(stmtNode->getExpr()) << std::endl;
				}
			}

			std::cout << statements[1]->getStatementNumber() << " while (" << printCondExpr(statements[1]->getCondExpr()) << ") {\n";
			std::vector<std::shared_ptr<StatementNode>> loopStmts = statements[1]->getStatements();
			for (std::shared_ptr<StatementNode> stmtNode : loopStmts) {
				if (stmtNode->getName() == "read" || stmtNode->getName() == "print") {
					std::cout << stmtNode->getStatementNumber() << " " << stmtNode->getName() << " " << stmtNode->getVar()->getValue() << std::endl;
				}
				else if (stmtNode->getName() == "assign") {
					std::cout << stmtNode->getStatementNumber() << " " << stmtNode->getVar()->getValue() << " = " << printExpr(stmtNode->getExpr()) << std::endl;
				}
			}*/

			std::cout.rdbuf(oldCoutBuffer);

			Logger::WriteMessage("Output:\n");
			Logger::WriteMessage(output.str().c_str());

		}

		TEST_METHOD(TestValidCondition)
		{
			std::string complicatedExpr = "(3 + ((6 * 3) % (8 * (4 + 5))))";// > (3 + ((6 * 3) % (8 * (4 + 5))))";
			std::string complicatedConditionExpr = "!(" + complicatedExpr + " > b) && (c <= d || e != 3) && (g < 5 || i >= j) && a == 5";
			std::vector<std::shared_ptr<Token>> procedureTest;
			//start of procedure

			//!(
			procedureTest.emplace_back(std::make_shared<NotOpToken>());
			procedureTest.emplace_back(std::make_shared<ParenOpenSepToken>());

			//(3 + ((6*3) % (8*(4 + 5))))
			//expression part
			procedureTest.emplace_back(std::make_shared<ParenOpenSepToken>());
			procedureTest.emplace_back(std::make_shared<LiteralToken>("3"));
			procedureTest.emplace_back(std::make_shared<PlusOpToken>());
			procedureTest.emplace_back(std::make_shared<ParenOpenSepToken>());
			procedureTest.emplace_back(std::make_shared<ParenOpenSepToken>());
			procedureTest.emplace_back(std::make_shared<LiteralToken>("6"));
			procedureTest.emplace_back(std::make_shared<MultiplyOpToken>());
			procedureTest.emplace_back(std::make_shared<LiteralToken>("3"));
			procedureTest.emplace_back(std::make_shared<ParenCloseSepToken>());
			procedureTest.emplace_back(std::make_shared<ModuloOpToken>());
			procedureTest.emplace_back(std::make_shared<ParenOpenSepToken>());
			procedureTest.emplace_back(std::make_shared<LiteralToken>("8"));
			procedureTest.emplace_back(std::make_shared<MultiplyOpToken>());
			procedureTest.emplace_back(std::make_shared<ParenOpenSepToken>());
			procedureTest.emplace_back(std::make_shared<LiteralToken>("4"));
			procedureTest.emplace_back(std::make_shared<PlusOpToken>());
			procedureTest.emplace_back(std::make_shared<LiteralToken>("5"));
			procedureTest.emplace_back(std::make_shared<ParenCloseSepToken>());
			procedureTest.emplace_back(std::make_shared<ParenCloseSepToken>());
			procedureTest.emplace_back(std::make_shared<ParenCloseSepToken>());
			procedureTest.emplace_back(std::make_shared<ParenCloseSepToken>());
			//end of expression

			// > b) && (c <= d || e != 3) && (g < 5 || i >= j) && a == 5
			procedureTest.emplace_back(std::make_shared<MoreThanOpToken>());
			procedureTest.emplace_back(std::make_shared<IdentifierToken>("b"sv));
			procedureTest.emplace_back(std::make_shared<ParenCloseSepToken>());
			procedureTest.emplace_back(std::make_shared<AndOpToken>());
			procedureTest.emplace_back(std::make_shared<ParenOpenSepToken>());
			procedureTest.emplace_back(std::make_shared<IdentifierToken>("c"sv));
			procedureTest.emplace_back(std::make_shared<LessThanEqualOpToken>());
			procedureTest.emplace_back(std::make_shared<IdentifierToken>("d"sv));
			procedureTest.emplace_back(std::make_shared<OrOpToken>());
			procedureTest.emplace_back(std::make_shared<IdentifierToken>("e"sv));
			procedureTest.emplace_back(std::make_shared<InequalityOpToken>());
			procedureTest.emplace_back(std::make_shared<LiteralToken>("3"));
			procedureTest.emplace_back(std::make_shared<ParenCloseSepToken>());
			procedureTest.emplace_back(std::make_shared<AndOpToken>());
			procedureTest.emplace_back(std::make_shared<ParenOpenSepToken>());
			procedureTest.emplace_back(std::make_shared<IdentifierToken>("g"sv));
			procedureTest.emplace_back(std::make_shared<LessThanOpToken>());
			procedureTest.emplace_back(std::make_shared<LiteralToken>("5"));
			procedureTest.emplace_back(std::make_shared<OrOpToken>());
			procedureTest.emplace_back(std::make_shared<IdentifierToken>("i"sv));
			procedureTest.emplace_back(std::make_shared<MoreThanEqualOpToken>());
			procedureTest.emplace_back(std::make_shared<IdentifierToken>("j"sv));
			procedureTest.emplace_back(std::make_shared<ParenCloseSepToken>());
			procedureTest.emplace_back(std::make_shared<AndOpToken>());
			procedureTest.emplace_back(std::make_shared<IdentifierToken>("a"sv));
			procedureTest.emplace_back(std::make_shared<EqualityOpToken>());
			procedureTest.emplace_back(std::make_shared<LiteralToken>("5"));
			
			std::shared_ptr<CondExprNode> condExpr = ASTBuilder::parseCondExpr(procedureTest);
			
			std::stringstream output;
			std::streambuf* oldCoutBuffer = std::cout.rdbuf(output.rdbuf());

			std::string res = printCondExpr(condExpr);
			std::cout << res;

			std::cout.rdbuf(oldCoutBuffer);

			Logger::WriteMessage("Output of parseCondExpr:\n");
			Logger::WriteMessage(output.str().c_str());
		}

		TEST_METHOD(TestSuperNest) {
			std::shared_ptr<TokenizedProgram> test = SimpleTokenizer::tokenizeProgram(
				"procedure p {"
				"if (x == 3) then {"
				"read num1;"
				"if (y != 4 && z < 5) then {"
				"print num3;"
				"if (!(j >= 4)) then {"
				"sum = num1 + num2 + num3; "
				"} else { read num5; }"
				"} else { while (a == 2) { num5; } }"
				"} else { read num2; }"
				"}"
			);

			std::shared_ptr<ProgramNode> program = ASTBuilder::parseProgram(test);

			std::stringstream output;
			std::streambuf* oldCoutBuffer = std::cout.rdbuf(output.rdbuf());

			std::shared_ptr<ProcedureNode> proc = program->getProcedures()[0];
			std::cout << printProcedure(proc);

			std::cout.rdbuf(oldCoutBuffer);

			Logger::WriteMessage("Output:\n");
			Logger::WriteMessage(output.str().c_str());

		}

		/*TEST_METHOD(Invalids) {
			try {
				std::vector < std::vector<std::shared_ptr<Token>>> test = SimpleTokenizer::tokenize(
					"read num2;"
					"sum = num2 @ num3;"
					"print ave;"
				);
				std::vector<std::shared_ptr<StatementNode>> statements = ASTBuilder::parseProgram(test);
			}
			catch (const std::exception& ex) {
				Assert::AreEqual(ex.what(), "Invalid string supplied: @");
			}

			try {
				std::vector < std::vector<std::shared_ptr<Token>>> test = SimpleTokenizer::tokenize(
					"read num2;"
					"sum = num2 + num3);"
					"print ave;"
				);
				std::vector<std::shared_ptr<StatementNode>> statements = ASTBuilder::parseProgram(test);
			}
			catch (const std::exception& ex) {
				Assert::AreEqual(ex.what(), "Missing left bracket.");
			}

			try {
				std::vector < std::vector<std::shared_ptr<Token>>> test = SimpleTokenizer::tokenize(
					"read num2;"
					"sum = (num2 + num3 * 5;"
					"print ave;"
				);
				std::vector<std::shared_ptr<StatementNode>> statements = ASTBuilder::parseProgram(test);
			}
			catch (const std::exception& ex) {
				Assert::AreEqual(ex.what(), "Need 2 operatees to operate on.");
			}

		}*/
	};
}