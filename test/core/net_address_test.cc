#include "core/net_address.h"

#include "doctest/doctest.h"

using Icee::NetAddress;
using Icee::Protocol;

TEST_CASE("[core/netaddress]") {
  MESSAGE("Ipv4 net address");
  {
    NetAddress address_ipv4("127.0.0.1", 20080, Protocol::Ipv4);
    REQUIRE(address_ipv4.GetProtocol() == Protocol::Ipv4);
    REQUIRE(address_ipv4.GetIp() == std::string("127.0.0.1"));
    REQUIRE(address_ipv4.GetPort() == 20080);
    REQUIRE(address_ipv4.ToString() == std::string("127.0.0.1 @ 20080"));
  }

  MESSAGE("Ipv6 net address");
  {
    NetAddress address_ipv6("2001:db8:3333:4444:5555:6666:7777:8888", 40080, Protocol::Ipv6);
    REQUIRE(address_ipv6.GetProtocol() == Protocol::Ipv6);
    REQUIRE(address_ipv6.GetIp() == std::string("2001:db8:3333:4444:5555:6666:7777:8888"));
    REQUIRE(address_ipv6.GetPort() == 40080);
    REQUIRE(address_ipv6.ToString() == std::string("2001:db8:3333:4444:5555:6666:7777:8888 @ 40080"));
  }
}
