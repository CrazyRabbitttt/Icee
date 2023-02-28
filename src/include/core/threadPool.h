#ifndef ICEE_INCLUDE_THREAD_POOL_H
#define ICEE_INCLUDE_THREAD_POOL_H

#include <functional>
#include <future>
#include <memory>
#include <thread>
#include <vector>

#include "core/joiner.h"
#include "core/threadSafeQueue.h"

namespace Icee {
using std::future;
using std::packaged_task;
using std::thread;
using std::unique;
using std::vector;

class ThreadPool {
 public:
  explicit ThreadPool(uint32_t thread_num = std::thread::hardware_concurrency()) : done_(false), joiner_(threads_) {
    try {
      for (int i = 0; i < thread_num; i++) {
        threads_.emplace_back(&ThreadPool::worker_thread, this);
      }
    } catch (...) {  // 产生异常进行捕获
      done_ = true;
      throw std::runtime_error("add thread error");
    }
  }

  ~ThreadPool() { done_ = true; }

  template <typename FunctionType>
  void submit(FunctionType func) {
    queue_.Push(std::function<void()>(func));
  }

  auto GetSize() -> int { return threads_.size(); }

 private:
  void worker_thread() {
    while (!done_) {
      std::function<void()> task;
      if (queue_.TryPop(task)) {  // 从任务队列中取任务
        task();
      } else {
        std::this_thread::yield();  // 当前线程挂起，避免争抢资源
      }
    }
  }

 private:
  std::atomic<bool> done_;
  ThreadSafeQueue<std::function<void()> > queue_;
  vector<thread> threads_;
  Joiner joiner_;
};

}  // namespace Icee

#endif