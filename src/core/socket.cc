
#include "core/socket.h"

#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdexcept>

#include "core/net_address.h"

namespace Icee {

static constexpr int BACK_LOG = 128;

Socket::Socket() noexcept : fd_(-1) {}

Socket::Socket(int fd) noexcept : fd_(fd) {}

Socket::Socket(Socket &&other) noexcept {
  fd_ = other.fd_;
  other.fd_ = -1;
}

Socket &Socket::operator=(Socket &&other) noexcept {
  if (fd_ != -1) {
    close(fd_);
  }
  std::swap(fd_, other.fd_);
  return *this;
}

Socket::~Socket() {
  if (fd_ != -1) {
    close(fd_);
    fd_ = -1;
  }
}

auto Socket::GetFd() const noexcept -> int { return fd_; }

void Socket::Connect(NetAddress &server_address) {
  if (fd_ == -1) {
    CreateByProtocol(server_address.GetProtocol());
  }

  if (connect(fd_, server_address.YieldAddr(), *server_address.YieldAddrlen()) == -1) {
    throw std::logic_error("Socket: connect() error");
  }
}

void Socket::Bind(NetAddress &server_address, bool set_reusable) {
  if (fd_ == -1) {
    CreateByProtocol(server_address.GetProtocol());
  }
  if (set_reusable) {
    SetReusable();
  }
  if (bind(fd_, server_address.YieldAddr(), *server_address.YieldAddrlen()) == -1) {
    throw std::logic_error("Socker: bind() error");
  }
}

void Socket::Listen() {
  if (fd_ == -1) {
    throw std::logic_error("can not listen() with invalid fd");
  }
  listen(fd_, BACK_LOG);
}

auto Socket::Accept(NetAddress &client_address) -> int {
  if (fd_ == -1) {
    throw std::logic_error("can not accept() with invalid fd");
  }
  int client_fd = -1; // fill the result : client address
  client_fd = accept(fd_, client_address.YieldAddr(), client_address.YieldAddrlen());
  if (client_fd == -1) {
    // 高压的情况下可能会失败，但是 server 不能直接失败
  }
  return client_fd;
}

void Socket::SetReusable() {
  assert(fd_ != 1 && "can not setreusable with an invalid fd");
  int ok = 1;
  if (setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, &ok, sizeof ok) == -1 ||
      setsockopt(fd_, SOL_SOCKET, SO_REUSEPORT, &ok, sizeof ok) == -1) {
    std::logic_error("Socket: set reusable error");
  }
}

void Socket::SetNonBlocking() {
  assert(fd_ != -1 && "can not setNonBlocking with an invalid fd");
  auto old_file_message = fcntl(fd_, F_GETFL);
  if (fcntl(fd_, F_SETFL, old_file_message | O_NONBLOCK) == -1) {
    throw std::logic_error("Socker: set non blocking error");
  }
}

auto Socket::GetAttrs() -> int {
  assert(fd_ != -1);
  return fcntl(fd_, F_GETFL);
}

void Socket::CreateByProtocol(Protocol protocol) {
  // make the socket file descriptor
  if (protocol == Protocol::Ipv4) {
    fd_ = socket(AF_INET, SOCK_STREAM, 0);
  } else {
    fd_ = socket(AF_INET6, SOCK_STREAM, 0);
  }

  if (fd_ == -1) {
    throw std::logic_error("Socket: socket() error");
  }
}

}  // namespace Icee
