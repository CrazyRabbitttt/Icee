#ifndef ICEE_CORE_INCLUDE_JOINER_H_
#define ICEE_CORE_INCLUDE_JOINER_H_

#include <thread>
#include <vector>

namespace Icee {

using std::thread;
using std::vector;

class Joiner {
 public:
  explicit Joiner(vector<thread> &threads) : threads_(threads) {}
  ~Joiner() {
    for (auto &&th : threads_) {
      if (th.joinable()) {
        th.join();
      }
    }
  }

 private:
  vector<thread> &threads_;
};

}  // namespace icee

#endif