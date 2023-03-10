
#include "core/poller.h"

#include <unistd.h>
#include <cstring>

#include "core/config.h"
#include "core/connection.h"

namespace Icee {

#ifdef OS_LINUX
Poller::Poller(uint64_t poll_size) : poll_size_(poll_size) {
  poll_fd_ = epoll_create1(0);
  if (poll_fd_ == -1) {
    perror("Poller: epoll_create1() error");
    exit(EXIT_FAILURE);
  }
  poll_events_ = new struct epoll_event[poll_size];
  memset(poll_events_, 0, poll_size_ * sizeof(struct epoll_event));
}
#elif OS_MAC
Poller::Poller(uint64_t poll_size) : poll_size_(poll_size) {
  poll_fd_ = kqueue();  // same to epoll_create
  if (poll_fd_ == -1) {
    perror("Poller: kqueue() error");
    exit(EXIT_FAILURE);
  }
  poll_events_ = new struct kevent[poll_size];
  memset(poll_events_, 0, poll_size + sizeof(struct kevent));
}
#endif

Poller::~Poller() {
  // 将申请的 poll_events 进行释放掉
  if (poll_fd_ != -1) {
    close(poll_fd_);
    delete[] poll_events_;
    poll_fd_ = -1;
  }
}

#ifdef OS_LINUX
auto Poller::AddConnection(Connection *conn) {
  assert(conn->GetFd() != -1 && "can not addConnection() with an invalid fd");
  struct epoll_event event;
  memset(event, 0, sizeof event);
  event.data.ptr = conn;
  event.events = conn->GetEvents();  // focus on read Or Write events
  int res = epoll_ctl(poll_fd_, POLL_ADD, conn->GetFd(), &event);
  if (ret_val == -1) {
    perror("Poller: epoll_ctl add error");
    exit(EXIT_FAILURE);
  }
}
#elif OS_MAC
void Poller::AddConnection(Connection *conn) {
  assert(conn->GetFd() != -1 && "cannot AddConnection() with an invalid fd");
  struct kevent event[1];
  memset(event, 0, sizeof(event));
  EV_SET(&event[0], conn->GetFd(), POLL_ADD, conn->GetEvents(), 0, 0,
         conn);  // read-trigger-only
  assert(kevent(poll_fd_, event, 1, nullptr, 0, nullptr) != -1 && "kevent add channel fails");
}
#endif

/*
 * If under the linux os, use the method of epoll
 *
 */
#ifdef OS_LINUX
auto Poller::Poll(int timeout) -> std::vector<Connection *> {
  std::vector<Connection *> events_happen;
  int ready = epoll_wait(poll_fd_, poll_events_, poll_size_, timeout);
  if (ready == -1) {
    perror("Poller: Poll() error");
    exit(EXIT_FAILURE);
  }
  for (int i = 0; i < ready; i++) {
    Connection *ready_connection = reinterpret_cast<Connection *>(
        poll_events_[i].data.ptr);  // 因为之前的 event.ptr 存储的是 connection， 所以这里可以取出来
    ready_connection->SetRevents(poll_events_[i].events);
    events_happen.emplace_back(ready_connection);
  }
  return events_happen;
}
#elif OS_MAC
auto Poller::Poll(int timeout) -> std::vector<Connection *> {
  std::vector<Connection *> events_happen;
  struct timespec t {};
  memset(&t, 0, sizeof(struct timespec));
  timeout = std::max(0, timeout);  // -1 is promoted to be no-wait
  t.tv_sec = timeout / 1000;
  t.tv_nsec = (timeout % 1000) * 1000 * 1000;

  int ready = kevent(poll_fd_, nullptr, 0, poll_events_, static_cast<int>(poll_size_), &t);
  if (ready == -1) {
    perror("Poller: Poll() error");
    exit(EXIT_FAILURE);
  }
  for (int i = 0; i < ready; i++) {
    auto *ready_connection = reinterpret_cast<Connection *>(poll_events_[i].udata);
    ready_connection->SetRevents(poll_events_[i].filter);
    events_happen.emplace_back(ready_connection);
  }
  return events_happen;
}
#endif

auto Poller::GetPollSize() const -> uint64_t { return poll_size_; }

}  // namespace Icee
