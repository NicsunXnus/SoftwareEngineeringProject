#include "stdafx.h"
#include "CppUnitTest.h"
#include <PKB.h>
#include <PKB/Adapter.h>


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
    TEST_CLASS(TestAdapter)
    {
    public:
        TEST_METHOD(TestConvertVectorToUnorderedSet)
		{
            // Arrange
            shared_ptr<map<string, vector<string>>> stringMap = make_shared<map<string, vector<string>>>();
            vector<string> vector1 = { "1", "2", "3", "3" };
            vector<string> vector2 = { "4", "4", "4" };
            stringMap->insert(make_pair("key1", vector1));
            stringMap->insert(make_pair("key2", vector2));

            // Act
            shared_ptr<map<string, unordered_set<string>>> resultSet = convertVectorToUnorderedSet(stringMap);

            // Assert
            Assert::AreEqual(3, (int)resultSet->at("key1").size());
            Assert::AreEqual(1, (int)resultSet->at("key2").size());
            Assert::AreEqual(1, (int)resultSet->at("key1").count("1"));
            Assert::AreEqual(1, (int)resultSet->at("key1").count("2"));
            Assert::AreEqual(1, (int)resultSet->at("key1").count("3"));
            Assert::AreEqual(1, (int)resultSet->at("key2").count("4"));
        }

        TEST_METHOD(TestConvertParentsFollowsStarToParentsFollows)
		{
            // Arrange
            shared_ptr<map<string, vector<string>>> stringMap = make_shared<map<string, vector<string>>>();
            vector<string> vector1 = { "1", "2", "3" };
            vector<string> vector2 = { "4", "5", "6" };
            stringMap->insert(make_pair("key1", vector1));
            stringMap->insert(make_pair("key2", vector2));

            // Act
            shared_ptr<map<string, unordered_set<string>>> resultSet = convertParentsFollowsStarToParentsFollows(stringMap);

            // Assert
            Assert::AreEqual(1, (int)resultSet->at("key1").size());
            Assert::AreEqual(1, (int)resultSet->at("key2").size());
            Assert::AreEqual(1, (int)resultSet->at("key1").count("1"));
            Assert::AreEqual(1, (int)resultSet->at("key2").count("4"));
        }

    };
}