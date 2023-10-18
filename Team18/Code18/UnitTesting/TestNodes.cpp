#include "stdafx.h"
#include "CppUnitTest.h"
#include "../source/SP/SimpleProcessor/SimpleProcessor.h"
#include "../source/SP/AST/Node.h"
#include <cassert>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

static std::shared_ptr<Node> deepNodeGenerator(int levels, int branchingFactor) {
  std::shared_ptr<Node> node = nullptr;
  std::vector<std::shared_ptr<Node>> prevNodes = {};
  for (int i = 0; i < levels; i++) {
    int stmtNumber = i % 2;
    std::string value = i % 2 == 0 ? "abc" : "def";
    node = std::make_shared<Node>(stmtNumber, value, prevNodes);
    prevNodes = {};
    for (int j = 0; j < branchingFactor; j++) {
      prevNodes.push_back(node);
    }
  }
  return node;
}

namespace Nodes_Test
{
  TEST_CLASS(isIdentical_Test) {
  public:
    TEST_METHOD(noChildren_success) {
      int statementNum = 1;
      std::string value = "abc";
      std::vector<std::shared_ptr<Node>> children = {

      };
      Node n1 = Node(statementNum, value, children);
      Node n2 = Node(statementNum, value, children);
      bool strictChecking = true;
      assert(n1.isIdentical(&n2, strictChecking));
    }

    TEST_METHOD(nonStrict_success) {
      int statementNum = 1;
      std::string value = "abc";
      std::vector<std::shared_ptr<Node>> children = {

      };
      Node n1 = Node(statementNum, value, children);
      Node n2 = Node(statementNum + 1, value, children);
      bool strictChecking = false;
      assert(n1.isIdentical(&n2, strictChecking));
    }

    TEST_METHOD(someChildren_success) {
      int statementNum = 1;
      std::string value = "abc";
      int childStmtNum = 2;
      std::string childVal = "def";
      std::vector<std::shared_ptr<Node>> children1 = {
        std::make_shared<Node>(childStmtNum, childVal, std::vector<std::shared_ptr<Node>>())
      };
      std::vector<std::shared_ptr<Node>> children2 = {
        std::make_shared<Node>(childStmtNum, childVal, std::vector<std::shared_ptr<Node>>())
      };
      Node n1 = Node(statementNum, value, children1);
      Node n2 = Node(statementNum, value, children2);
      bool strictChecking = true;
      assert(n1.isIdentical(&n2, strictChecking));
    }

    TEST_METHOD(nestedChildren_success) {
      std::shared_ptr<Node> n1 = deepNodeGenerator(5, 2);
      std::shared_ptr<Node> n2 = deepNodeGenerator(5, 2);
      bool strictChecking = true;
      assert(n1->isIdentical(n2, strictChecking));
    }

    TEST_METHOD(diffStatementNum_failure) {
      int statementNum = 1;
      std::string value = "abc";
      std::vector<std::shared_ptr<Node>> children = {

      };
      Node n1 = Node(statementNum, value, children);
      Node n2 = Node(statementNum + 1, value, children);
      bool strictChecking = true;
      assert(!n1.isIdentical(&n2, strictChecking));
    }

    TEST_METHOD(diffStringVal_failure) {
      int statementNum = 1;
      std::string value = "abc";
      std::vector<std::shared_ptr<Node>> children = {

      };
      Node n1 = Node(statementNum, value, children);
      Node n2 = Node(statementNum, "", children);
      bool strictChecking = true;
      assert(!n1.isIdentical(&n2, strictChecking));
    }

    TEST_METHOD(diffChildren_failure) {
      int statementNum = 1;
      std::string value = "abc";
      int childStmtNum = 2;
      std::string childVal = "def";
      std::vector<std::shared_ptr<Node>> children1 = {
        std::make_shared<Node>(childStmtNum, childVal, std::vector<std::shared_ptr<Node>>())
      };
      std::vector<std::shared_ptr<Node>> children2 = {
        std::make_shared<Node>(childStmtNum, value, std::vector<std::shared_ptr<Node>>())
      };
      Node n1 = Node(statementNum, value, children1);
      Node n2 = Node(statementNum, value, children2);
      bool strictChecking = true;
      assert(!n1.isIdentical(&n2, strictChecking));
    }
  };

  TEST_CLASS(isSubTreeOf_Test) {
  public:
    TEST_METHOD(identical_success) {
      int statementNum = 1;
      std::string value = "abc";
      std::vector<std::shared_ptr<Node>> children = {

      };
      Node n1 = Node(statementNum, value, children);
      Node n2 = Node(statementNum, value, children);
      bool strictChecking = true;
      assert(n1.isSubtreeOf(&n2, strictChecking));
    }

    TEST_METHOD(subtree_success) {
      int statementNum = 1;
      std::string value = "abc";
      std::vector<std::shared_ptr<Node>> children = {

      };
      Node n1 = Node(statementNum, value, children);
      Node n2 = Node(statementNum + 1, "", { std::make_shared<Node>(statementNum, value, children) });
      bool strictChecking = true;
      assert(n1.isSubtreeOf(&n2, strictChecking));
    }

    TEST_METHOD(nonStrict_success) {
      int statementNum = 1;
      std::string value = "abc";
      std::vector<std::shared_ptr<Node>> children = {

      };
      Node n1 = Node(statementNum, value, children);
      Node n2 = Node(statementNum + 1, "", { std::make_shared<Node>(statementNum + 2, value, children) });
      bool strictChecking = false;
      assert(n1.isSubtreeOf(&n2, strictChecking));
    }

    TEST_METHOD(wrongDirectionSubtree_failure) {
      int statementNum = 1;
      std::string value = "abc";
      std::vector<std::shared_ptr<Node>> children = {

      };
      Node n1 = Node(statementNum, value, children);
      Node n2 = Node(statementNum + 1, "", { std::make_shared<Node>(statementNum, value, children) });
      bool strictChecking = true;
      assert(!n2.isSubtreeOf(&n1, strictChecking));
    }

    TEST_METHOD(diffChildren_failure) {
      int statementNum = 1;
      std::string value = "abc";
      int childStmtNum = 2;
      std::string childVal = "def";
      std::vector<std::shared_ptr<Node>> children1 = {
        std::make_shared<Node>(childStmtNum, childVal, std::vector<std::shared_ptr<Node>>())
      };
      std::vector<std::shared_ptr<Node>> children2 = {
        std::make_shared<Node>(childStmtNum, value, std::vector<std::shared_ptr<Node>>())
      };
      Node n1 = Node(statementNum, value, children1);
      Node n2 = Node(statementNum, value, children2);
      bool strictChecking = true;
      assert(!n1.isSubtreeOf(&n2, strictChecking));
    }

    TEST_METHOD(nestedChildren_success) {
      std::shared_ptr<Node> n1 = deepNodeGenerator(1, 2);
      std::shared_ptr<Node> n2 = deepNodeGenerator(5, 2);
      bool strictChecking = true;
      assert(n1->isSubtreeOf(n2, strictChecking));
    }

    TEST_METHOD(nestedChildren_failure) {
      std::shared_ptr<Node> n1 = deepNodeGenerator(2, 3);
      std::shared_ptr<Node> n2 = deepNodeGenerator(5, 2);
      bool strictChecking = true;
      assert(!n1->isSubtreeOf(n2, strictChecking));
    }
  };
}