#include "http/request.h"

#include "http/http_utils.h"

#include "http/header.h"

namespace Icee::Http {

Request::Request(Method method, Version version, std::string resource_url, const std::vector<Header> &headers) noexcept
    : method_(method), version_(version), url_(std::move(resource_url)), headers_(headers), is_valid_(true) {}

Request::Request(const std::string &request_str) noexcept {
  // 根据 \r\n 将报文进行分割
  auto tokens = Split(request_str, CRLF);
  if (tokens.size() < 2 || !tokens.back().empty()) {
    invalid_reason_ = "Request format wrong.";
    return;
  }

  if (!tokens.back().empty()) {
    invalid_reason_ = "End of request's token should be empty";
    return;
  }

  tokens.pop_back();
  bool request_parse_success = ParseRequestLine(tokens[0]);
  if (!request_parse_success) {
    invalid_reason_ = "Request line error";
    return ;
  }
  tokens.erase(tokens.begin());
  for (auto &line : tokens) {
    Header header{line};    // 利用 Header 直接初始化 string 来进行解析
    if (!header.IsValid()) {
      invalid_reason_ = "Fail to parse header line:" + line;
      return;
    }
    CheckConnectionState(header);
    headers_.push_back(std::move(header));    // 将得到的 header push 进 vector 中
  }
  is_valid_ = true; // check whether the request is valid
}

void Request::CheckConnectionState(const Header &header) {
  auto key = Format(header.GetKey());
  if (key == Format(HEADER_CONNECTION)) {
    auto value = Format(header.GetValue());
    if (value == Format(CONNECTION_KEEP_ALIVE)) {   // 如果说出现了 keep-alive, 那么就不能够在回送报文后直接关闭
      should_close_ = false;
    }
  }
}

auto Request::ShouldClose() const noexcept -> bool { return should_close_; }

auto Request::IsValid() const noexcept -> bool { return is_valid_; }

auto Request::GetMethod() const noexcept -> Method { return method_; }

auto Request::GetVersion() const noexcept -> Version { return version_; }

auto Request::GetResourceUrl() const noexcept -> std::string {
  return url_;
}
auto Request::GetHeaders() const noexcept -> std::vector<Header> {
  return headers_;
}
auto Request::GetInvalidReason() const noexcept -> std::string {
  return invalid_reason_;
}


auto Request::ParseRequestLine(const std::string &request_line) -> bool {
  /*
   * Get www.baidu.com HTTP/1.0
   */
  auto tokens = Split(request_line);  // split by the separator => SPACE
  if (tokens.size() != 3) {
    invalid_reason_ = "The first request is invalid:[" + request_line + "]";
    return false;
  }

  /** check the first token: Method*/
  method_ = ToMethod(tokens[0]);
  if (method_ == Method::UNSUPPORTED) {
    invalid_reason_ = "Invalid Method[unsupported method]";
    return false;
  }

  /** check the third token: Version*/
  version_ = ToVersion(tokens[2]);
  if (version_ == Version::UNSUPPORTED) {
    invalid_reason_ = "Invalid Version[unsupported version]";
    return false;
  }

  url_ = (tokens[1].empty() || tokens[1].back() == '/') ? tokens[1] + DEFAULT_ROUTE : tokens[1];
  return true;
}

}  // namespace Icee::Http