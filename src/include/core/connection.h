#ifndef SRC_INCLUDE_CORE_CONNECTION_H
#define SRC_INCLUDE_CORE_CONNECTION_H

#include <functional>
#include <memory>

#include "core/buffer.h"
#include "core/socket.h"
#include "core/utils.h"

#define TEMP_BUF_SIZE 2048

namespace Icee {

class EventLooper;  // 本Connection属于哪一个EventLooper

/*
 * 封装了 socket 的各种 method（如读写），并且掌控了 socket 所关注的 event 能够供
 * Poller所使用
 *
 * Handle the member variable : Socket & Buffer
 * User can set the call back function to each Connection
 *
 */

class Connection {
 public:
  explicit Connection(std::unique_ptr<Socket> socket);
  ~Connection() = default;

  NON_COPYABLE(Connection);

  auto GetFd() const -> int;

  auto GetSocket() -> Socket *;

  /* Methods for Poller，关注读写 */
  void SetEvents(uint32_t events);
  auto GetEvents() const noexcept -> uint32_t;
  void SetRevents(uint32_t revents);
  auto GetRevents() const noexcept -> uint32_t;

  void SetCallback(const std::function<void(Connection *)> &callback);
  auto GetCallback() noexcept -> std::function<void()>;

  /* Methods for Buffer */
  auto GetReadBufferSize() const -> size_t;

  auto GetWriteBufferSize() const -> size_t;

  void WriteToReadBuffer(const unsigned char *buf, size_t size);
  void WriteToWriteBuffer(const unsigned char *buf, size_t size);
  void WriteToReadBuffer(const std::string &str);
  void WriteToWriteBuffer(const std::string &str);
  void WriteToWriteBuffer(std::vector<unsigned char> &&other_buf);

  auto Read() const -> const unsigned char *;
  auto ReadAsString() const noexcept -> std::string;

  auto Recv() -> ssize_t;
  void Send();

  void ClearWriteBuffer() noexcept;
  void ClearReadBuffer() noexcept;

  void SetLooper(EventLooper *looper) noexcept;
  auto GetLooper() -> EventLooper *;

 private:
  /** 本 Connection 属于哪一个 Looper */
  EventLooper *owner_looper_{nullptr};
  /** 本 Connection 对应的 client socket*/
  std::unique_ptr<Socket> socket_;
  std::unique_ptr<Buffer> read_buffer_;
  std::unique_ptr<Buffer> write_buffer_;
  uint32_t events_{0};
  uint32_t revents_{0};
  std::function<void()> call_back_{nullptr};
};

}  // namespace Icee

#endif