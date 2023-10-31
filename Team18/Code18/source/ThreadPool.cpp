#include "ThreadPool.h"

void ThreadPool::wait(bool removeAfterDone) {
  for (auto it = threads.begin(); it != threads.end(); ++it) {
    if (it->joinable()) {
      it->join();
    }
  }
  threads.clear();
}

int ThreadPool::getPoolSize() {
  return threads.size();
}