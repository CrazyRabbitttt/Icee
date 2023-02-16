#include "core/net_address.h"

namespace Icee {

NetAddress::NetAddress() noexcept {
  memset(&addr_, 0, sizeof(addr_));
  addr_len_ = sizeof(addr_);
}

NetAddress::NetAddress(const char *ip, uint16_t port, Protocol protocol) : protocol_(protocol) {
  memset(&addr_, 0, sizeof(addr_));
  addr_len_ = sizeof(addr_);

  if (protocol == Protocol::Ipv4) {
    auto addr_ipv4 = reinterpret_cast<struct sockaddr_in *>(&addr_);
    addr_ipv4->sin_family = AF_INET;
    inet_pton(AF_INET, ip, &addr_ipv4->sin_addr.s_addr);
    addr_ipv4->sin_port = htons(port);  // host to net, 网络序列
  } else {
    auto addr_ipv6 = reinterpret_cast<struct sockaddr_in6 *>(&addr_);
    addr_ipv6->sin6_family = AF_INET6;
    inet_pton(AF_INET6, ip, &addr_ipv6->sin6_addr.s6_addr);
    addr_ipv6->sin6_port = htons(port);
  }
}

auto NetAddress::GetProtocol() const -> Protocol { return protocol_; }

auto NetAddress::YieldAddr() -> struct sockaddr * { return &addr_; }

auto NetAddress::YieldAddrlen() -> socklen_t * { return &addr_len_; }

auto NetAddress::GetIp() const -> std::string {
  char ip_address[INET6_ADDRSTRLEN];
  if (protocol_ == Protocol::Ipv4) {
    auto addr_ipv4 = reinterpret_cast<struct sockaddr_in *>(&addr_);
    // convert the binary data to text
    inet_ntop(AF_INET, &addr_ipv4->sin_addr, ip_address, INET_ADDRSTRLEN);
  } else {
    auto addr_ipv6 = reinterpret_cast<struct sockaddr_in6 *>(&addr_);
    inet_ntop(AF_INET6, &addr_ipv6->sin6_addr, ip_address, INET6_ADDRSTRLEN);
  }
  return ip_address;
}

auto NetAddress::GetPort() const -> uint16_t {
  uint16_t port;
  if (protocol_ == Protocol::Ipv4) {
    auto addr_ipv4 = reinterpret_cast<struct sockaddr_in *>(&addr_);
    port = ntohs(addr_ipv4->sin_port);
  } else {
    auto addr_ipv4 = reinterpret_cast<struct sockaddr_in6 *>(&addr_);
    port = ntohs(addr_ipv4->sin6_port);
  }
  return port;
}

auto NetAddress::ToString() const -> std::string { return GetIp() + std::string(" @ ") + std::to_string(GetPort()); }

std::ostream &operator<<(std::ostream &os, const NetAddress &address) {
  os << address.ToString();
  return os;
}

}  // namespace Icee