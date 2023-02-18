#ifndef SRC_INCLUDE_CORE_ACCEPTOR_H
#define SRC_INCLUDE_CORE_ACCEPTOR_H

/*
 * Acceptor 专门用来设定新的 client 连接并且设定好 customer handle function for clients
 */
#include <vector>

#include "core/utils.h"
#include "core/eventLooper.h"

namespace Icee {

class Connection;
class EventLooper;
class NetAddress;

/*
 * 接受新的连接请求并且需要分配给不同的 Poller， 分配给不同的线程去处理
 */
class Acceptor {
 public:
  Acceptor(EventLooper *listener, std::vector<EventLooper *> reactors, NetAddress server_address);

  ~Acceptor() = default;

  NON_COPYABLE(Acceptor);

  /** 基本的 accept 回调函数(当 listen 的读事件发生的会掉函数)， 分配连接到不同的线程 EventLooper 中去*/
  void BaseAcceptCallback(Connection *server_conn);

  void SetCustomAcceptCallback(std::function<void(Connection *)> custom_accept_callback);

  void SetCustomHandleCallback(std::function<void(Connection *)> custom_handle_callback);

  auto GetCustomAcceptCallback() const noexcept -> std::function<void(Connection *)>;

  auto GetCustomHandleCallback() const noexcept -> std::function<void(Connection *)>;

  auto GetAcceptorConnection() noexcept -> Connection *;

 private:
  std::vector<EventLooper *> event_loopers_;
  std::unique_ptr<Connection> acceptor_conn_;  // 这个就是专门负责连接客户端的 Connection:[socket:server's socket]
  std::function<void(Connection *)> accept_callback_{};
  std::function<void(Connection *)> handle_callback_{};
};

}  // namespace Icee

#endif
