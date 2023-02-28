#include "core/connection.h"

#include "doctest/doctest.h"

#include <unistd.h>
#include <thread>
#include <memory>

#include "core/net_address.h"
#include "core/poller.h"
#include "core/socket.h"

using Icee::Connection;
using Icee::NetAddress;
using Icee::POLL_ADD;
using Icee::POLL_ET;
using Icee::POLL_READ;
using Icee::Socket;

TEST_CASE("[core/connection]") {
  NetAddress lock_host("127.0.0.1", 12138);
  auto server_socket = std::make_unique<Socket>();
  server_socket->Bind(lock_host);
  server_socket->Listen();

  REQUIRE(server_socket->GetFd() != -1);

  Connection server_conn(std::move(server_socket));
  REQUIRE(server_conn.GetSocket() != nullptr);

  SUBCASE("connection set events(which is this connection focus on) & return events") {
    server_conn.SetEvents(POLL_ADD | POLL_ET);
    CHECK((server_conn.GetEvents() & POLL_ADD));
    CHECK((server_conn.GetEvents() & POLL_ET));
    server_conn.SetRevents(POLL_READ);
    CHECK((server_conn.GetRevents() & POLL_READ));
  }

  SUBCASE("connection set call back & invoke it ") {
    server_conn.SetCallback([](Connection *) -> void {});  // nothing
    int i = 1;
    server_conn.SetCallback([&target = i](Connection *) -> void { target++; });

    // Call the call back function
    server_conn.GetCallback()();

    CHECK(i == 2);
  }

  SUBCASE("send and recv message from client through connection") {
//    char client_message[] = "message from client!";
//    char server_message[] = "message from server!hahaha";
//
//    std::thread t1([&]() {
//      auto client_socket = std::make_unique<Socket>();
//      client_socket->Connect(lock_host);
//      Connection client_conn(std::move(client_socket));
//
//      // send message to server
//      client_conn.WriteToWriteBuffer(client_message);
//      client_conn.Send();
//
//      // recv message from server
//      auto [size, exit] = client_conn.Recv();
//      CHECK(size == strlen(server_message));
//      CHECK(client_conn.ReadAsString() == std::string(server_message));
//    });
//
//    t1.detach();      // 自生自灭吧
//
//    NetAddress client_address;
//    auto connected_sock = std::make_unique<Socket>(server_conn.GetSocket()->Accept(client_address));
//    connected_sock->SetNonBlocking();
//    CHECK(connected_sock->GetFd() != -1);
//    Connection connected_conn(std::move(connected_sock));
//    sleep(1);
//    // recv a message from client
//    auto [read, exit] = connected_conn.Recv();
//    CHECK((read == strlen(client_message) && !exit));
//    CHECK(connected_conn.GetReadBufferSize() == strlen(client_message));
//    // send a message to client
//    connected_conn.WriteToWriteBuffer(server_message);
//    connected_conn.Send();
    const char *client_message = "from client！client was finish";
    const char *server_message = "hello from server";
    std::thread client_thread([&]() {
      // build a client connecting with server
      auto client_sock = std::make_unique<Socket>();
      client_sock->Connect(lock_host);
      Connection client_conn(std::move(client_sock));
      // send a message to server
      client_conn.WriteToWriteBuffer(client_message);
      std::cout << "before send:String write to server[" << client_conn.WriteAsString() << "]\n";
      CHECK(client_conn.GetWriteBufferSize() == strlen(client_message));
      client_conn.Send();
      // recv a message from server
      auto [read, exit] = client_conn.Recv();
      CHECK((read == strlen(server_message) && exit));
      CHECK(client_conn.ReadAsString() == std::string(server_message));
    });

    client_thread.detach();
    NetAddress client_address;
    auto connected_sock = std::make_unique<Socket>(server_conn.GetSocket()->Accept(client_address));
    connected_sock->SetNonBlocking();
    CHECK(connected_sock->GetFd() != -1);
    Connection connected_conn(std::move(connected_sock));
    sleep(1);
    // recv a message from client
    auto [read, exit] = connected_conn.Recv();
    std::cout << "Read as string:[" << connected_conn.ReadAsString() << "]" << std::endl;
    CHECK(read == strlen(client_message));
    CHECK(!exit);
    CHECK(connected_conn.GetReadBufferSize() == strlen(client_message));
    // send a message to client
    connected_conn.WriteToWriteBuffer(server_message);
    connected_conn.Send();
  }

}
