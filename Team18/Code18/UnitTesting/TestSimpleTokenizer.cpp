#include "stdafx.h"
#include "CppUnitTest.h"

#include "../source/TokenizerClasses/SimpleTokenizer.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace SimpleTokeniser_Test
{
	TEST_CLASS(tokeniseMethod_Test) {
	private:
		bool equalityWrapper(std::vector<std::vector<std::shared_ptr<Token>>> first,
			std::vector<std::vector<std::shared_ptr<Token>>> second) {
			if (!&first || !&second) { // If either are nulls
				return false;
			}
			bool output = true;
			int firstSize = first.size();
			// Check for first dimension same size
			output = output && firstSize == second.size();
			// if no first dimension or not same size, return output
			if (!output || firstSize == 0) return output;
			for (int stmtNum = 0; stmtNum < firstSize; stmtNum++) {
				std::vector<std::shared_ptr<Token>> firstStmt = first[stmtNum];
				std::vector<std::shared_ptr<Token>> secondStmt = second[stmtNum];
				int firstSize = firstStmt.size();
				// Check for second dimension same size
				output = output && firstSize == secondStmt.size();
				// if no second dimension or not same size, return output
				if (!output || firstSize == 0) return output;
				for (int tokenNum = 0; tokenNum < firstSize; tokenNum++) {
					std::shared_ptr<Token> firstToken = firstStmt[tokenNum];
					std::shared_ptr<Token> secondToken = secondStmt[tokenNum];
					output = output && Token::checkEquality(firstToken, secondToken);
				}
			}
			return output;
		}
	public:
	};
}
