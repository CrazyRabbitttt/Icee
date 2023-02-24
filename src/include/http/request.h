#ifndef SRC_INCLUDE_HTTP_REQUEST_H
#define SRC_INCLUDE_HTTP_REQUEST_H

#include "core/utils.h"
#include "http/http_utils.h"

#include <vector>

namespace Icee::Http {

class Header;

enum class Version;
enum class Method;

class Request {
 public:
  Request(Method method, Version version, std::string resource_url, const std::vector<Header> &headers) noexcept;

  explicit Request(const std::string &request_str) noexcept;

  NON_COPYABLE(Request);
  auto IsValid() const noexcept -> bool;
  auto ShouldClose() const noexcept -> bool;
  auto GetInvalidReason() const noexcept -> std::string;
  auto GetMethod() const noexcept -> Method;
  auto GetVersion() const noexcept -> Version;
  auto GetResourceUrl() const noexcept -> std::string;
  auto GetHeaders() const noexcept -> std::vector<Header>;

 private:
  /** 传入request line，将其拆分称为 Method Url Version */
  auto ParseRequestLine(const std::string &request_line) -> bool;

  /** 检查一下当前的 connection 是不是 Keep-alive */
  void CheckConnectionState(const Header &header);

  Method method_;
  Version version_;
  std::string url_;
  std::vector<Header> headers_;  // vector 中的竟然也是可以使用前置声明的
  bool is_valid_{false};
  bool should_close_{true};
  std::string invalid_reason_;
};

}  // namespace Icee::Http

#endif