#include "core/eventLooper.h"

#include "core/connection.h"
#include "core/poller.h"
#include "core/threadPool.h"

namespace Icee {

EventLooper::EventLooper() : poller_(std::make_unique<Poller>()) {}

void EventLooper::loop() {
  while (!exit_) {
    auto read_connections = poller_->Poll(TIMEOUT);
    for (auto &it : read_connections) {
      it->GetCallback()();  // 执行回调函数
    }
  }
}

void EventLooper::AddAcceptor(Connection *accept_conn) {
  std::unique_lock<std::mutex> lock(latch_);
  // acceptor 也是可以看作是一个 Connection，但是并不会加入到 map 中去的
  poller_->AddConnection(accept_conn);
}

void EventLooper::AddConnection(std::unique_ptr<Connection> new_conn) {
  std::unique_lock<std::mutex> lock(latch_);
  poller_->AddConnection(new_conn.get());
  int fd = new_conn->GetFd();
  connections_.insert({fd, std::move(new_conn)});
}

auto EventLooper::DeleteConnection(int fd) -> bool {
  std::unique_lock<std::mutex> lock(latch_);
  auto index = connections_.find(fd);
  if (index == connections_.end()) {
    return false;
  }
  connections_.erase(index);
  return true;
}

void EventLooper::SetExit() noexcept { exit_ = true; }

}  // namespace Icee
