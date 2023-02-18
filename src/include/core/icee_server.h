#ifndef SRC_INCLUDE_CORE_ICEE_SERVER_H
#define SRC_INCLUDE_CORE_ICEE_SERVER_H

#include "core/acceptor.h"
#include "core/connection.h"
#include "core/eventLooper.h"
#include "core/net_address.h"
#include "core/poller.h"
#include "core/threadPool.h"

namespace Icee {

class IceeServer {
 public:
  explicit IceeServer(NetAddress server_address,
                      int concurrent_number = static_cast<int>(std::thread::hardware_concurrency() - 1))
      : thread_pool_(std::make_unique<ThreadPool>(concurrent_number)), listener_(std::make_unique<EventLooper>()) {
    for (int i = 0; i < concurrent_number; i++) {
      reactors_.push_back(std::make_unique<EventLooper>());
    }
    // 将  Reactors 中的任务提交到线程池中去
    for (auto &reactor : reactors_) {
      thread_pool_->submit([capture = reactor.get()] { capture->loop(); });
    }
    // 构造 acceptor
    std::vector<EventLooper*> raw_reactors{};
    for (auto& it : reactors_) {
      raw_reactors.emplace_back(it.get());
    }
    acceptor_ = std::make_unique<Acceptor>(listener_.get(), raw_reactors, server_address);
  }

  virtual ~IceeServer() = default;

  auto OnAccept(std::function<void(Connection *)> on_accept) -> IceeServer & {
    acceptor_->SetCustomAcceptCallback(std::move(on_accept));
    return *this;
  }

  auto OnHandle(std::function<void(Connection*)> on_handle) -> IceeServer &{
    acceptor_->SetCustomHandleCallback(std::move(on_handle));
    seted_On_handle_ = true;
    return *this;
  }

  void Begin() {
    if (!seted_On_handle_) {
      throw std::logic_error("Should specify onHandle callback function before starts");
    }
    listener_->loop();
  }

 private:
  bool seted_On_handle_{false};
  std::vector<std::unique_ptr<EventLooper>> reactors_;  // one loop(this) per thread,
  std::unique_ptr<Acceptor> acceptor_;
  std::unique_ptr<EventLooper> listener_;
  std::unique_ptr<ThreadPool> thread_pool_;
};

}  // namespace Icee

#endif
