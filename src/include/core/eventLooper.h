#ifndef SRC_INCLUDE_CORE_EVENT_LOOPER_H
#define SRC_INCLUDE_CORE_EVENT_LOOPER_H

#include <atomic>
#include <functional>
#include <memory>
#include <mutex>
#include <unordered_map>

#include "core/utils.h"

namespace Icee {

/** the epoll_wait timeout ? 超过 3s epoll_wait 便不会继续等待，直接 Return */
static const int TIMEOUT = 3000;

class Poller;

class ThreadPool;

class Connection;

class Acceptor;

/*
 * The event_loop acts ad the executor on a single thread
 * which adopt the philosophy of  'one loop per thread'
 */
class EventLooper {
 public:
  EventLooper();

  ~EventLooper() = default;

  NON_COPYABLE(EventLooper);

  void loop();

  /** 算是比较上层的操控【每一个线程一个Looper】，需要能够添加Acceptor & Connection*/

  void AddAcceptor(Connection *accept_conn);

  void AddConnection(std::unique_ptr<Connection> new_conn);

  auto DeleteConnection(int fd) -> bool;

  void SetExit() noexcept;

 private:
  std::unique_ptr<Poller> poller_;
  std::mutex latch_;
  std::unordered_map<int, std::unique_ptr<Connection> > connections_;  // fd ==> connection
  std::atomic<bool> exit_{false};
};

}  // namespace Icee

#endif
