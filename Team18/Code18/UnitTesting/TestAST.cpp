#include "stdafx.h"
#include "CppUnitTest.h"

#include "../source/TokenizerClasses/SimpleTokenizer.h"
#include "../source/TokenizerClasses/TokenFactory.h"
#include "../source/AST/ASTBuilder.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(TestSP)
	{
	public:
		TEST_METHOD(TestExpression)
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

			std::shared_ptr<ExprNode> e = ASTBuilder::parseExpr(exprTest);

			std::stringstream output;
			std::streambuf* oldCoutBuffer = std::cout.rdbuf(output.rdbuf());

			std::cout << ASTBuilder::printExpr(e);

			std::cout.rdbuf(oldCoutBuffer);

			Logger::WriteMessage("Output of printExpr:\n");
			Logger::WriteMessage(output.str().c_str());

		}
		TEST_METHOD(TestStatement)
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
		}

		TEST_METHOD(TestMethod2)
		{
			int x = 10;
			assert(x == 1);
			// throw std::invalid_argument("str");
		}

	};
}