#include "stdafx.h"
#include "SystemTestWrapper.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace SystemTesting {
TEST_CLASS(Milestone1) {
 private:
  static inline const string defaultPath = "../../Tests18/Milestone1/";

 public:
  TEST_METHOD(no_clauses) {
    string srcFilePath = defaultPath + "no_clauses_source.txt";
    string queryFilePath = defaultPath + "no_clauses_queries.txt";
    assert(SystemTestWrapper::run(srcFilePath, queryFilePath));
  }
  TEST_METHOD(single_follows_clause) {
    string srcFilePath = defaultPath + "single_follows_source.txt";
    string queryFilePath = defaultPath + "single_follows_queries.txt";
    assert(SystemTestWrapper::run(srcFilePath, queryFilePath));
  }
  TEST_METHOD(single_followsT_clause) {
    string srcFilePath = defaultPath + "single_followsT_source.txt";
    string queryFilePath = defaultPath + "single_followsT_queries.txt";
    assert(SystemTestWrapper::run(srcFilePath, queryFilePath));
  }
  TEST_METHOD(single_parent_clause) {
    string srcFilePath = defaultPath + "single_parent_source.txt";
    string queryFilePath = defaultPath + "single_parent_queries.txt";
    assert(SystemTestWrapper::run(srcFilePath, queryFilePath));
  }
  TEST_METHOD(single_parentT_clause) {
    string srcFilePath = defaultPath + "single_parentT_source.txt";
    string queryFilePath = defaultPath + "single_parentT_queries.txt";
    assert(SystemTestWrapper::run(srcFilePath, queryFilePath));
  }
  TEST_METHOD(single_uses_clause) {
    string srcFilePath = defaultPath + "single_uses_source.txt";
    string queryFilePath = defaultPath + "single_uses_queries.txt";
    assert(SystemTestWrapper::run(srcFilePath, queryFilePath));
  }
  TEST_METHOD(single_modifies_clause) {
    string srcFilePath = defaultPath + "single_modifies_source.txt";
    string queryFilePath = defaultPath + "single_modifies_queries.txt";
    assert(SystemTestWrapper::run(srcFilePath, queryFilePath));
  }
  TEST_METHOD(single_pattern_clause) {
    string srcFilePath = defaultPath + "single_pattern_source.txt";
    string queryFilePath = defaultPath + "single_pattern_queries.txt";
    assert(SystemTestWrapper::run(srcFilePath, queryFilePath));
  }
  TEST_METHOD(double_clause) {
    string srcFilePath = defaultPath + "double_clause_source.txt";
    string queryFilePath = defaultPath + "double_clause_queries.txt";
    assert(SystemTestWrapper::run(srcFilePath, queryFilePath));
  }
  TEST_METHOD(debug) {
    string srcFilePath = defaultPath + "single_follows_source.txt";
    string queryFilePath = defaultPath + "single_follows_queries.txt";
    assert(SystemTestWrapper::run(srcFilePath, queryFilePath, true));
  }
};

TEST_CLASS(Milestone2) {
 private:
  static inline const string defaultPath = "../../Tests18/Milestone2/";

 public:
      TEST_METHOD(single_calls_clause) {
        string srcFilePath = defaultPath + "single_calls_source.txt";
        string queryFilePath = defaultPath + "single_calls_queries.txt";
        assert(SystemTestWrapper::run(srcFilePath, queryFilePath));
      }

	TEST_METHOD(single_callsStar_clause)
	{
		string srcFilePath = defaultPath + "single_callsStar_source.txt";
		string queryFilePath = defaultPath + "single_callsStar_queries.txt";
		assert(SystemTestWrapper::run(srcFilePath, queryFilePath));
	}

		//TEST_METHOD(sprint3_test)
		//{
		//	string srcFilePath = defaultPath + "sprint3_source.txt";
		//	string queryFilePath = defaultPath + "sprint3_queries.txt";
		//	assert(SystemTestWrapper::run(srcFilePath, queryFilePath));
		//}

		TEST_METHOD(single_next_clause)
		{
			string srcFilePath = defaultPath + "single_next_source.txt";
			string queryFilePath = defaultPath + "single_next_queries.txt";
			assert(SystemTestWrapper::run(srcFilePath, queryFilePath));
		}
    TEST_METHOD(no_clauses) {
      string srcFilePath = defaultPath + "no_clause_source.txt";
      string queryFilePath = defaultPath + "no_clause_queries.txt";
      assert(SystemTestWrapper::run(srcFilePath, queryFilePath));
    }
    TEST_METHOD(single_follows_clause) {
      string srcFilePath = defaultPath + "single_follows_source.txt";
      string queryFilePath = defaultPath + "single_follows_queries.txt";
      assert(SystemTestWrapper::run(srcFilePath, queryFilePath));
    }
    TEST_METHOD(single_followsT_clause) {
      string srcFilePath = defaultPath + "single_followsT_source.txt";
      string queryFilePath = defaultPath + "single_followsT_queries.txt";
      assert(SystemTestWrapper::run(srcFilePath, queryFilePath));
    }
    TEST_METHOD(single_parent_clause) {
      string srcFilePath = defaultPath + "single_parent_source.txt";
      string queryFilePath = defaultPath + "single_parent_queries.txt";
      assert(SystemTestWrapper::run(srcFilePath, queryFilePath));
    }
    TEST_METHOD(single_parentT_clause) {
      string srcFilePath = defaultPath + "single_parentT_source.txt";
      string queryFilePath = defaultPath + "single_parentT_queries.txt";
      assert(SystemTestWrapper::run(srcFilePath, queryFilePath));
    }
    TEST_METHOD(single_usesP_clause) {
      string srcFilePath = defaultPath + "single_UsesP_source.txt";
      string queryFilePath = defaultPath + "single_UsesP_queries.txt";
      assert(SystemTestWrapper::run(srcFilePath, queryFilePath));
    }
    TEST_METHOD(single_pattern_clause) {
      string srcFilePath = defaultPath + "single_pattern_source.txt";
      string queryFilePath = defaultPath + "single_pattern_queries.txt";
      assert(SystemTestWrapper::run(srcFilePath, queryFilePath));
    }
		TEST_METHOD(single_modifies_clause)
		{
			string srcFilePath = defaultPath + "single_modifies_source.txt";
			string queryFilePath = defaultPath + "single_modifies_queries.txt";
			assert(SystemTestWrapper::run(srcFilePath, queryFilePath));
		}
		TEST_METHOD(debugM2)
		{
			string srcFilePath = defaultPath + "debug_source.txt";
			string queryFilePath = defaultPath + "debug_queries.txt";
			assert(SystemTestWrapper::run(srcFilePath, queryFilePath));
		}
};

	TEST_CLASS(Misc) {
	private:
		static inline const string defaultPath = "../../Tests18/";
	public:
		TEST_METHOD(StressTest)
		{
			string srcFilePath = defaultPath + "Stress_source.txt";
			string queryFilePath = defaultPath + "Stress_queries.txt";
			assert(SystemTestWrapper::run(srcFilePath, queryFilePath));
		}

	};
}