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
			
			std::shared_ptr<ExprNode> e = ASTBuilder::parseExpr(exprTest2, 1);
			std::stringstream output;
			std::streambuf* oldCoutBuffer = std::cout.rdbuf(output.rdbuf());

			std::cout << printExpr(e);

			std::cout.rdbuf(oldCoutBuffer);

			Logger::WriteMessage("Output of parseExpr:\n");
			Logger::WriteMessage(output.str().c_str());

		}

		TEST_METHOD(TestValidSimpleProgram) {
			std::shared_ptr<TokenizedProgram> test = SimpleTokenizer::tokenizeProgram(
				"procedure p { read num1; read num2; read num3; sum = num1 + num2 + num3;"
				"ave = sum / 3;"
				"print ave;}"
			);

			std::shared_ptr<ProgramNode> program = ASTBuilder::parseProgram(test);
			std::stringstream output;
			std::streambuf* oldCoutBuffer = std::cout.rdbuf(output.rdbuf());

			std::cout << printProcedure(program->getProcedures()[0]);

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
				"} else { while (a == 2) { print num5; } }"
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

		TEST_METHOD(Invalids) {
			try {
				std::shared_ptr<TokenizedProgram> test = SimpleTokenizer::tokenizeProgram(
					"procedure a { read num2;"
					"sum = num2 + num3);"
					"print ave;}"
				);
				std::shared_ptr<ProgramNode> program = ASTBuilder::parseProgram(test);
			}
			catch (const std::exception& ex) {
				Assert::AreEqual("Missing left bracket.", ex.what());
			}

			try {
				std::shared_ptr<TokenizedProgram> test = SimpleTokenizer::tokenizeProgram(
					"procedure a {read num2;"
					"sum = (num2 + num3 * 5;"
					"print ave;}"
				);
				std::shared_ptr<ProgramNode> program = ASTBuilder::parseProgram(test);
			}
			catch (const std::exception& ex) {
				Assert::AreEqual("Need 2 operatees to operate on.",ex.what());
			}

		}
	};
}