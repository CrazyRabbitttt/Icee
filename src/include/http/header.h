#ifndef SRC_INCLUDE_HTTP_HEADER_H
#define SRC_INCLUDE_HTTP_HEADER_H

#include <string>

namespace Icee::Http {

/** The http header with the form "key : value"*/

class Header {
 public:
  Header(const std::string &key, const std::string &value);
  Header(std::string &&key, std::string &&value);   // copy constructor
  Header(const Header &other) = default;
  explicit Header(const std::string &header_line);       // 使用一个带有 colon 的 string 直接进行 Header 的构造
  Header(Header &&other) noexcept;
  auto operator=(const Header &other) -> Header& = default;
  auto operator=(Header &&other) noexcept -> Header &;
  ~Header() = default;

  auto IsValid() -> bool;
  auto GetKey() const -> std::string;
  auto GetValue() const -> std::string;
  void SetValue(const std::string &new_value) noexcept;
  /** serialize the key & value => "key:value\r\n"*/
  auto Serialize() const -> std::string;

 private:
  std::string key_;
  std::string value_;
  bool valid_{true};
};

}  // namespace Icee::Http

#endif