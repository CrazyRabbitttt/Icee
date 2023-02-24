#ifndef SRC_INCLUDE_HTTP_HEADER_H
#define SRC_INCLUDE_HTTP_HEADER_H

#include "http/http_utils.h"
#include "core/utils.h"

namespace Icee::Http {


class Header;
enum class Version;
enum class Method;


class Request {

 public:
  Request(Method method, Version version, std::string resource_url, const std::vector<Header> &headers) noexcept;

  explicit Request(const std::string &request_str) noexcept;

  NON_COPYABLE(Request);


 private:
  Method method_;
  Version version_;
  std::string url_;
  std::vector<Header> headers_; // vector 中的竟然也是可以使用前置声明的
  bool is_valid{false};
};














}





#endif