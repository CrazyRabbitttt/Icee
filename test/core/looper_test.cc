#include "core/eventLooper.h"

#include "doctest/doctest.h"

#include "core/connection.h"
#include "core/net_address.h"
#include "core/poller.h"
#include "core/socket.h"

#include <unistd.h>
#include <atomic>
#include <memory>
#include <numeric>
#include <thread>
#include <vector>

using Icee::Connection;
using Icee::EventLooper;
using Icee::NetAddress;
using Icee::POLL_ADD;
using Icee::POLL_ET;
using Icee::POLL_READ;
using Icee::Socket;

TEST_CASE("[core/eventlopper]") {
  EventLooper looper;
  // build the server socket
  NetAddress local_host("127.0.0.1", 20080);
  Socket server_sock;
  server_sock.Bind(local_host);
  server_sock.Listen();
  REQUIRE(server_sock.GetFd() != -1);

  SUBCASE("looper execute connection's customized callback func") {
    std::atomic<int> var = 0;
    int client_num = 3;
    std::vector<std::thread> threads;
    for (int i = 0; i < client_num; i++) {
      threads.emplace_back([&host = local_host]() {
        auto client_socket = Socket();
        client_socket.Connect(host);
        sleep(1);
      });
    }

    // build 3 connections and add into looper with customized callback function
    std::vector<int> reach(client_num, 0);
    for (int i = 0; i < client_num; i++) {
      NetAddress client_address;
      auto client_sock = std::make_unique<Socket>(server_sock.Accept(client_address));
      CHECK(client_sock->GetFd() != -1);
      client_sock->SetNonBlocking();
      auto client_conn = std::make_unique<Connection>(std::move(client_sock));
      client_conn->SetEvents(POLL_READ);
      client_conn->SetCallback([&reach = reach, index = i](Connection *conn) { reach[index] = 1; });
      looper.AddConnection(std::move(client_conn));
    }

    /* the looper execute each client's callback once, upon their exit */
    std::thread runner([&]() { looper.loop(); });
    sleep(2);
    looper.SetExit();

    /* each client's callback should have already been executed */
    CHECK(std::accumulate(reach.begin(), reach.end(), 0) == client_num);

    runner.join();
    for (int i = 0; i < client_num; i++) {
      threads[i].join();
    }
  }
}
