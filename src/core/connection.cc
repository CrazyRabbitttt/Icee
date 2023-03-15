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
  // lambda 能够使用到 call back & this object，return 的原因是可能需要传递返回值
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

auto Connection::WriteAsString() const noexcept -> std::string {
  auto string_view = write_buffer_->ToStringView();
  return {string_view.begin(), string_view.end()};
}

// 调用 Recv 其实是在数据已经准备好了的情况下， 然后不断的将数据拷贝到用户空间
auto Connection::Recv() -> std::pair<ssize_t, bool> {
  ssize_t recv_bytes{0};
  int from_fd = GetFd();
  unsigned char buf[TEMP_BUF_SIZE];  // 使用栈变量来进行数据的存放
  memset(buf, 0, sizeof buf);
  while (true) {  // ET 模式需要一次性将数据读取出来
    ssize_t cur_read = recv(from_fd, buf, TEMP_BUF_SIZE, 0);
    if (cur_read > 0) {
      recv_bytes += cur_read;
      WriteToReadBuffer(buf, cur_read);
      memset(buf, 0, sizeof buf);
    } else if (cur_read == 0) {  // 返回值是0，表示对端已关闭连接（也就是收到了 EOF）。
      return {recv_bytes, true};
    } else if (cur_read == -1 && errno == EINTR) {  // normal interrupt
      continue;
    } else if (cur_read == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)) {  // 所有的数据都被读取上来了,缓冲区没有数据了
      break;
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
  ssize_t write;
  const size_t left_write = GetWriteBufferSize();
  const unsigned char *buf = write_buffer_->Data();
  while (cur_write < left_write) {
    // 写缓冲区满了之后才发送，write 是缓冲区有数据就发送
    write = send(GetFd(), buf + cur_write, left_write - cur_write, 0);
    if (write <= 0) {
      // 那么就是真正的发生了 error
      if (errno != EINTR && errno != EAGAIN && errno != EWOULDBLOCK) {
        perror("Error in Connection::Send()");
        ClearWriteBuffer();
        return;
      }
      // 否则等一会而就行了（你的数据发送的太快了，网卡跟不上）
      // TODO：fix,注册写事件到 epoll 中，让操作系统去支持，不然其实就是 阻塞的了
      write = 0;
    }
    cur_write += write;
  }
  ClearWriteBuffer();
}

void Connection::ClearWriteBuffer() noexcept { write_buffer_->Clear(); }

void Connection::ClearReadBuffer() noexcept { read_buffer_->Clear(); }

void Connection::SetLooper(EventLooper *looper) noexcept { owner_looper_ = looper; }

auto Connection::GetLooper() -> EventLooper * { return owner_looper_; }

}  // namespace Icee
