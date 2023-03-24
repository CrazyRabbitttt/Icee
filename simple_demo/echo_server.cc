#include "core/icee_server.h"
#include "log/logger.h"

int main() {
  Icee::NetAddress local_address("0.0.0.0", 20080);
  Icee::IceeServer echo_server(local_address);
  echo_server
      .OnHandle([&](Icee::Connection *client_conn) {
        int from_fd = client_conn->GetFd();
        auto [read, exit] = client_conn->Recv();
        if (exit) {
          client_conn->GetLooper()->DeleteConnection(from_fd);
          // client_conn ptr is invalid below here, do not touch it again
          return;
        }
        if (read) {
          client_conn->WriteToWriteBuffer(client_conn->ReadAsString());
          client_conn->Send();
          client_conn->ClearReadBuffer();
        }
      });
  echo_server.Begin();

}
