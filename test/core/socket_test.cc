#include "core/socket.h"

#include <fcntl.h>
#include <thread>

#include "core/net_address.h"
#include "doctest/doctest.h"

using Icee::NetAddress;
using Icee::Socket;

TEST_CASE("[core/socket]") {
  NetAddress local_host("127.0.0.1", 20080);
  Socket server_sock;

  // build the server socket
  server_sock.Bind(local_host);
  REQUIRE(server_sock.GetFd() != -1);
  server_sock.Listen();

  MESSAGE("move operations & switch fd");
  {
    int orig_fd = server_sock.GetFd();
    Socket another_sock(123);
    server_sock = std::move(another_sock);
    CHECK(server_sock.GetFd() == 123);
    CHECK(another_sock.GetFd() == orig_fd);

    Socket second_sock = std::move(another_sock);
    CHECK(second_sock.GetFd() == orig_fd);
    CHECK(another_sock.GetFd() == -1);
  }

  MESSAGE("non-blocking mode setting for socket");
  {
    Socket sock;
    sock.Bind(local_host);
    CHECK((sock.GetAttrs() & O_NONBLOCK) == 0);
    sock.SetNonBlocking();
    CHECK((sock.GetAttrs() & O_NONBLOCK) != 0);
  }

  MESSAGE("interaction between server and client socket");

  // 创建一个 client 线程去尝试联通 server
  {
    NetAddress client_address;
    std::thread client_thread = std::thread([&]() {
      Socket client_socket;
      client_socket.Connect(local_host);
    });
    client_thread.detach();
    CHECK(server_sock.Accept(client_address) != -1);
  }
}
