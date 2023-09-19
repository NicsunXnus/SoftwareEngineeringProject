#include "stdafx.h"
#include "CppUnitTest.h"

#include "../source/ApplicationWrapper.h"
#include "SystemTestWrapper.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace SystemTesting
{		
	TEST_CLASS(Milestone1) {
	private:
		static inline const std::string defaultPath = "../../Tests18/Milestone1/";
	public:
		
		TEST_METHOD(no_clauses)
		{
			std::string srcFilePath = defaultPath + "no_clauses_source.txt";
			std::string queryFilePath = defaultPath + "no_clauses_queries.txt";
			assert(SystemTestWrapper::run(srcFilePath, queryFilePath));
		}
		TEST_METHOD(single_follows_clause)
		{
			std::string srcFilePath = defaultPath + "single_follows_source.txt";
			std::string queryFilePath = defaultPath + "single_follows_queries.txt";
			assert(SystemTestWrapper::run(srcFilePath, queryFilePath));
		}
		TEST_METHOD(single_followsT_clause)
		{
			std::string srcFilePath = defaultPath + "single_followsT_source.txt";
			std::string queryFilePath = defaultPath + "single_followsT_queries.txt";
			assert(SystemTestWrapper::run(srcFilePath, queryFilePath));
		}
		TEST_METHOD(single_parent_clause)
		{
			std::string srcFilePath = defaultPath + "single_parent_source.txt";
			std::string queryFilePath = defaultPath + "single_parent_queries.txt";
			assert(SystemTestWrapper::run(srcFilePath, queryFilePath));
		}
		TEST_METHOD(single_parentT_clause)
		{
			std::string srcFilePath = defaultPath + "single_parentT_source.txt";
			std::string queryFilePath = defaultPath + "single_parentT_queries.txt";
			assert(SystemTestWrapper::run(srcFilePath, queryFilePath));
		}
		TEST_METHOD(single_uses_clause)
		{
			std::string srcFilePath = defaultPath + "single_uses_source.txt";
			std::string queryFilePath = defaultPath + "single_uses_queries.txt";
			assert(SystemTestWrapper::run(srcFilePath, queryFilePath));
		}
		TEST_METHOD(single_modifies_clause)
		{
			std::string srcFilePath = defaultPath + "single_modifies_source.txt";
			std::string queryFilePath = defaultPath + "single_modifies_queries.txt";
			assert(SystemTestWrapper::run(srcFilePath, queryFilePath));
		}
		TEST_METHOD(single_pattern_clause)
		{
			std::string srcFilePath = defaultPath + "single_pattern_source.txt";
			std::string queryFilePath = defaultPath + "single_pattern_queries.txt";
			assert(SystemTestWrapper::run(srcFilePath, queryFilePath));
		}
		TEST_METHOD(double_clause)
		{
			std::string srcFilePath = defaultPath + "double_clause_source.txt";
			std::string queryFilePath = defaultPath + "double_clause_queries.txt";
			assert(SystemTestWrapper::run(srcFilePath, queryFilePath));
		}

	};
}