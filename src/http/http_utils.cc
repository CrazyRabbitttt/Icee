#include "http/http_utils.h"

namespace Icee::Http {

/*
 * origin string => "hello:this:new:world:!" ， 通过
 * 需要被分割符号进行分割，{hello, this, new, world}
 */
auto Split(const std::string &str, const char *delim) -> std::vector<std::string> {
  std::vector<std::string> res;
  if (str.empty()) {
    return res;
  }
  int begin_index{0};
  size_t target_index;
  auto delim_len = strlen(delim);
  while ((target_index = str.find(delim, begin_index)) != std::string::npos) {  // 只要是能够进行分割出数据
    res.emplace_back(str.substr(begin_index, target_index - begin_index));
    begin_index = target_index + delim_len;
  }
  /*
   * corner case: "hello:world:haha"  =>
   *                          ^【在这里开始] 但是没有再找到 : 了，还是需要加上 haha
   * */
  if (begin_index != str.size()) {
    res.emplace_back(str.substr(begin_index, str.size() - begin_index));
  }
  return std::move(res);
}

auto Join(const std::vector<std::string> &tokens, const char *deli) noexcept -> std::string {
  assert(!tokens.empty());
  std::string res{};
  if (tokens.size() == 1) {
    return tokens[0];
  }
  auto n = tokens.size();
  for (int i = 0; i < n; i++) {
    if (i != n - 1) {
      res += ":";
    }
    res += tokens[i];
  }
  return res;
}


auto Trim(const std::string &str, const char* delim) -> std::string {
  // 将首位的属于集合中的字符给删除掉，一般是 space，也就是清除两端的空白
  auto first_index = str.find_first_not_of(delim);
  auto last_index = str.find_last_not_of(delim);
  if (last_index == std::string::npos) {
    return {};
  }
  return str.substr(first_index, last_index - first_index + 1);
}

auto ToUpper(std::string str) -> std::string {
  std::transform(str.begin(), str.end(), str.begin(), [](char c) { return toupper(c); });
  return str;
}

auto Format(const std::string &str) noexcept->std::string {
  return ToUpper(Trim(str));
}

auto ToMethod(const std::string &method_str) noexcept -> Method {
  auto method_formatted = Format(method_str);
  if (method_formatted == Method_String_Map.at(Method::GET)) {
    return Method::GET;
  }
  if (method_formatted == Method_String_Map.at(Method::HEAD)) {
    return Method::HEAD;
  }
  return Method::UNSUPPORTED;
}

auto ToVersion(const std::string &version_str) noexcept -> Version {
  auto version_formatted = Format(version_str);
  if (version_formatted == Version_String_Map.at(Version::HTTP_1_1)) {
    return Version::HTTP_1_1;
  }
  return Version::UNSUPPORTED;
}



























}  // namespace Icee::Http