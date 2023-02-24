#include "http/response.h"

#include <sstream>

#include "http/header.h"
#include "http/http_utils.h"

namespace Icee::Http {

Response::Response(const std::string &status_code, bool should_close, std::optional<std::string> &url)
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
      auto raw_string = url_.value().substr(index + 1);  // 直接从 index + 1 开始到结束
      auto extension = ToExtension(raw_string);
      headers_.emplace_back(HEADER_CONTENT_TYPE, ExtensionToMime(extension));
    }
  } else {  // 如果没有文件的话
    url_ = std::nullopt;
    headers_.emplace_back(HEADER_CONTENT_LENGTH, "0");
  }
}

}  // namespace Icee::Http
