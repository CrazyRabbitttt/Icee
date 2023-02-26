#include "core/connection.h"

#include <sys/socket.h>

namespace Icee {

Connection::Connection(std::unique_ptr<Socket> socket)
    : socket_(std::move(socket)), read_buffer_(std::make_unique<Buffer>()), write_buffer_(std::make_unique<Buffer>()) {}

auto Connection::GetFd() const -> int { return socket_->GetFd(); }

auto Connection::GetSocket() -> Socket * { return socket_.get(); }

void Connection::SetEvents(uint32_t events) { events_ = events; }

auto Connection::GetEvents() const noexcept -> uint32_t { return events_; }

void Connection::SetRevents(uint32_t revents) { revents_ = revents; }

auto Connection::GetRevents() const noexcept -> uint32_t { return revents_; }

void Connection::SetCallback(const std::function<void(Connection *)> &callback) {
  // lambda 能够使用到 call back & this object
  call_back_ = [callback, this] { return callback(this); };
}

auto Connection::GetCallback() noexcept -> std::function<void()> { return call_back_; }

/* Buffer*/
auto Connection::GetWriteBufferSize() const -> size_t { return write_buffer_->Size(); }

auto Connection::GetReadBufferSize() const -> size_t { return read_buffer_->Size(); }

void Connection::WriteToReadBuffer(const unsigned char *buf, size_t size) { read_buffer_->Append(buf, size); }

void Connection::WriteToWriteBuffer(const unsigned char *buf, size_t size) { write_buffer_->Append(buf, size); }

void Connection::WriteToReadBuffer(const std::string &str) { read_buffer_->Append(str); }

void Connection::WriteToWriteBuffer(const std::string &str) { write_buffer_->Append(str); }

void Connection::WriteToWriteBuffer(std::vector<unsigned char> &&other_buf) {
  write_buffer_->Append(std::move(other_buf));
}

auto Connection::FindAndPopTill(const std::string &target) -> std::optional<std::string> {
  return read_buffer_->FindAndPopTill(target);
}

auto Connection::Read() const -> const unsigned char * { return read_buffer_->Data(); }

auto Connection::ReadAsString() const noexcept -> std::string {
  auto string_view = read_buffer_->ToStringView();
  return {string_view.begin(), string_view.end()};
}

auto Connection::Recv() -> std::pair<ssize_t, bool> {
  ssize_t recv_bytes{0};
  int from_fd = GetFd();
  unsigned char buf[TEMP_BUF_SIZE];  // 使用栈变量来进行数据的存放
  memset(buf, 0, sizeof buf);
  while (true) {
    ssize_t cur_read = recv(from_fd, buf, TEMP_BUF_SIZE, 0);
    if (cur_read > 0) {
      recv_bytes += cur_read;
      WriteToReadBuffer(buf, cur_read);
      memset(buf, 0, sizeof buf);
    } else if (cur_read == 0) { // 返回值是0，表示对端已关闭连接（也就是收到了 EOF）。
      return {recv_bytes, true};
    } else if (cur_read == -1 && errno == EINTR) {
      continue;
    } else if (cur_read == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)) { // 普通的错误, 可能就是缓冲区满了，等一会就好了
      break;  // 应该被阻塞，那么就代表所有的数据都被读取上来了
    } else {
      perror("HandleConnection: recv() error");
      return {recv_bytes, true};  // 出现了 error， client 已经是退出了
    }
  }
  return {recv_bytes, false};
}

// 将存在于 buffer 中的数据发送出去
void Connection::Send() {
  size_t cur_write{0};
  const size_t left_write = GetWriteBufferSize();
  const unsigned char *buf = write_buffer_->Data();
  while (cur_write < left_write) {
    auto write_size = send(GetFd(), buf, sizeof buf, 0);
    if (write_size <= 0) {
      // 那么就是真正的发生了 error
      if (errno != EINTR && errno != EAGAIN && errno != EWOULDBLOCK) {
        perror("Error in Connection::Send()");
        ClearWriteBuffer();
        return;
      }
      // 否则等一会而就行了
      write_size = 0;
    }
    cur_write += write_size;
  }
  ClearWriteBuffer();
}

void Connection::ClearWriteBuffer() noexcept { write_buffer_->Clear(); }

void Connection::ClearReadBuffer() noexcept { read_buffer_->Clear(); }

void Connection::SetLooper(EventLooper *looper) noexcept { owner_looper_ = looper; }

auto Connection::GetLooper() -> EventLooper * { return owner_looper_; }

}  // namespace Icee
