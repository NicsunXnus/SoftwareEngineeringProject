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

		TEST_METHOD(emptyProcedure_failure) {
			try {
				std::string input = "procedure x {}";
				SimpleTokenizer::tokenizeProgram(input);
				assert(false);
			}
			catch (std::invalid_argument e) {
				assert(e.what() == ExceptionMessages::emptyProcedureGiven);
			}
		}

		TEST_METHOD(invalidProcedureName_failure) {
			try {
				std::string input = "procedure 1 {}";
				SimpleTokenizer::tokenizeProgram(input);
				assert(false);
			}
			catch (std::invalid_argument e) {
				assert(e.what() == ExceptionMessages::invalidProcedureDefinition);
			}
			try {
				std::string input = "procedure 0x1 {}";
				SimpleTokenizer::tokenizeProgram(input);
				assert(false);
			}
			catch (std::invalid_argument e) {
				assert(e.what() == ExceptionMessages::invalidProcedureDefinition);
			}
			try {
				std::string input = "procedure ' {}";
				SimpleTokenizer::tokenizeProgram(input);
				assert(false);
			}
			catch (std::invalid_argument e) {
				assert(e.what() == ExceptionMessages::invalidProcedureDefinition);
			}
		}

		TEST_METHOD(invalidProcedureDeclaration_failure) {
			try {
				std::string input = "pr0cedure x {}";
				SimpleTokenizer::tokenizeProgram(input);
				assert(false);
			}
			catch (std::invalid_argument e) {
				assert(e.what() == ExceptionMessages::invalidProcedureDefinition);
			}
			try {
				std::string input = "Procedure x {}"; // capitals
				SimpleTokenizer::tokenizeProgram(input);
				assert(false);
			}
			catch (std::invalid_argument e) {
				assert(e.what() == ExceptionMessages::invalidProcedureDefinition);
			}
			try {
				std::string input = "procedure    {}";
				SimpleTokenizer::tokenizeProgram(input);
				assert(false);
			}
			catch (std::invalid_argument e) {
				assert(e.what() == ExceptionMessages::invalidProcedureDefinition);
			}
			try {
				std::string input = "     procName   {}";
				SimpleTokenizer::tokenizeProgram(input);
				assert(false);
			}
			catch (std::invalid_argument e) {
				assert(e.what() == ExceptionMessages::invalidProcedureDefinition);
			}
		}

		TEST_METHOD(emptyStatement_failure) {
			try {
				std::string input = "procedure x { ;}";
				SimpleTokenizer::tokenizeProgram(input);
				assert(false);
			}
			catch (std::invalid_argument e) {
				assert(e.what() == ExceptionMessages::emptyProcedureGiven);
			}
		}
	};
}
