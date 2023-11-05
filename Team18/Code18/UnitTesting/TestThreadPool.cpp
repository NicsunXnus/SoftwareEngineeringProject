#include "stdafx.h"
#include "CppUnitTest.h"

#include <iostream>
#include <string>
#include <exception>
#include "../source/ThreadPool.h"
#include "../source/ApplicationWrapper.h"
#include "../source/SP/SimpleProcessor/SimpleProcessor.h"
#include "../source/SP/SimpleProcessor/ExpressionProcessor.h"
#include "../source/HelperFunctions.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std::string_view_literals;

namespace Multithreading_Test
{
	TEST_CLASS(dummyFunctions_Tests) {
	private:
		void slowFunc1() {
			std::cout << "slow 1 called!" << std::endl;
			int i = 0;
			while (i < 10000) {
				i += 1;
			}
			std::cout << "slow 1 ended!" << std::endl;
		}
		void slowFunc2() {
			std::cout << "slow 2 called!" << std::endl;
			int i = 0;
			while (i < 10000000) {
				i += 1;
			}
			std::cout << "slow 2 ended!" << std::endl;
		}
	public:
		// Test 1: While loop that does nothing meaningful
		TEST_METHOD(test1_no_multithread) {
			try {
				for (int i = 0; i < 100; i++) {
					slowFunc2();
				}
			}
			catch (exception e) {
				assert(false);
			}
			assert(true);
		}
		TEST_METHOD(test1_multithread) {
			ThreadPool tp{};
			try {
				for (int i = 0; i < 100; i++) {
					// Need to add `this` in the [] to indicate that the variables for this class
					// are added to the scope of the lambda
					tp.addTask([this]() {slowFunc2(); });
				}
				tp.wait();
			}
			catch (exception e) {
				assert(false);
			}
			assert(true);
		}
	};
	
	TEST_CLASS(compatibility_classless) {
	public:
		// Test 1 : Classless functions (ie helper functions in HelperFunctions.h)
		TEST_METHOD(test1_no_multithread) {
			// The below 3 lines are just to generate the string to use
			string sample = ".";
			for (int i = 0; i < 10; i++) {
				sample = sample.append(sample);
			}

			try {
				for (int i = 0; i < 10; i++) {
					splitString(sample, ".", true);
				}
			}
			catch (exception e) {
				assert(false);
			}
			assert(true);
		}
		TEST_METHOD(test1_multithread) {
			// The below 3 lines are just to generate the string to use
			string sample = ".";
			for (int i = 0; i < 10; i++) {
				sample = sample.append(sample);
			}

			ThreadPool tp{};
			try {
				for (int i = 0; i < 10; i++) {
					// Need to add & in the [] to indicate that the variables in this scope are added to the scope of the
					// lambda
					tp.addTask([&]() {splitString(sample, ".", true); });
				}
				tp.wait();
			}
			catch (exception e) {
				assert(false);
			}
			assert(true);
		}

	};

	class ExpressionProcessorExposer : public ExpressionProcessor {
	public:
		static void tokenizeArithmeticExpression(std::string expression) {
			ExpressionProcessor::tokenizeArithmeticExpression(expression);
		}
	};
	TEST_CLASS(compatibility_static) {
public:
	// Test 1 : Static functions
	TEST_METHOD(test1_no_multithread) {
		string sample = "((a + ((b + c) / d) * (((e % f)))))";
		try {
			for (int i = 0; i < 10; i++) {
				ExpressionProcessorExposer::tokenizeArithmeticExpression(sample);
			}
		}
		catch (exception e) {
			assert(false);
		}
		assert(true);
	}
	TEST_METHOD(test1_multithread) {
		string sample = "((a + ((b + c) / d) * (((e % f)))))";

		ThreadPool tp{};
		try {
			for (int i = 0; i < 10; i++) {
				tp.addTask(ExpressionProcessorExposer::tokenizeArithmeticExpression, sample);
			}
			tp.wait();
		}
		catch (exception e) {
			assert(false);
		}
		assert(true);
	}

	};
	
	TEST_CLASS(compatibility_object) {
	public:
		// Test 1 : Object Methods (large method)
		// Can't get test 1 to work for the stress test file, no idea why
		TEST_METHOD(test1_no_multithread) {
			ApplicationWrapper applicationWrapper;
			try {
				for (int i = 0; i < 3; i++) {
					applicationWrapper.parse("../../Tests18/Sample_source.txt", true);
				}
			}
			catch (exception e) {
				assert(false);
			}
			assert(true);
		}
		TEST_METHOD(test1_multithread) {
			ApplicationWrapper applicationWrapper;
			ThreadPool tp{};
			try {
				for (int i = 0; i < 3; i++) {
					tp.addTask(&ApplicationWrapper::parse, &applicationWrapper, "../../Tests18/Sample_source.txt", true);
				}
				tp.wait();
			}
			catch (exception e) {
				assert(false);
			}
			assert(true);
		}

		// TEST 2 and 3 shows the difference in overhead for creating the threads
		// Tests 2, 3 : Small object methods
		TEST_METHOD(test2_no_multithread) {
			// The below 5 lines are just to generate the string to use
			string sample = "a+";
			for (int i = 0; i < 7; i++) {
				sample = sample.append(sample);
			}
			sample.append("a");

			ExpressionProcessor ep;
			try {
				for (int i = 0; i < 5; i++) {
					ep.nodifyArithmeticExpression(sample);
				}
			}
			catch (exception e) {
				assert(false);
			}
			assert(true);
		}

		TEST_METHOD(test2_multithread) {
			// The below 5 lines are just to generate the string to use
			string sample = "a+";
			for (int i = 0; i < 7; i++) {
				sample = sample.append(sample);
			}
			sample.append("a");

			ExpressionProcessor ep;
			ThreadPool tp{};
			try {
				for (int i = 0; i < 5; i++) {
					tp.addTask(&ExpressionProcessor::nodifyArithmeticExpression, &ep, sample);
				}
				tp.wait();
			}
			catch (exception e) {
				assert(false);
			}
			assert(true);
		}
		TEST_METHOD(test3_no_multithread) {
			// The below 5 lines are just to generate the string to use
			string sample = "a+";
			for (int i = 0; i < 7; i++) {
				sample = sample.append(sample);
			}
			sample.append("a");

			ExpressionProcessor ep;
			try {
				for (int i = 0; i < 25; i++) {
					ep.nodifyArithmeticExpression(sample);
				}
			}
			catch (exception e) {
				assert(false);
			}
			assert(true);
		}

		TEST_METHOD(test3_multithread) {
			// The below 5 lines are just to generate the string to use
			string sample = "a+";
			for (int i = 0; i < 7; i++) {
				sample = sample.append(sample);
			}
			sample.append("a");

			ExpressionProcessor ep;
			ThreadPool tp{};	
			try {
				for (int i = 0; i < 25; i++) {
					tp.addTask(&ExpressionProcessor::nodifyArithmeticExpression, &ep, sample);
				}
				tp.wait();
			}
			catch (exception e) {
				assert(false);
			}
			assert(true);
		}

	};
}