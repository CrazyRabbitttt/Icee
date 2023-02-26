#include "http/response.h"

#include <sstream>

#include "http/header.h"
#include "http/http_utils.h"

namespace Icee::Http {

/*
 * 创建一个 200 OK 的 response
 */

auto Response::Make200Response(bool should_close, std::optional<std::string> url) -> Response {
  assert(url.has_value());
  return {RESPONSE_OK, should_close, std::move(url.value())};
}

auto Response::Make400Response() noexcept -> Response { return {RESPONSE_BAD_REQUEST, true, std::nullopt}; }

auto Response::Make404Response() noexcept -> Response { return {RESPONSE_NOT_FOUND, true, std::nullopt}; }

auto Response::Make503Response() noexcept -> Response { return {RESPONSE_SERVICE_UNAVAILABLE, true, std::nullopt}; }

Response::Response(const std::string &status_code, bool should_close, std::optional<std::string> url)
    : should_close_(should_close), url_(std::move(url)) {
  // make the status line
  std::stringstream ss;
  ss << HTTP_VERSION << SPACE << status_code;
  status_line_ = ss.str();

  /** 添加常见的 headers */
  headers_.emplace_back(HEADER_SERVER, SERVER_ICEE);  // Server:Icee/1.0
  /** 是否是长连接*/
  headers_.emplace_back(HEADER_CONNECTION, (should_close_) ? CONNECTION_CLOSE : CONNECTION_KEEP_ALIVE);
  /** 如果是获取文件的长度 & Type*/
  if (url_.has_value() && IsFileExist(url_.value())) {
    auto file_size = FileSize(url_.value());
    /** 添加上 file_size 的属性*/
    headers_.emplace_back(HEADER_CONTENT_LENGTH, std::to_string(file_size));
    /** 添加 file type 的属性(需要找到 url 的后缀对应的 type)*/
    auto index = url_->find_last_of(Dot);
    if (index != std::string::npos) {
      auto raw_string = url_.value().substr(index + 1);  // 直接从 index + 1 开始到结束,
      auto extension = ToExtension(raw_string);
      headers_.emplace_back(HEADER_CONTENT_TYPE, ExtensionToMime(extension));
    }
  } else {  // 如果没有文件的话
    url_ = std::nullopt;
    headers_.emplace_back(HEADER_CONTENT_LENGTH, "0");
  }
}

void Response::Serialize(std::vector<unsigned char> &buffer) {
  /** 将所有的非 body 的部分组合进buffer中*/
  std::stringstream ss;
  ss << status_line_ << CRLF; // response line
  // 2.all of the headers
  for (auto &it : headers_) {
    auto header_line = it.Serialize();  // key:value\r\n
    ss << header_line;
  }
  ss << CRLF;
  // 3.将除了 body 之外的数据先写到 buffer 中
  std::string basic_data = ss.str();
  buffer.insert(buffer.end(), basic_data.begin(), basic_data.end());    // push back the basic data to the buffer

  // 4.将请求的 File 数据写入到 buffer 中
  if (url_.has_value()) {
    LoadFile(url_.value(), buffer);
  }
}

auto Response::GetHeaders() noexcept -> std::vector<Header> { return headers_; }

void Response::SetShouldTransferContent(bool should_transfer_content) noexcept {
  should_transfer_content_ = should_transfer_content;
}

auto Response::ChangeHeader(const std::string &key, const std::string &new_value) noexcept -> bool {
  for (auto &it : headers_) {
    if (it.GetKey() == key) {
      it.SetValue(new_value);
      return true;
    }
  }
  return false;
}

}  // namespace Icee::Http
