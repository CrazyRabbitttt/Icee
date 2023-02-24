#ifndef SRC_INCLUDE_HTTP_RESPONSE_H_
#define SRC_INCLUDE_HTTP_RESPONSE_H_

#include <optional>
#include <string>
#include <vector>

namespace Icee::Http {

class Header;

class Response {
 public:
  Response(const std::string &status_code, bool should_close, std::optional<std::string>& url);

  /*
   * 基本的 response 至少是需要有 version status_code status_line
   * headers
   * body
   */
 private:
  std::string status_line_;
  std::vector<Header> headers_;
  /** 这次的连接是不是 HTTP 长连接 */
  bool should_close_;
  std::optional<std::string> url_;
  std::vector<unsigned char> body_;
};

}  // namespace Icee::Http

#endif