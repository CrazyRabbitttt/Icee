#ifndef SRC_INCLUDE_HTTP_RESPONSE_H_
#define SRC_INCLUDE_HTTP_RESPONSE_H_

#include <optional>
#include <string>
#include <vector>

namespace Icee::Http {

class Header;

class Response {
 public:
  static auto Make200Response(bool should_close, std::optional<std::string> url) -> Response;

  /* 400 Bad Request response, close connection */
  static auto Make400Response() noexcept -> Response;
  /* 404 Not Found response, close connection */
  static auto Make404Response() noexcept -> Response;
  /* 503 Service Unavailable response, close connection */
  static auto Make503Response() noexcept -> Response;

  Response(const std::string &status_code, bool should_close, std::optional<std::string> url);

  auto GetHeaders() noexcept -> std::vector<Header>;

  auto ChangeHeader(const std::string &key, const std::string &new_value) noexcept -> bool;

  void Serialize(std::vector<unsigned char> &buffer);

  void SetShouldTransferContent(bool should_transfer_content) noexcept;

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
  bool should_transfer_content_;
  std::optional<std::string> url_;
  std::vector<unsigned char> body_;
};

}  // namespace Icee::Http

#endif