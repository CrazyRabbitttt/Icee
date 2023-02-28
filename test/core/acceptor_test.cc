#include "core/acceptor.h"

#include "doctest/doctest.h"

#include <unistd.h>

#include <future>  // NOLINT
#include <memory>
#include <vector>

#include "core/connection.h"
#include "core/eventLooper.h"
#include "core/net_address.h"
#include "core/poller.h"
#include "core/socket.h"
#include "core/threadPool.h"


using Icee::NetAddress;
using Icee::Connection;
using Icee::NetAddress;
using Icee::ThreadPool;
using Icee::EventLooper;
using Icee::Acceptor;
using Icee::Socket;
using Icee::Poller;


TEST_CASE("[core/acceptor]") {
  NetAddress local_host("127.0.0.1", 12138);

  ThreadPool pool;

  // build the reactor for listening looper
  auto single_reactor = std::make_unique<EventLooper>();  // listener event loop

  std::vector<EventLooper*> raw_reactors = {single_reactor.get()};
  auto acceptor = Acceptor(single_reactor.get(), raw_reactors, local_host);

  REQUIRE(acceptor.GetAcceptorConnection()->GetFd() != -1);

  SUBCASE("Acceptor should be able to accept the connection & callback") {
    int client_num = 3;
    std::atomic<int> accept_trigger = 0;
    std::atomic<int> handle_trigger = 0;

    // set acceptor customize functions
    acceptor.SetCustomAcceptCallback([&](Connection *) { accept_trigger++; });
    acceptor.SetCustomHandleCallback([&](Connection *) { handle_trigger++; });

    // start three clients and connect with server
    const char *msg = "Hello from client!";
    std::vector<std::future<void>> futs;
    for (int i = 0; i < client_num; i++) {
      auto fut = std::async(std::launch::async, [&]() { // 新创建一个线程去执行函数（子线程请求连接 & 发送部分信息）
        Socket client_sock;
        client_sock.Connect(local_host);
        CHECK(client_sock.GetFd() != -1);
        send(client_sock.GetFd(), msg, strlen(msg), 0);
      });
      futs.push_back(std::move(fut));
    }

    auto runner = std::async(std::launch::async, [&]() { single_reactor->loop(); });
    futs.push_back(std::move(runner));
    sleep(2);
    single_reactor->SetExit();  // terminate the looper=

    // accept & handle should be triggered exactly 3 times
    CHECK(accept_trigger == client_num);
    CHECK(handle_trigger >= client_num);

    for (auto &f : futs) {
      f.wait();
    }

  }

}



