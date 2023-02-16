#include "core/buffer.h"

namespace Icee {

Buffer::Buffer(size_t initial_cap) { buf_.reserve(initial_cap); }

void Buffer::Append(const unsigned char *new_char_data, size_t data_size) {
  buf_.insert(buf_.end(), new_char_data, new_char_data + data_size);
}

void Buffer::Append(const std::string &new_str_data) {
  Append(reinterpret_cast<const unsigned char *>(new_str_data.c_str()), new_str_data.size());
}

void Buffer::Append(std::vector<unsigned char> &&other_buffer) {
  buf_.insert(buf_.end(), std::make_move_iterator(other_buffer.begin()), std::make_move_iterator(other_buffer.end()));
}

void Buffer::AppendHead(const unsigned char *new_char_data, size_t data_size) {
  buf_.insert(buf_.begin(), new_char_data, new_char_data + data_size);
}

void Buffer::AppendHead(const std::string &new_str_data) {
  AppendHead(reinterpret_cast<const unsigned char *>(new_str_data.c_str()),
             new_str_data.size());
}

auto Buffer::FindAndPopTill(const std::string &target) -> std::optional<std::string> {
  std::optional<std::string> res = std::nullopt;
  auto content = ToStringView();  // 拿到 view 去查看是否是有 match 的情况
  auto pos = content.find(target);
  if (pos != std::string::npos) { // 找到了， 就留下以 target 为开头的数据
    res = content.substr(0, pos + target.size());
    buf_.erase(buf_.begin(), buf_.begin() + pos + target.size());
  }
  return res;
}


auto Buffer::ToStringView() const noexcept -> std::string_view {
  return {reinterpret_cast<const char*>(buf_.data()), buf_.size()};
}

auto Buffer::Size() const noexcept -> size_t { return buf_.size(); }

auto Buffer::Capacity() const noexcept -> size_t { return buf_.capacity(); }

auto Buffer::Data() noexcept -> const unsigned char * { return buf_.data(); }

void Buffer::Clear() noexcept { buf_.clear(); }


}  // namespace Icee
