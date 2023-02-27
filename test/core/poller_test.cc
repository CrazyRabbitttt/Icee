#include "core/poller.h"

#include <unistd.h>

#include <memory>
#include <thread>  // NOLINT
#include <vector>

#include "core/connection.h"
#include "core/net_address.h"
#include "core/socket.h"
#include "doctest/doctest.h"

/* for convenience reason */
using Icee::Connection;
using Icee::NetAddress;
using Icee::POLL_ADD;
using Icee::POLL_ET;
using Icee::POLL_READ;
using Icee::Poller;
using Icee::Socket;

TEST_CASE("[core/poller]") {
  NetAddress local_host("127.0.0.1", 12138);

  // build the server socket
  Socket server_socket;
  server_socket.Bind(local_host);
  server_socket.Listen();

  REQUIRE(server_socket.GetFd() != 0);
  int client_num = 4;

  Poller poller(client_num);

  SUBCASE("able to poll out the client's messages sent over") {
    std::vector<std::thread> threads;
    for (int i = 0; i < client_num; i++) {
      threads.emplace_back([&]() {
        auto client_socket = Socket();
        client_socket.Connect(local_host);
        char message[] = "Hello from client!";
        send(client_socket.GetFd(), message, strlen(message), 0);
        sleep(2);
      });
    }

    // server accept clients and build connection for them
    std::vector<std::shared_ptr<Connection>> client_conns;
    for (int i = 0; i < client_num; i++) {
      NetAddress client_address;
      auto client_sock = std::make_unique<Socket>(server_socket.Accept(client_address));
      CHECK(client_sock->GetFd() != -1);
      client_conns.push_back(std::make_shared<Connection>(std::move(client_sock)));
      client_conns[i]->SetEvents(POLL_READ);
    }

    // each client connection under poller's monitor
    for (int i = 0; i < client_num; i++) {
      poller.AddConnection(client_conns[i].get());
    }
    sleep(1);
    auto ready_conns = poller.Poll();
    CHECK(ready_conns.size() == client_num);

    for (int i = 0; i < client_num; i++) {
      threads[i].join();
    }
  }
}
