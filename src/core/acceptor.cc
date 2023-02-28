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
  acceptor_conn_->SetEvents(POLL_READ);  // 设置一下当前的 connection 当前的关注的事件是什么
  acceptor_conn_->SetLooper(listener);   // 本 connection 设置 EventLooper

  listener->AddAcceptor(acceptor_conn_.get());
  SetCustomAcceptCallback([](Connection *){});  // 传入的参数没有意义
  SetCustomHandleCallback([](Connection *){});
}

void Acceptor::BaseAcceptCallback(Connection *server_conn) {
  /** first: call the accept function(), that was it should do */
  NetAddress client_address;
  int accept_fd = server_conn->GetSocket()->Accept(client_address);  // 进行 accept
  if (accept_fd == -1) {
    return;
  }
  /** second, 创建 Socket & Connection */
  auto client_socket = std::make_unique<Socket>(accept_fd);
  client_socket->SetNonBlocking();
  auto client_connection = std::make_unique<Connection>(std::move(client_socket));

  /** third: 设置 Connection 的一些属性*/
  client_connection->SetEvents(POLL_ET | POLL_READ);  // ET 模式
  client_connection->SetCallback(GetCustomHandleCallback());    // 执行自定义的回调函数

  /** fourth: distribute the connection to any thread*/
  int index = rand() % event_loopers_.size();
  client_connection->SetLooper(event_loopers_[index]);  // 本连接属于哪一个 looper
  event_loopers_[index]->AddConnection(std::move(client_connection));
}

void Acceptor::SetCustomAcceptCallback(std::function<void(Connection *)> custom_accept_callback) {
  // point to the accept callback function
  accept_callback_ = std::move(custom_accept_callback);
  acceptor_conn_->SetCallback([this](auto &&ph1) {
    // 完美转发， 将类型转发给调用的函数 BaseAcceptCallback & accept_callback_
    BaseAcceptCallback(std::forward<decltype(ph1)>(ph1));
    accept_callback_(std::forward<decltype(ph1)>(ph1));
  });
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

auto Acceptor::GetAcceptorConnection() noexcept -> Connection * { return acceptor_conn_.get(); }

}  // namespace Icee