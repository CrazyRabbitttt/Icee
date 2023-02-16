#ifndef ICEE_INCLUDE_CORE_QUEUE_H_
#define ICEE_INCLUDE_CORE_QUEUE_H_

#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>

namespace Icee {

using std::condition_variable;
using std::mutex;
using std::queue;

template <typename T>
class ThreadSafeQueue {
 public:
  void Push(T new_value) {
    auto p = std::make_shared<T>(std::move(new_value));
    std::lock_guard<std::mutex> lk(m_mtx);
    m_queue.push(p);
    m_cond.notify_one();  // 1
  }

  void WaitAndPop(T &value)  // 2
  {
    std::unique_lock<std::mutex> lk(m_mtx);
    m_cond.wait(lk, [this] { return !m_queue.empty(); });
    value = std::move(*(m_queue.front()));
    m_queue.pop();
  }

  std::shared_ptr<T> WaitAndPop()  // 3
  {
    std::unique_lock<std::mutex> lk(m_mtx);
    m_cond.wait(lk, [this] { return !m_queue.empty(); });  // 4
    auto ret = m_queue.front();
    m_queue.pop();
    return ret;
  }

  bool TryPop(T &value) {
    std::lock_guard<std::mutex> lk(m_mtx);
    if (m_queue.empty()) return false;
    value = std::move(*(m_queue.front()));
    m_queue.pop();
    return true;
  }

  std::shared_ptr<T> TryPop() {
    std::lock_guard<std::mutex> lk(m_mtx);
    if (m_queue.empty()) return std::shared_ptr<T>();  // 5
    auto res = m_queue.front();
    m_queue.pop();
    return res;
  }

  bool Empty() const {
    std::lock_guard<std::mutex> lk(m_mtx);
    return m_queue.empty();
  }

 private:
  mutable mutex m_mtx;
  queue<std::shared_ptr<T> > m_queue;
  condition_variable m_cond;
};

}  // namespace Icee
#endif
