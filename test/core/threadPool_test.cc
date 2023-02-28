#include "core/threadPool.h"

#include <atomic>
#include <iostream>

#include "doctest/doctest.h"

using Icee::ThreadPool;

TEST_CASE("[core/thread_pool]") {
  static int thread_pool_size = 8;
  ThreadPool pool(thread_pool_size);
  REQUIRE(pool.GetSize() == thread_pool_size);
  {
    std::atomic<int> var{0};

    ThreadPool local_pool(thread_pool_size);
    for (int i = 0; i < 3 * thread_pool_size; i++) {
      local_pool.submit([&]() { var++; });
    }
    std::cout << "The number of var: " << var << std::endl;
    //    CHECK(var == 3 * thread_pool_size);
  }
}
