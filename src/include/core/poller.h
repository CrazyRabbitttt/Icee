#ifndef SRC_INCLUDE_CORE_POLLER_H
#define SRC_INCLUDE_CORE_POLLER_H

#include "core/config.h"

#ifdef OS_LINUX
#include <sys/epoll.h>
#endif
#ifdef OS_MAC
#include <sys/event.h>  // mac 本地使用 kqueue 来进行 epoll
#endif

#include <vector>

#include "core/utils.h"

namespace Icee {

// the maximum number of events to be listened on epoll

static const int DEFAULT_EVENT_LISTENED = 1024;
#ifdef OS_LINUX  // Linux Epoll
static constexpr unsigned POLL_ADD = EPOLL_CTL_ADD;
static constexpr unsigned POLL_READ = EPOLLIN;
static constexpr unsigned POLL_ET = EPOLLET;
#elif  OS_MAC
static const unsigned POLL_ADD = EVFILT_READ;  // a bit awkward but this is how kqueue works
static const unsigned POLL_READ = EV_ADD;
static const unsigned POLL_ET = EV_CLEAR;
#endif

class Connection;  // hold the pointer of the connection class

/*
 *  轮询的判断 socket 的读写事件
 */
class Poller {
 public:
  explicit Poller(uint64_t poll_size = DEFAULT_EVENT_LISTENED);

  ~Poller();

  NON_COPYABLE(Poller);

  /*
   * 在 Linux 下 epoll_event.data.ptr 能够存储数据，那么就可以将这个 Connection
   * 作为 event 的 data 放进 Poller 中
   */
  void AddConnection(Connection *conn);

  auto Poll(int timeout = -1) -> std::vector<Connection *>;

  auto GetPollSize() const -> uint64_t;

 private:
  int poll_fd_;
  uint64_t poll_size_;
  struct kevent *poll_events_{nullptr};
};

}  // namespace Icee

#endif
