#ifndef SRC_INCLUDE_CORE_ICEE_SERVER_H
#define SRC_INCLUDE_CORE_ICEE_SERVER_H

#include "core/acceptor.h"
#include "core/cache.h"
#include "core/connection.h"
#include "core/eventLooper.h"
#include "core/net_address.h"
#include "core/poller.h"
#include "core/threadPool.h"
#include "http/cgier.h"
#include "http/header.h"
#include "http/http_utils.h"
#include "http/request.h"
#include "http/response.h"
#include "log/logger.h"

namespace Icee {

class IceeServer {
 public:
  explicit IceeServer(NetAddress server_address,
                      int concurrent_number = static_cast<int>(std::thread::hardware_concurrency() - 1))
      : listener_(std::make_unique<EventLooper>()), thread_pool_(std::make_unique<ThreadPool>(concurrent_number)) {
    for (int i = 0; i < thread_pool_->GetSize(); i++) {
      reactors_.push_back(std::make_unique<EventLooper>());
    }
    // 将  Reactors 中的任务提交到线程池中去
    for (auto &reactor : reactors_) {
      thread_pool_->submit([capture = reactor.get()] { capture->loop(); });  // 执行 reactor 中的loop
    }
    // 构造 acceptor, 获得原始的 reactor 指针（相对于shared_ptr性能好一些）
    std::vector<EventLooper *> raw_reactors{};
    raw_reactors.reserve(reactors_.size());

    //    for (auto &it : reactors_) {
    //      raw_reactors.emplace_back(it.get());
    //    }
    std::transform(reactors_.begin(), reactors_.end(), std::back_inserter(raw_reactors),
                   [](auto &uni_ptr) { return uni_ptr.get(); });
    acceptor_ = std::make_unique<Acceptor>(listener_.get(), raw_reactors, server_address);
  }

  virtual ~IceeServer() = default;

  auto OnAccept(std::function<void(Connection *)> on_accept) -> IceeServer & {
    acceptor_->SetCustomAcceptCallback(std::move(on_accept));
    return *this;
  }

  auto OnHandle(std::function<void(Connection *)> on_handle) -> IceeServer & {
    acceptor_->SetCustomHandleCallback(std::move(on_handle));
    seted_On_handle_ = true;
    return *this;
  }

  void Begin() {
    LOG_INFO("hello from log info()");
    LOG_DEBUG("hello from log debug...");
    if (!seted_On_handle_) {
      printf("feguy\n");
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
