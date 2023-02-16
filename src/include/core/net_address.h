#ifndef SRC_INCLUDE_CORE_NET_ADDRESS_H
#define SRC_INCLUDE_CORE_NET_ADDRESS_H

#include <arpa/inet.h>

#include <cstring>
#include <iostream>
#include <string>

namespace Icee {

/*
 * NetAddress 封装了 Ip + port 作为唯一的标识
 */

enum class Protocol { Ipv4, Ipv6 };

class NetAddress {
 public:
  NetAddress() noexcept;

  NetAddress(const char *ip, uint16_t port, Protocol protocol = Protocol::Ipv4);

  ~NetAddress() = default;

  auto GetProtocol() const -> Protocol;

  auto YieldAddr() -> struct sockaddr *;

  auto YieldAddrlen() -> socklen_t *;

  auto GetIp() const -> std::string;

  auto GetPort() const -> uint16_t;

  auto ToString() const -> std::string;

 private:
  Protocol protocol_{Protocol::Ipv4};
  mutable struct sockaddr addr_{};
  socklen_t addr_len_;
};


std::ostream &operator<<(std::ostream &os, const NetAddress &address);

}  // namespace Icee

#endif
