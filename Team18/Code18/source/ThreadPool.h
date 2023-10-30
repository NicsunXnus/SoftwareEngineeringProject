#ifndef MULTITHREADING_H
#define MULTITHREADING_H

#include <thread>
#include <vector>
#include <functional>

using namespace std;

// This class implements a very generic threadpool class.
// Usage instructions:
// 1. Intiailise ThreadPool with the default constructor
// 2. Add tasks to ThreadPool using the addTask method. Once added, the functions should start running.
// 3. Call wait() to ensure all threads have been joined.
class ThreadPool {
private:
  std::vector<thread> threads = {};
public:
  ThreadPool() {};

  /// <summary>
  /// Adds a task to be done by a thread. This method is non-blocking.
  /// 
  /// Simply add the task and an available thread will pick it up
  /// automatically. There are 4 cases to how to add a task, depending on the type of function used:
  /// 1. Methods defined in the same class
  /// 2. Methods not defined in any class (ie in just a .cpp or .h file)
  /// 3. Static methods
  /// 4. Object methods
  /// 
  /// == ADD METHOD FOR 1. Methods defined in the same class
  /// threadPoolObj.addTask([this]() {methodName(methodArg, methodArg2); });
  /// 
  /// The above is simply wrapping the method to call in a lambda, with the inclusion of this object's
  /// scope into the lambda.
  /// 
  /// == ADD METHOD FOR 2. Methods not defined in any class (ie in just a .cpp or .h file)
  /// threadPoolObj.addTask([&]() {classlessMethod(methodArg, methodArg2); });
  /// 
  /// The above is simply wrapping the method to call in a lambda, with the inclusion of the current
  /// scope into the lambda.
  /// 
  /// == ADD METHOD FOR 3. Static methods
  /// threadPoolObj.addTask(OtherClass::staticMethod, methodArg, methodArg2);
  /// 
  /// == ADD METHOD FOR 4. Object methods
  /// OtherClass oc;
  /// threadPoolObj.addTask(&OtherClass::memberMethod, &oc, methodArg, methodArg2);
  /// 
  /// Note that method 4 can be buggy when performing very computationally intensive tasks (ie parsing stress test source)
  /// 
  /// </summary>
  /// 
  /// <remark>
  /// Examples of the usages can also be seen in the Unit Tests file for this class
  /// </remark>
  /// <typeparam name="Function">A generic function to use, with any return type</typeparam>
  /// <typeparam name="...Args">Any number of generic inputs to the above function</typeparam>
  /// <param name="func">The function to be added to the threadpool</param>
  /// <param name="...args">The arguments to the function to be added to the threadpool</param>
  template <typename Function, typename... Args>
  void addTask(Function&& func, Args&&... args) {
    threads.emplace_back(forward<Function>(func), forward<Args>(args)...);
  }

  /// <summary>
  /// Waits until all tasks have been finished in the current pool. This method is blocking.
  /// </summary>
  /// <param name="removeAfterDone">Whether to clear the pool after all tasks are done</param>
  void wait(bool removeAfterDone = false);
  
  // Returns the size of the threadpool
  int getPoolSize();
};

#endif