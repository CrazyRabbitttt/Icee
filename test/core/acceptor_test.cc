#include "core/acceptor.h"

#include "doctest/doctest.h"

#include <unistd.h>

#include <future>  // NOLINT
#include <memory>
#include <vector>


#include "core/connection.h"
#include "core/eventLooper.h"
#include "core/net_address.h"
#include "core/threadPool.h"

using Icee::NetAddress;
using Icee::Connection;
using Icee::NetAddress;
using Icee::ThreadPool;
using Icee::EventLooper;
using Icee::Acceptor;


TEST_CASE("[core/acceptor]") {
  NetAddress local_host("127.0.0.1", 12138);

  ThreadPool pool;

  // build the reactor for listening looper
  auto single_reactor = std::make_unique<EventLooper>();

  std::vector<EventLooper*> raw_reactors = {single_reactor.get()};
  auto acceptor = Acceptor(single_reactor.get(), raw_reactors, local_host);

  REQUIRE(acceptor.GetAcceptorConnection()->GetFd() != -1);





}























