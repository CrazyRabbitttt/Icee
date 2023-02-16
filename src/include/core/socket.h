#ifndef SRC_INCLUDE_CORE_SOCKET_H
#define SRC_INCLUDE_CORE_SOCKET_H

#include "utils.h"

namespace Icee {

class NetAddress;  // only use the pointer of the netaddress

enum class Protocol;

/*
 * Socket method: bind, listen, accept. and attributes setting....
 */

class Socket {
 public:
  Socket() noexcept;

  explicit Socket(int fd) noexcept;

  NON_COPYABLE(Socket);

  Socket(Socket &&other) noexcept;  // 移动构造

  Socket &operator=(Socket &&other) noexcept;

  ~Socket();

  auto GetFd() const noexcept -> int;

  void Connect(NetAddress &server_address);

  void Bind(NetAddress &server_address, bool set_reusable = true);

  void Listen();

  auto Accept(NetAddress &client_address) -> int; // @Return : the file descriptor

  void SetReusable();

  void SetNonBlocking();

  // get the info of of the file(by fcntl())
  auto GetAttrs() -> int;

 private:
  void CreateByProtocol(Protocol protocol);

  int fd_{-1};
};

}  // namespace Icee

#endif
