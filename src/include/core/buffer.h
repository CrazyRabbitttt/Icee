#ifndef SRC_INCLUDE_CORE_BUFFER_H
#define SRC_INCLUDE_CORE_BUFFER_H

#include <optional>
#include <string>
#include <vector>

#include "utils.h"

namespace Icee {

/* 默认的 buffer size, 如果需要更大的使用 栈空间来申请 */
static const size_t INITIAL_BUFFER_SIZE = 1024;


class Buffer {
 public:
  explicit Buffer(size_t initial_cap = INITIAL_BUFFER_SIZE);

  ~Buffer() = default;

  Buffer(const Buffer& other) = default;

  Buffer&operator=(const Buffer& other) = default;

  NON_MOVEABLE(Buffer);

  // Append method to append data to the buffer

  void Append(const unsigned char *new_char_data, size_t data_size);

  void Append(const std::string &new_str_data);

  void Append(std::vector<unsigned char> &&other_buffer);

  void AppendHead(const unsigned char *new_char_data, size_t data_size);

  void AppendHead(const std::string &new_str_data);

  auto FindAndPopTill(const std::string &target) -> std::optional<std::string>;

  auto Size() const noexcept -> size_t ;

  auto Capacity() const noexcept -> size_t ;

  auto Data() noexcept -> const unsigned char *;

  // 如果只是想要打印数据而不会【修改】的话，可以直接使用 string_view 来表示
  auto ToStringView() const noexcept -> std::string_view;

  void Clear() noexcept;


 private:
  std::vector<unsigned char> buf_;
};


}

#endif
