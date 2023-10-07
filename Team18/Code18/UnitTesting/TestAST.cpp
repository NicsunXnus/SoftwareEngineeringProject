#include "stdafx.h"
#include "CppUnitTest.h"
#include "../source/SimpleTokenizer/SimpleTokenizer.h"
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
			
			std::shared_ptr<CondExprNode> condExpr = ASTBuilder::parseCondExpr(procedureTest, 0);
			
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

		TEST_METHOD(TestCall) {
			std::shared_ptr<TokenizedProgram> test = SimpleTokenizer::tokenizeProgram(
				"procedure p {"
				"call q; }"
				"procedure q {"
				"call z; }"
				"procedure y {"
				"call q; }"
				"procedure z {"
				"print done; }"
			);

			std::shared_ptr<ProgramNode> program = ASTBuilder::parseProgram(test);

			std::stringstream output;
			std::streambuf* oldCoutBuffer = std::cout.rdbuf(output.rdbuf());

			//std::shared_ptr<ProcedureNode> proc = program->getProcedures()[0];
			std::cout << printProgram(program);

			std::cout.rdbuf(oldCoutBuffer);

			Logger::WriteMessage("Output:\n");
			Logger::WriteMessage(output.str().c_str());

		}

		TEST_METHOD(TestCall2) {
			//if node after callnode
			std::shared_ptr<TokenizedProgram> test = SimpleTokenizer::tokenizeProgram(
				"procedure p { print paper; call q; read book; bioshock = 1 + 2 + infinite;"
				"if (fruit == pear) then { print true; } else { print false; }"
				"while (semester != end) { print noSleep; }"
				"call q; }"
				//All statements before call node + while node directly before callnode
				"procedure q { call printSth; call printSth; print paper; read magazine; enterTheGungeon = bullets * chaos;"
				"if (fruit == pear) then { print true; } else { print false; }"
				"while (semester != end) { print noSleep; } }"
				//All statements after callnode + consecutive callnodes in a procedure
				"procedure y {"
				"if (game == onSale) then { call checkWallet; } else { print wait; }"
				"call q; }"
				//If node directly before callnode + callnode in if statement
				"procedure checkWallet { call printSth; while (money <= 0 && wallet == empty) { call terminate; }; }"
				//while node after callnode + callnode in while node
				"procedure printSth { print Sth; }"
				"procedure ifNodeAfterCallNode { call printSth;"
				"if (1 == 2) then { print yes; } else { print no; } }"
			    //if node after call node
				"procedure terminate { print done; }"
			);

			/*
			Expected output:
			procedure p {
			1	print paper
			2	calls q
			3	read book
			4	bioshock = 1 + 2 + infinite
			5	if (fruit == pear) {
			6		print true
				} else {
			7		print false
				}
			8	while (semester != end) {
			9		print noSleep
				}
			10	calls q
			}
			procedure q {
			11	calls printSth
			12	calls printSth
			13	print paper
			14	read magazine
			15	enterTheGungeon = bullets * chaos
			16	if (fruit == pear) {
			17		print true
				} else {
			18		print false
				}
			19	while (semester != end) {
			20		print noSleep
				}
			}
			procedure y {
			21	if (game == onSale) {
			22		calls checkWallet
				} else {
			23		print wait
				}
			24	calls q
			}
			procedure checkWallet {
			25	calls printSth
			26	while ((money <= 0) && (wallet == empty)) {
			27		calls terminate
				}
			}
			procedure printSth {
			28	print Sth
			}
			procedure ifNodeAfterCallNode {
			29	calls printSth
			30	if (1 == 2) {
			31		print yes
				} else {
			32		print no
				}
			}
			procedure terminate {
			33	print done
			}
			*/

			std::shared_ptr<ProgramNode> program = ASTBuilder::parseProgram(test);

			std::stringstream output;
			std::streambuf* oldCoutBuffer = std::cout.rdbuf(output.rdbuf());

			//std::shared_ptr<ProcedureNode> proc = program->getProcedures()[0];
			std::cout << printProgram(program);

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