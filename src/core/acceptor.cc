#include "core/acceptor.h"

#include "core/connection.h"
#include "core/eventLooper.h"
#include "core/net_address.h"
#include "core/poller.h"

namespace Icee {

Acceptor::Acceptor(EventLooper *listener, std::vector<EventLooper *> reactors, NetAddress server_address)
    : event_loopers_(std::move(reactors)) {
  // 首先 创建 Socket、Bind、Listen（使用server_address）
  auto acceptor_socket = std::make_unique<Socket>();
  acceptor_socket->Bind(server_address, true);  // internal scope will make socket & bind
  acceptor_socket->Listen();

  acceptor_conn_ = std::make_unique<Connection>(std::move(acceptor_socket));
  acceptor_conn_->SetEvents(POLL_READ);                                  // 关注读事件
  acceptor_conn_->SetLooper(reinterpret_cast<EventLopper *>(listener));  // 属于 Listener 这个 Looper

  listener->AddAcceptor(acceptor_conn_.get());
}

void Acceptor::BaseAcceptCallback(Connection *server_conn) {
  /** first: call the accept function(), that was it should do */
  NetAddress client_address;
  int accept_fd = server_conn->GetSocket()->Accept(client_address); // 进行 accept
  if (accept_fd == -1) {
    return;
  }
  /** second, 创建 Socket & Connection */
  auto client_socket = std::make_unique<Socket>(accept_fd);
  client_socket->SetNonBlocking();
  auto client_connection = std::make_unique<Connection>(std::move(client_socket));

  /** third: 设置 Connection 的一些属性*/
  client_connection->SetEvents()


}

void Acceptor::SetCustomHandleCallback(std::function<void(Connection *)> custom_handle_callback) {
  handle_callback_ = std::move(custom_handle_callback);
}

auto Acceptor::GetCustomAcceptCallback() const noexcept -> std::function<void(Connection *)> {
  return accept_callback_;
}

auto Acceptor::GetCustomHandleCallback() const noexcept -> std::function<void(Connection *)> {
  return handle_callback_;
}

}  // namespace Icee